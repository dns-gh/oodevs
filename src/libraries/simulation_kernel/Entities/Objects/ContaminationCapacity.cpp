// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "ContaminationCapacity.h"
#include "MIL_Object_ABC.h"
#include "MIL_NbcAgentType.h"
#include "NBCAttribute.h"
#include "ToxicAttribute_ABC.h"
#include "MIL_ToxicEffectManipulator.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Roles/NBC/PHY_RoleInterface_NBC.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Populations/MIL_PopulationElement_ABC.h"
#include "simulation_terrain/TER_Geometry.h"
#include <boost/ptr_container/serialize_ptr_vector.hpp>
#include <boost/foreach.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( ContaminationCapacity )

// -----------------------------------------------------------------------------
// Name: ContaminationCapacity constructor
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
ContaminationCapacity::ContaminationCapacity()
    : maxToxic_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ContaminationCapacity constructor
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
ContaminationCapacity::ContaminationCapacity( xml::xistream& xis )
    : maxToxic_( xis.attribute< int >( "max-toxic" ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ContaminationCapacity constructor
// Created: JCR 2008-06-13
// -----------------------------------------------------------------------------
ContaminationCapacity::ContaminationCapacity( const ContaminationCapacity& from )
    : maxToxic_( from.maxToxic_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ContaminationCapacity destructor
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
ContaminationCapacity::~ContaminationCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ContaminationCapacity::serialize
// Created: JCR 2008-08-28
// -----------------------------------------------------------------------------
template< typename Archive >
void ContaminationCapacity::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this )
         & maxToxic_
         & decontaminatedZones_;
}

// -----------------------------------------------------------------------------
// Name: ContaminationCapacity::Register
// Created: JCR 2008-07-03
// -----------------------------------------------------------------------------
void ContaminationCapacity::Register( MIL_Object_ABC& object )
{
    object.AddCapacity( this );
    object.Register( static_cast< MIL_InteractiveContainer_ABC *>( this ) );
}

// -----------------------------------------------------------------------------
// Name: ContaminationCapacity::Instanciate
// Created: JCR 2008-06-08
// -----------------------------------------------------------------------------
void ContaminationCapacity::Instanciate( MIL_Object_ABC& object ) const
{
    ContaminationCapacity* capacity = new ContaminationCapacity( *this );
    object.AddCapacity( capacity );
    object.Register( static_cast< MIL_InteractiveContainer_ABC *>( capacity ) );
}

// -----------------------------------------------------------------------------
// Name: ContaminationCapacity::ProcessAgentInside
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
void ContaminationCapacity::ProcessAgentInside( MIL_Object_ABC& object, MIL_Agent_ABC& agent )
{
    const MT_Vector2D& position = agent.GetRole< PHY_RoleInterface_Location >().GetPosition();
    const NBCAttribute* pNBC = object.RetrieveAttribute< NBCAttribute >();

    if( !IsInsideDecontaminatedZone( position ) && pNBC && pNBC->IsContaminating() )
    {
        const ToxicAttribute_ABC* pAttribute = object.RetrieveAttribute< ToxicAttribute_ABC >();
        if( pAttribute )
        {
            const MIL_ToxicEffectManipulator contamination( pAttribute->GetContaminationEffect( *pNBC, position ) );
            agent.GetRole< nbc::PHY_RoleInterface_NBC >().Contaminate( contamination );
        }
        else
        {
            const MIL_ToxicEffectManipulator contamination( object.GetAttribute< NBCAttribute >().GetNBCAgents(), 1 );
            agent.GetRole< nbc::PHY_RoleInterface_NBC >().Contaminate( contamination );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ContaminationCapacity::ProcessPopulationInside
// Created: LGY 2011-03-30
// -----------------------------------------------------------------------------
void ContaminationCapacity::ProcessPopulationInside( MIL_Object_ABC& object, MIL_PopulationElement_ABC& population )
{
    const NBCAttribute* pNBC = object.RetrieveAttribute< NBCAttribute >();
    if( pNBC && pNBC->IsContaminating() )
    {
        const NBCAttribute::T_NBCAgents& agents = pNBC->GetNBCAgents();
        for( auto it = agents.begin(); it != agents.end(); ++it )
            if( (*it)->IsGasContaminating() || (*it)->IsLiquidContaminating() )
                population.ApplyContamination( **it );
    }
}

// -----------------------------------------------------------------------------
// Name: ContaminationCapacity::IsAgentInsideDecontaminatedZone
// Created: NLD 2005-03-22
// -----------------------------------------------------------------------------
bool ContaminationCapacity::IsInsideDecontaminatedZone( const MT_Vector2D& position ) const
{
    for( auto it = decontaminatedZones_.begin(); it != decontaminatedZones_.end(); ++it )
        if( it->IsInside( position ) )
            return true;
    return false;
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
// Name: ContaminationCapacity::Decontaminate
// Created: JCR 2008-08-19
// -----------------------------------------------------------------------------
void ContaminationCapacity::DecontaminateZone( MIL_Object_ABC& object, const TER_Localisation& zone )
{
    decontaminatedZones_.push_back( new TER_Localisation( zone ) );

    if( zone.Contains( object.GetLocalisation() ) )
        object.MarkForDestruction();
    else if( object.GetLocalisation().GetPoints().size() > 1 )
    {
        std::vector< TER_Localisation > covers;
        GetLocationsFromPtrVector( decontaminatedZones_, covers );
        if( TER_Geometry::IsEntirelyCovered( object.GetLocalisation(), covers ) )
            object.MarkForDestruction();
    }
}
