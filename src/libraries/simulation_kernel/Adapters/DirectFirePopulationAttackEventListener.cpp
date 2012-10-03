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
#include <core/Facade.h>
#include <core/Model.h>

using namespace sword;

namespace
{
    const std::string event = "direct fire population attack";
}

//-----------------------------------------------------------------------------
// Name: DirectFirePopulationAttackEventListener constructor
// Created: MCO 2012-04-26
//-----------------------------------------------------------------------------
DirectFirePopulationAttackEventListener::DirectFirePopulationAttackEventListener( const core::Model& model, core::Facade& facade )
    : model_ ( model )
    , facade_( facade )
{
    facade.Register( event, *this );
}

//-----------------------------------------------------------------------------
// Name: DirectFirePopulationAttackEventListener destructor
// Created: MCO 2012-04-26
//-----------------------------------------------------------------------------
DirectFirePopulationAttackEventListener::~DirectFirePopulationAttackEventListener()
{
    facade_.Unregister( event, *this );
}

// -----------------------------------------------------------------------------
// Name: DirectFirePopulationAttackEventListener::Notify
// Created: MCO 2012-04-26
// -----------------------------------------------------------------------------
void DirectFirePopulationAttackEventListener::Notify( const core::Model& event )
{
    try
    {
        MIL_AgentPion& pion = event[ "entity/pion" ].GetUserData< MIL_AgentPion >();
        unsigned int nTargetKnowledgeID = event[ "population" ];
        const boost::shared_ptr< DEC_Knowledge_Population > pKnowledge = pion.GetKnowledgeGroup()->GetKnowledge().GetKnowledgePopulationFromID( nTargetKnowledgeID );
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
    catch( std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Exception in " << __FUNCTION__ << " : " << e.what() );
    }
    catch( ... )
    {
        MT_LOG_ERROR_MSG( "Unknown exception in " << __FUNCTION__ );
    }
}
