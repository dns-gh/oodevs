// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_AgentServer.h"
#include "SpawnCapacity.h"
#include "ChildObjectAttribute.h"
#include "Object.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "MIL_ObjectLoader.h"
#include "MIL_ObjectManipulator_ABC.h"
#include "MIL_AgentServer.h"
#include "Entities/MIL_EntityManager.h"
#include "MIL_Singletons.h"

#include "simulation_terrain/TER_Localisation.h"
#include "Tools/MIL_Tools.h"
#include "tools/xmlcodecs.h"
#include <xeumeuleu/xml.hpp>

BOOST_CLASS_EXPORT_GUID( SpawnCapacity, "SpawnCapacity" )

// -----------------------------------------------------------------------------
// Name: SpawnCapacity constructor
// Created: SLG 2010-02-17
// -----------------------------------------------------------------------------
SpawnCapacity::SpawnCapacity( xml::xistream& xis )
: rActionRange_( 0 )
{
    xis >> xml::attribute( "action-range", rActionRange_ )
        >> xml::attribute( "object", childType_ );
}

// -----------------------------------------------------------------------------
// Name: SpawnCapacity constructor
// Created: SLG 2010-02-17
// -----------------------------------------------------------------------------
SpawnCapacity::SpawnCapacity()
: rActionRange_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SpawnCapacity constructor
// Created: SLG 2010-02-17
// -----------------------------------------------------------------------------
SpawnCapacity::SpawnCapacity( const SpawnCapacity& from )
: rActionRange_( from.rActionRange_ )
, childType_( from.childType_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SpawnCapacity Destructor
// Created: SLG 2010-02-17
// -----------------------------------------------------------------------------
SpawnCapacity::~SpawnCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: template< typename Archive > void SpawnCapacity::serialize
// Created: SLG 2010-02-17
// -----------------------------------------------------------------------------
template< typename Archive >
void SpawnCapacity::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this )
        & boost::serialization::base_object< MIL_InteractiveContainer_ABC >( *this )
        & rActionRange_
        & childType_;
}

// -----------------------------------------------------------------------------
// Name: SpawnCapacity::Register
// Created: MGD 2009-03-05
// -----------------------------------------------------------------------------
void SpawnCapacity::Register( MIL_Object_ABC& object )
{
    object.AddCapacity( this );
    object.Register( static_cast< MIL_InteractiveContainer_ABC *>( this ) );
}

// -----------------------------------------------------------------------------
// Name: SpawnCapacity::Instanciate
// Created: MGD 2009-03-05
// -----------------------------------------------------------------------------
void SpawnCapacity::Instanciate( MIL_Object_ABC& object ) const
{
    SpawnCapacity* capacity = new SpawnCapacity( *this );
    object.AddCapacity( capacity );
    TER_Localisation location = object.GetLocalisation();
    location.Scale( rActionRange_ );
    try
    {
        Object* childObject= static_cast< Object* >( MIL_Singletons::GetEntityManager().CreateObject( childType_, *object.GetArmy(), location ) );
        object.GetAttribute< ChildObjectAttribute >().AddChildObject( *childObject );
        object.Register( static_cast< MIL_InteractiveContainer_ABC *>( capacity ) );
    }
    catch( std::exception& e )
    {
        MT_LOG_ERROR_MSG( e.what() );
    }
}

// -----------------------------------------------------------------------------
// Name: SpawnCapacity::AddCreator
// Created: SLG 2010-02-16
// -----------------------------------------------------------------------------
void SpawnCapacity::AddCreator( MIL_Object_ABC& object, const MIL_Agent_ABC& agent )
{
    Object* childObject = object.RetrieveAttribute< ChildObjectAttribute >()->GetChildObject();
    if( childObject )
        childObject->operator ()().AddDetector( agent );
}