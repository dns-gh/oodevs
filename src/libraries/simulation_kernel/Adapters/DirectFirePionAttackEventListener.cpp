// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DirectFirePionAttackEventListener.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "MT_Tools/MT_Logger.h"
#include <core/Facade.h>
#include <core/Model.h>

using namespace sword;

namespace
{
    const std::string event = "direct fire pion attack";
}

//-----------------------------------------------------------------------------
// Name: DirectFirePionAttackEventListener constructor
// Created: MCO 2012-04-26
//-----------------------------------------------------------------------------
DirectFirePionAttackEventListener::DirectFirePionAttackEventListener( core::Facade& facade )
    : facade_( facade )
{
    facade.Register( event, *this );
}

//-----------------------------------------------------------------------------
// Name: DirectFirePionAttackEventListener destructor
// Created: MCO 2012-04-26
//-----------------------------------------------------------------------------
DirectFirePionAttackEventListener::~DirectFirePionAttackEventListener()
{
    facade_.Unregister( event, *this );
}

// -----------------------------------------------------------------------------
// Name: DirectFirePionAttackEventListener::Notify
// Created: MCO 2012-04-26
// -----------------------------------------------------------------------------
void DirectFirePionAttackEventListener::Notify( const core::Model& event )
{
    try
    {
        MIL_AgentPion& pion = event[ "entity/data" ].GetUserData< MIL_AgentPion >();
        MIL_Agent_ABC& target = event[ "enemy/data" ].GetUserData< boost::shared_ptr< DEC_Knowledge_Agent > >()->GetAgentKnown();
        if( ! event[ "paused" ] )
            pion.NotifyAttacking( target, event[ "report" ] );
        target.NotifyAttackedBy( pion, event[ "report" ] );
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
