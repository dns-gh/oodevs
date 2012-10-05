// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "ExternalPerceptionEventListener.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_AgentPion.h"
#include "Knowledge/DEC_KS_Perception.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "MT_Tools/MT_Logger.h"
#include <core/Facade.h>
#include <core/Model.h>

using namespace sword;

namespace
{
    const std::string event = "external perception";
}

//-----------------------------------------------------------------------------
// Name: ExternalPerceptionEventListener constructor
// Created: MCO 2012-04-26
//-----------------------------------------------------------------------------
ExternalPerceptionEventListener::ExternalPerceptionEventListener( const core::Model& model, core::Facade& facade, tools::Resolver< MIL_AgentPion >& resolver )
    : model_   ( model )
    , facade_  ( facade )
    , resolver_( resolver )
{
    facade.Register( event, *this );
}

//-----------------------------------------------------------------------------
// Name: ExternalPerceptionEventListener destructor
// Created: MCO 2012-04-26
//-----------------------------------------------------------------------------
ExternalPerceptionEventListener::~ExternalPerceptionEventListener()
{
    facade_.Unregister( event, *this );
}

// -----------------------------------------------------------------------------
// Name: ExternalPerceptionEventListener::Notify
// Created: MCO 2012-04-26
// -----------------------------------------------------------------------------
void ExternalPerceptionEventListener::Notify( const core::Model& event )
{
    try
    {
        MIL_AgentPion& pion = resolver_.Get( static_cast< unsigned int >( event[ "identifier" ] ) );
        MIL_Agent_ABC& target = event[ "target/data" ].GetUserData< MIL_AgentPion >();
        const PHY_PerceptionLevel& level = PHY_PerceptionLevel::FindPerceptionLevel( event[ "level" ] );
        pion.GetKnowledge().GetKsPerception().NotifyExternalPerception( target, level );
    }
    catch( std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Exception in " << __FUNCTION__ << " : " << e.what() );
    }
    catch( ... )
    {
        MT_LOG_ERROR_MSG( "Unknown exception in " << __FUNCTION__ );
    }
}
