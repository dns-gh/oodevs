// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "SpawnCapacity.h"
#include "MIL_AgentServer.h"
#include "ChildObjectAttribute.h"
#include "SpawnedAttribute.h"
#include "Object.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "MIL_ObjectManipulator_ABC.h"
#include "Entities/MIL_EntityManager.h"
#include "MT_Tools/MT_Logger.h"
#include "simulation_terrain/TER_Localisation.h"
#include "Tools/MIL_Tools.h"
#include "tools/Codec.h"
#include "MIL_NbcAgentType.h"
#include "NBCAttribute.h"

BOOST_CLASS_EXPORT_GUID( SpawnCapacity, "SpawnCapacity" )

// -----------------------------------------------------------------------------
// Name: SpawnCapacity constructor
// Created: SLG 2010-02-17
// -----------------------------------------------------------------------------
SpawnCapacity::SpawnCapacity( xml::xistream& xis )
    : rActionRange_( 0 )
    , nbc_         ( false )
{
    xis >> xml::attribute( "action-range", rActionRange_ )
        >> xml::attribute( "object", childType_ )
        >> xml::attribute( "nbc", nbc_ );
}

// -----------------------------------------------------------------------------
// Name: SpawnCapacity constructor
// Created: SLG 2010-02-17
// -----------------------------------------------------------------------------
SpawnCapacity::SpawnCapacity()
    : rActionRange_( 0 )
    , nbc_         ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SpawnCapacity constructor
// Created: SLG 2010-02-17
// -----------------------------------------------------------------------------
SpawnCapacity::SpawnCapacity( const SpawnCapacity& from )
    : rActionRange_( from.rActionRange_ )
    , childType_   ( from.childType_ )
    , nbc_         ( from.nbc_ )
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
// Name: SpawnCapacity::serialize
// Created: SLG 2010-02-17
// -----------------------------------------------------------------------------
template< typename Archive >
void SpawnCapacity::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this )
        & rActionRange_
        & childType_
        & nbc_;
}

// -----------------------------------------------------------------------------
// Name: SpawnCapacity::Register
// Created: MGD 2009-03-05
// -----------------------------------------------------------------------------
void SpawnCapacity::Register( MIL_Object_ABC& object )
{
    object.AddCapacity( this );
}

// -----------------------------------------------------------------------------
// Name: SpawnCapacity::Instanciate
// Created: MGD 2009-03-05
// -----------------------------------------------------------------------------
void SpawnCapacity::Instanciate( MIL_Object_ABC& object ) const
{
    SpawnCapacity* capacity = new SpawnCapacity( *this );
    object.AddCapacity( capacity );
}

// -----------------------------------------------------------------------------
// Name: SpawnCapacity::Finalize
// Created: LGY 2011-09-26
// -----------------------------------------------------------------------------
void SpawnCapacity::Finalize( MIL_Object_ABC& object )
{
    try
    {
        if( nbc_ )
            CreateNBCObject( object );
        else
            CreateObject( object );
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( tools::GetExceptionMsg( e ) );
    }
}

// -----------------------------------------------------------------------------
// Name: SpawnCapacity::CreateObject
// Created: LGY 2011-09-26
// -----------------------------------------------------------------------------
void SpawnCapacity::CreateObject( MIL_Object_ABC& object )
{
    TER_Localisation location = object.GetLocalisation();
    location.Scale( rActionRange_ );
    Object* childObject = static_cast< Object* >( MIL_AgentServer::GetWorkspace().GetEntityManager().CreateObject( childType_,
        object.GetArmy(), location, object.GetID() + 1 ) );
    if( childObject )
    {
        object.GetAttribute< ChildObjectAttribute >().AddChildObject( *childObject );
        childObject->GetAttribute< SpawnedAttribute >();
    }
}

namespace
{
    bool IsGas( const NBCAttribute& attribute )
    {
        const NBCAttribute::T_NBCAgents& agents = attribute.GetNBCAgents();
        for( auto it = agents.begin(); it != agents.end(); ++it )
            if( (*it)->IsGasContaminating() || (*it)->IsGasPoisonous() )
                return true;
        return false;
    }
}

// -----------------------------------------------------------------------------
// Name: SpawnCapacity::CreateNBCObject
// Created: LGY 2011-09-26
// -----------------------------------------------------------------------------
void SpawnCapacity::CreateNBCObject( MIL_Object_ABC& object )
{
    const NBCAttribute* pNBC = object.RetrieveAttribute< NBCAttribute >();
    if( pNBC && IsGas( *pNBC ) )
    {
        MIL_Object_ABC* pObject = MIL_AgentServer::GetWorkspace().GetEntityManager().CreateObject( childType_, object.GetArmy(), object.GetLocalisation(), object.GetID() + 1 );
        pObject->GetAttribute< NBCAttribute >().UpdateCloudAttribute( *pNBC );
    }
}

// -----------------------------------------------------------------------------
// Name: SpawnCapacity::AddCreator
// Created: SLG 2010-02-16
// -----------------------------------------------------------------------------
void SpawnCapacity::AddCreator( MIL_Object_ABC& object, const MIL_Agent_ABC& agent )
{
    MIL_Object_ABC* childObject = object.RetrieveAttribute< ChildObjectAttribute >()->GetChildObject();
    if( childObject )
        (*childObject)().AddDetector( agent );
}
