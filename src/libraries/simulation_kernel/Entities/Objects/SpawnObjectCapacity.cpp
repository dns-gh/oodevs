// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "SpawnObjectCapacity.h"
#include "MIL_Object_ABC.h"
#include "MIL_AgentServer.h"
#include "MIL_NbcAgentType.h"
#include "NBCAttribute.h"
#include "Entities/MIL_EntityManager.h"
#include "MT_Tools/MT_Logger.h"

// -----------------------------------------------------------------------------
// Name: SpawnObjectCapacity constructor
// Created: LGY 2011-08-17
// -----------------------------------------------------------------------------
SpawnObjectCapacity::SpawnObjectCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SpawnObjectCapacity constructor
// Created: LGY 2011-08-17
// -----------------------------------------------------------------------------
SpawnObjectCapacity::SpawnObjectCapacity( xml::xistream& xis )
    : type_( xis.attribute< std::string >( "object" ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SpawnObjectCapacity constructor
// Created: LGY 2011-08-17
// -----------------------------------------------------------------------------
SpawnObjectCapacity::SpawnObjectCapacity( const SpawnObjectCapacity& capacity )
    : type_( capacity.type_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SpawnObjectCapacity destructor
// Created: LGY 2011-08-17
// -----------------------------------------------------------------------------
SpawnObjectCapacity::~SpawnObjectCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SpawnObjectCapacity::serialize
// Created: LGY 2011-08-17
// -----------------------------------------------------------------------------
template< typename Archive >
void SpawnObjectCapacity::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this )
         & type_;
}

// -----------------------------------------------------------------------------
// Name: SpawnObjectCapacity::Instanciate
// Created: LGY 2011-08-17
// -----------------------------------------------------------------------------
void SpawnObjectCapacity::Instanciate( MIL_Object_ABC& object ) const
{
    SpawnObjectCapacity* capacity = new SpawnObjectCapacity( *this );
    object.AddCapacity( capacity );
}

// -----------------------------------------------------------------------------
// Name: SpawnObjectCapacity::Register
// Created: LGY 2011-08-17
// -----------------------------------------------------------------------------
void SpawnObjectCapacity::Register( MIL_Object_ABC& object )
{
    object.AddCapacity( this );
}

namespace
{
    bool IsGas( const NBCAttribute& attribute )
    {
        const NBCAttribute::T_NBCAgents& agents = attribute.GetNBCAgents();
        for( NBCAttribute::CIT_NBCAgents it = agents.begin(); it != agents.end(); ++it )
            if( (*it)->IsGasContaminating() || (*it)->IsGasPoisonous() )
                return true;
        return false;
    }
}

// -----------------------------------------------------------------------------
// Name: SpawnObjectCapacity::Finalize
// Created: LGY 2011-08-17
// -----------------------------------------------------------------------------
void SpawnObjectCapacity::Finalize( MIL_Object_ABC& object )
{
    try
    {
        const NBCAttribute* pNBC = object.RetrieveAttribute< NBCAttribute >();
        if( pNBC && IsGas( *pNBC ) )
        {
            MIL_Object_ABC* pObject = MIL_AgentServer::GetWorkspace().GetEntityManager().CreateObject( type_, *object.GetArmy(), object.GetLocalisation() );
            pObject->GetAttribute< NBCAttribute >().Update( *pNBC );
        }
    }
    catch( std::exception& e )
    {
        MT_LOG_ERROR_MSG( e.what() );
    }
}
