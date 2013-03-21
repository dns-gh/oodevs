// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DirectFirePopulationAttackEventListener.h"
#include "Entities/Populations/MIL_Population.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "Knowledge/DEC_Knowledge_Population.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "MT_Tools/MT_Logger.h"
#include <core/Model.h>

using namespace sword;

//-----------------------------------------------------------------------------
// Name: DirectFirePopulationAttackEventListener constructor
// Created: MCO 2012-04-26
//-----------------------------------------------------------------------------
DirectFirePopulationAttackEventListener::DirectFirePopulationAttackEventListener( core::Facade& facade )
    : EventListenerBase( facade, "direct fire population attack" )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: DirectFirePopulationAttackEventListener destructor
// Created: MCO 2012-04-26
//-----------------------------------------------------------------------------
DirectFirePopulationAttackEventListener::~DirectFirePopulationAttackEventListener()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DirectFirePopulationAttackEventListener::Notify
// Created: MCO 2012-04-26
// -----------------------------------------------------------------------------
void DirectFirePopulationAttackEventListener::Notify( const core::Model& event )
{
    try
    {
        MIL_AgentPion& pion = event[ "entity/data" ].GetUserData< MIL_AgentPion >();
        unsigned int nTargetKnowledgeID = event[ "population" ];
        auto bbKg = pion.GetKnowledgeGroup()->GetKnowledge();
        if( !bbKg )
            return;
        const boost::shared_ptr< DEC_Knowledge_Population > pKnowledge = bbKg->GetKnowledgePopulationFromID( nTargetKnowledgeID );
        if( ! pKnowledge )
        {
            MT_LOG_ERROR_MSG( "Error in " << __FUNCTION__ << " : no population associated to knowldge identifier" );
            return;
        }
        MIL_Population& target = pKnowledge->GetPopulationKnown();
        if( ! event[ "paused" ] )
            pion.NotifyAttacking( target );
        target.NotifyAttackedBy( pion );
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Exception in " << __FUNCTION__ << " : " << tools::GetExceptionMsg( e ) );
    }
    catch( ... )
    {
        MT_LOG_ERROR_MSG( "Unknown exception in " << __FUNCTION__ );
    }
}
