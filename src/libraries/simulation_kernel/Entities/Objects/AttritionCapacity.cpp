// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "AttritionCapacity.h"
#include "MIL_Object_ABC.h"
#include "PHY_ObjectExplosionFireResult.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Agents/Roles/Transported/PHY_RolePion_Transported.h"
#include "Entities/Populations/MIL_PopulationElement_ABC.h"
#include "Entities/MIL_Army.h"
#include "Entities/Orders/MIL_Report.h"
#include "Entities/Objects/MIL_ObjectType_ABC.h"
#include "ObstacleAttribute.h"
#include "BypassAttribute.h"
#include "ConstructionAttribute.h"

BOOST_CLASS_EXPORT_IMPLEMENT( AttritionCapacity )

// -----------------------------------------------------------------------------
// Name: AttritionCapacity constructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
AttritionCapacity::AttritionCapacity( xml::xistream& xis )
    : dotation_( PHY_DotationType::FindDotationCategory( xis.attribute< std::string >( "category", std::string() ) ) )
{
    if( !dotation_ )
        throw MASA_EXCEPTION( xis.context() + "Unknown dotation category '"
            + xis.attribute< std::string >( "category" ) + "' for attrition capacity" );
    xis >> xml::attribute( "attrition-surface", population_.surface_ )
        >> xml::attribute( "ph", population_.ph_ );
}

// -----------------------------------------------------------------------------
// Name: AttritionCapacity constructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
AttritionCapacity::AttritionCapacity()
    : dotation_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AttritionCapacity constructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
AttritionCapacity::AttritionCapacity( const AttritionCapacity& from )
    : dotation_  ( from.dotation_ )
    , population_( from.population_)
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AttritionCapacity destructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
AttritionCapacity::~AttritionCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AttritionCapacity::load
// Created: JCR 2008-07-03
// -----------------------------------------------------------------------------
void AttritionCapacity::load( MIL_CheckPointInArchive& ar, const unsigned int )
{
    ar >> boost::serialization::base_object< ObjectCapacity_ABC >( *this );
    ar >> boost::serialization::base_object< MIL_InteractiveContainer_ABC >( *this );
    ar >> dotation_;
    ar >> population_.surface_;
    ar >> population_.ph_;
}

// -----------------------------------------------------------------------------
// Name: AttritionCapacity::save
// Created: JCR 2008-07-03
// -----------------------------------------------------------------------------
void AttritionCapacity::save( MIL_CheckPointOutArchive& ar, const unsigned int ) const
{
    ar << boost::serialization::base_object< ObjectCapacity_ABC >( *this );
    ar << boost::serialization::base_object< MIL_InteractiveContainer_ABC >( *this );
    ar << dotation_;
    ar << population_.surface_;
    ar << population_.ph_;
}

// -----------------------------------------------------------------------------
// Name: AttritionCapacity::Register
// Created: JCR 2008-07-03
// -----------------------------------------------------------------------------
void AttritionCapacity::Register( MIL_Object_ABC& object )
{
    object.AddCapacity( this );
    object.Register( static_cast< MIL_InteractiveContainer_ABC *>( this ) );
}

// -----------------------------------------------------------------------------
// Name: AttritionCapacity::Instanciate
// Created: JCR 2008-06-08
// -----------------------------------------------------------------------------
void AttritionCapacity::Instanciate( MIL_Object_ABC& object ) const
{
    AttritionCapacity* capacity = new AttritionCapacity( *this );
    object.AddCapacity( capacity );
    object.Register( static_cast< MIL_InteractiveContainer_ABC *>( capacity ) );
}

namespace
{
    // contract of explosable = has a method ApplyExplosion( const AttritionCapacity& capacity, PHY_FireResults_ABC& result );
    template< typename T > void ApplyExplosion( AttritionCapacity& attrition, MIL_Object_ABC& object, const PHY_DotationCategory* dotation, T& explosable )
    {
        const BypassAttribute* bypass = object.RetrieveAttribute< BypassAttribute >();
        if( bypass && bypass->IsBypassed() )
            return;
        const ObstacleAttribute* obstacle = object.RetrieveAttribute< ObstacleAttribute >();
        if( obstacle && !obstacle->IsActivated() )
            return;
        ConstructionAttribute* construction = object.RetrieveAttribute< ConstructionAttribute >();
        if( ! ( construction && construction->HasDotation( *dotation ) && dotation->HasAttritions() ) )
            return;
        PHY_ObjectExplosionFireResult fireResult( object );
        explosable.ApplyExplosion( attrition, fireResult );
        unsigned int hits = fireResult.GetHits();
        if( hits > 0 )
            construction->Build( - static_cast< double >( hits ) / construction->GetMaxDotation() );
    }
}

// -----------------------------------------------------------------------------
// Name: AttritionCapacity::ProcessAgentMovingInside
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
void AttritionCapacity::ProcessAgentMovingInside( MIL_Object_ABC& object, MIL_Agent_ABC& agent )
{
    if( object.GetArmy() && object.GetArmy()->GetID() == agent.GetArmy().GetID() )
        return;
    if( agent.GetRole< transport::PHY_RolePion_Transported >().IsTransported() )
        return;
    ApplyExplosion< PHY_RoleInterface_Composantes >( *this, object, dotation_, agent.GetRole< PHY_RoleInterface_Composantes >() );
}

// -----------------------------------------------------------------------------
// Name: AttritionCapacity::ProcessPopulationInside
// Created: JCR 2008-06-06
// -----------------------------------------------------------------------------
void AttritionCapacity::ProcessPopulationInside( MIL_Object_ABC& object, MIL_PopulationElement_ABC& population )
{
    if( population_.surface_ <= 0. )
        return;
    ApplyExplosion< MIL_PopulationElement_ABC >( *this, object, dotation_, population );
}

// -----------------------------------------------------------------------------
// Name: AttritionCapacity::GetAttritionData
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
const PHY_AttritionData& AttritionCapacity::GetAttritionData( const PHY_Protection& protection ) const
{
    assert ( dotation_ );
    return dotation_->GetAttritionData( protection );
}

// -----------------------------------------------------------------------------
// Name: AttritionCapacity::GetAttritionSurface
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
double AttritionCapacity::GetAttritionSurface() const
{
    return population_.surface_;
}

// -----------------------------------------------------------------------------
// Name: AttritionCapacity::GetAttritionSurface
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
double AttritionCapacity::GetAttritionPH() const
{
    return population_.ph_;
}
