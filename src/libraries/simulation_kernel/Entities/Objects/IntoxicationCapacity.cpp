// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "IntoxicationCapacity.h"
#include "MIL_NbcAgentType.h"
#include "NBCAttribute.h"
#include "MIL_Object_ABC.h"
#include "ToxicAttribute_ABC.h"
#include "MIL_ToxicEffectManipulator.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Roles/NBC/PHY_RoleInterface_NBC.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Populations/MIL_PopulationElement_ABC.h"
#include "simulation_terrain/TER_Geometry.h"
#include <boost/ptr_container/serialize_ptr_vector.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( IntoxicationCapacity )

// -----------------------------------------------------------------------------
// Name: IntoxicationCapacity constructor
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
IntoxicationCapacity::IntoxicationCapacity( xml::xistream& xis )
    : maxToxic_( xis.attribute< int >( "max-toxic" ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IntoxicationCapacity constructor
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
IntoxicationCapacity::IntoxicationCapacity()
    : maxToxic_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IntoxicationCapacity constructor
// Created: JCR 2008-06-13
// -----------------------------------------------------------------------------
IntoxicationCapacity::IntoxicationCapacity( const IntoxicationCapacity& from )
    : maxToxic_( from.maxToxic_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IntoxicationCapacity destructor
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
IntoxicationCapacity::~IntoxicationCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IntoxicationCapacity::serialize
// Created: JCR 2008-08-28
// -----------------------------------------------------------------------------
template< typename Archive >
void IntoxicationCapacity::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this );
    file & maxToxic_;
    file & desintoxicatedZones_;
}

// -----------------------------------------------------------------------------
// Name: IntoxicationCapacity::Register
// Created: JCR 2008-07-03
// -----------------------------------------------------------------------------
void IntoxicationCapacity::Register( MIL_Object_ABC& object )
{
    object.AddCapacity( this );
    object.Register( static_cast< MIL_InteractiveContainer_ABC *>( this ) );
}

// -----------------------------------------------------------------------------
// Name: IntoxicationCapacity::Instanciate
// Created: JCR 2008-06-08
// -----------------------------------------------------------------------------
void IntoxicationCapacity::Instanciate( MIL_Object_ABC& object ) const
{
    IntoxicationCapacity* capacity = new IntoxicationCapacity( *this );
    object.AddCapacity( capacity );
    object.Register( static_cast< MIL_InteractiveContainer_ABC *>( capacity ) );
}

// -----------------------------------------------------------------------------
// Name: IntoxicationCapacity::ProcessAgentInside
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
void IntoxicationCapacity::ProcessAgentInside( MIL_Object_ABC& object, MIL_Agent_ABC& agent )
{
    const MT_Vector2D& position = agent.GetRole< PHY_RoleInterface_Location >().GetPosition();
    const NBCAttribute* pNBC = object.RetrieveAttribute< NBCAttribute >();
    if( !IsInsideDesintoxicatedZone( position ) && pNBC && pNBC->IsPoisonous() )
    {
        const ToxicAttribute_ABC* pAttribute = object.RetrieveAttribute< ToxicAttribute_ABC >();
        if( pAttribute )
        {
            const MIL_ToxicEffectManipulator contamination( pAttribute->GetContaminationEffect( *pNBC, position ) );
            agent.GetRole< nbc::PHY_RoleInterface_NBC >().Poison( contamination );
        }
        else
        {
            const MIL_ToxicEffectManipulator contamination( object.GetAttribute< NBCAttribute >().GetNBCAgents(), 1 );
            agent.GetRole< nbc::PHY_RoleInterface_NBC >().Poison( contamination );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: IntoxicationCapacity::ProcessPopulationInside
// Created: LGY 2011-03-30
// -----------------------------------------------------------------------------
void IntoxicationCapacity::ProcessPopulationInside( MIL_Object_ABC& object, MIL_PopulationElement_ABC& population )
{
    const NBCAttribute* pNBC = object.RetrieveAttribute< NBCAttribute >();
    if( pNBC && pNBC->IsPoisonous() )
    {
        const NBCAttribute::T_NBCAgents& agents = pNBC->GetNBCAgents();
        for( auto it = agents.begin(); it != agents.end(); ++it )
            if( (*it)->IsGasPoisonous() || (*it)->IsLiquidPoisonous() )
                population.ApplyIntoxication( **it );
    }
}

namespace
{
    void GetLocationsFromPtrVector( const boost::ptr_vector< TER_Localisation >& ptrLocations, std::vector< TER_Localisation >& locations )
    {
        locations.reserve( ptrLocations.size() );
        for( auto it = ptrLocations.begin(); it != ptrLocations.end(); ++it )
            if( (*it).GetPoints().size() > 2 )
                locations.push_back( *it );
    }
}

// -----------------------------------------------------------------------------
// Name: IntoxicationCapacity::DesintoxicateZone
// Created: JSR 2010-08-18
// -----------------------------------------------------------------------------
void IntoxicationCapacity::DesintoxicateZone( MIL_Object_ABC& object, const TER_Localisation& zone )
{
    desintoxicatedZones_.push_back( new TER_Localisation( zone ) );

    if( zone.Contains( object.GetLocalisation() ) )
        object.MarkForDestruction();
    else if( object.GetLocalisation().GetPoints().size() > 1 )
    {
        std::vector< TER_Localisation > covers;
        GetLocationsFromPtrVector( desintoxicatedZones_, covers );
        if( TER_Geometry::IsEntirelyCovered( object.GetLocalisation(), covers ) )
            object.MarkForDestruction();
    }
}

// -----------------------------------------------------------------------------
// Name: IntoxicationCapacity::IsInsideDesintoxicatedZone
// Created: JSR 2010-08-18
// -----------------------------------------------------------------------------
bool IntoxicationCapacity::IsInsideDesintoxicatedZone( const MT_Vector2D& position ) const
{
    for( auto it = desintoxicatedZones_.begin(); it != desintoxicatedZones_.end(); ++it )
        if( it->IsInside( position ) )
            return true;
    return false;
}
