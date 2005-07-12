// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: NLD 2004-03-18 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Gtia.cpp $
// $Author: Nld $
// $Modtime: 14/02/05 14:26 $
// $Revision: 3 $
// $Workfile: MOS_Gtia.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light_Pch.h"

#include "MOS_Gtia.h"

#include "MOS_AgentKnowledge.h"
#include "MOS_KnowledgeList_View.h"
#include "MOS_App.h"
#include "MOS_AgentManager.h"

// -----------------------------------------------------------------------------
// Name: MOS_Gtia constructor
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
MOS_Gtia::MOS_Gtia( uint nID, DIN::DIN_Input& input )
    : nID_              ( nID )
    , pKnowledgeListView_( 0 )
    , bShouldDrawKnowledges_( false )
{
    uint32 nArmyID;
    input >> nArmyID;
    pArmy_ = MOS_App::GetApp().GetAgentManager().FindTeam( nArmyID );
    assert( pArmy_ );

    MOS_App::GetApp().GetAgentManager().RegisterGtia( *this );
}
    
// -----------------------------------------------------------------------------
// Name: MOS_Gtia destructor
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
MOS_Gtia::~MOS_Gtia()
{
    for( CIT_AgentKnowledgeMap it = agentKnowledges_.begin(); it != agentKnowledges_.end(); ++it )
        delete it->second;
    agentKnowledges_.clear();
    pKnowledgeListView_ = 0;
}
    

// =============================================================================
// NETWORK EVENTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MOS_Gtia::OnReceiveMsgUnitKnowledgeCreation
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void MOS_Gtia::OnReceiveMsgUnitKnowledgeCreation( const ASN1T_MsgUnitKnowledgeCreation& asnMsg )
{
    if( agentKnowledges_.find( asnMsg.oid_connaissance ) != agentKnowledges_.end() )
        return;

    MOS_AgentKnowledge* pAgentKnowledge = new MOS_AgentKnowledge( asnMsg );
    agentKnowledges_.insert( std::make_pair( pAgentKnowledge->GetID(), pAgentKnowledge ) );

    if( pKnowledgeListView_ )            
        pKnowledgeListView_->AddKnowledge( *pAgentKnowledge );
}

// -----------------------------------------------------------------------------
// Name: MOS_Gtia::OnReceiveMsgUnitKnowledgeUpdate
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void MOS_Gtia::OnReceiveMsgUnitKnowledgeUpdate( const ASN1T_MsgUnitKnowledgeUpdate&      asnMsg )
{
    IT_AgentKnowledgeMap itAgentKnowledge = agentKnowledges_.find( asnMsg.oid_connaissance );
    assert( itAgentKnowledge != agentKnowledges_.end() );

    itAgentKnowledge->second->Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: MOS_Gtia::OnReceiveMsgUnitKnowledgeDestruction
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void MOS_Gtia::OnReceiveMsgUnitKnowledgeDestruction( const ASN1T_MsgUnitKnowledgeDestruction& asnMsg )
{
    IT_AgentKnowledgeMap itAgentKnowledge = agentKnowledges_.find( asnMsg.oid_connaissance );
    assert( itAgentKnowledge != agentKnowledges_.end() );

    if( pKnowledgeListView_ )            
        pKnowledgeListView_->RemoveKnowledge( *itAgentKnowledge->second );

    delete itAgentKnowledge->second;
    agentKnowledges_.erase( itAgentKnowledge );
}


// -----------------------------------------------------------------------------
// Name: MOS_Gtia::SetAgentKnowledgeListView
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void MOS_Gtia::SetAgentKnowledgeListView( MOS_KnowledgeList_View* pListView )
{
    pKnowledgeListView_ = pListView;
    if( !pKnowledgeListView_ )
        return;

    for( CIT_AgentKnowledgeMap itKnowledge = agentKnowledges_.begin(); itKnowledge != agentKnowledges_.end(); ++itKnowledge )
        pKnowledgeListView_->AddKnowledge( *itKnowledge->second );
}


// -----------------------------------------------------------------------------
// Name: MOS_Gtia::ShouldDrawKnowledges
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
bool MOS_Gtia::ShouldDrawKnowledges() const
{
    return bShouldDrawKnowledges_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Gtia::NotifyShouldDrawKnowledges
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void MOS_Gtia::NotifyShouldDrawKnowledges( bool bDraw )
{
    bShouldDrawKnowledges_ = bDraw;    
}

// -----------------------------------------------------------------------------
// Name: MOS_Gtia::Draw
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void MOS_Gtia::Draw()
{
    if( !bShouldDrawKnowledges_ )
        return;

    for( CIT_AgentKnowledgeMap itKnowledge = agentKnowledges_.begin(); itKnowledge != agentKnowledges_.end(); ++itKnowledge )
        itKnowledge->second->Draw();
}
