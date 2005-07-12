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
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Team.cpp $
// $Author: Nld $
// $Modtime: 14/02/05 11:26 $
// $Revision: 2 $
// $Workfile: MOS_Team.cpp $
//
// *****************************************************************************

#include "MOS_Light_Pch.h"

#include "MOS_Team.h"

#include "MOS_ObjectKnowledge.h"
#include "MOS_ObjectKnowledge_ListView.h"
#include "MOS_App.h"
#include "MOS_AgentManager.h"

static int nColorID_ = 0; //$$ DEGUEU

// -----------------------------------------------------------------------------
// Name: MOS_Team constructor
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
MOS_Team::MOS_Team( uint nID, DIN::DIN_Input& input )
    : strName_              ( )
    , nID_                  ( nID  )
    , bShouldDrawKnowledges_( false )
    , pKnowledgeListView_   ( 0 )
    , color_                ( GFX_Tools::GetColorForTeam( nColorID_ ++ ) )
{
    input >> strName_;

    MOS_App::GetApp().GetAgentManager().RegisterTeam( *this );
}
    
// -----------------------------------------------------------------------------
// Name: MOS_Team destructor
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
MOS_Team::~MOS_Team()
{
    DestroyAllKnowledges();
}

// -----------------------------------------------------------------------------
// Name: MOS_Team::DestroyAllKnowledges
// Created: NLD 2004-03-26
// -----------------------------------------------------------------------------
void MOS_Team::DestroyAllKnowledges()
{
    for( CIT_ObjectKnowledgeMap it = objectKnowledges_.begin(); it != objectKnowledges_.end(); ++it )
        delete it->second;
    objectKnowledges_.clear();
    pKnowledgeListView_ = 0;
}
    
// =============================================================================
// NETWORK EVENTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MOS_Team::OnReceiveMsgObjectKnowledgeCreation
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void MOS_Team::OnReceiveMsgObjectKnowledgeCreation( const ASN1T_MsgObjectKnowledgeCreation& asnMsg )
{
    if( objectKnowledges_.find( asnMsg.oid_connaissance ) != objectKnowledges_.end() )
        return;

    MOS_ObjectKnowledge* pObjectKnowledge = new MOS_ObjectKnowledge( asnMsg );
    objectKnowledges_.insert( std::make_pair( pObjectKnowledge->GetID(), pObjectKnowledge ) );

    if( pKnowledgeListView_ )            
        pKnowledgeListView_->AddKnowledge( *pObjectKnowledge );
}

// -----------------------------------------------------------------------------
// Name: MOS_Team::OnReceiveMsgObjectKnowledgeUpdate
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void MOS_Team::OnReceiveMsgObjectKnowledgeUpdate( const ASN1T_MsgObjectKnowledgeUpdate&      asnMsg )
{
    IT_ObjectKnowledgeMap itObjectKnowledge = objectKnowledges_.find( asnMsg.oid_connaissance );
    assert( itObjectKnowledge != objectKnowledges_.end() );

    itObjectKnowledge->second->Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: MOS_Team::OnReceiveMsgObjectKnowledgeDestruction
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void MOS_Team::OnReceiveMsgObjectKnowledgeDestruction( const ASN1T_MsgObjectKnowledgeDestruction& asnMsg )
{
    IT_ObjectKnowledgeMap itObjectKnowledge = objectKnowledges_.find( asnMsg.oid_connaissance );
    assert( itObjectKnowledge != objectKnowledges_.end() );

    if( pKnowledgeListView_ )            
        pKnowledgeListView_->RemoveKnowledge( *itObjectKnowledge->second );

    delete itObjectKnowledge->second;
    objectKnowledges_.erase( itObjectKnowledge );
}

// -----------------------------------------------------------------------------
// Name: MOS_Team::SetObjectKnowledgeListView
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void MOS_Team::SetObjectKnowledgeListView( MOS_ObjectKnowledge_ListView* pListView )
{
    pKnowledgeListView_ = pListView;
    if( !pKnowledgeListView_ )
        return;

    for( CIT_ObjectKnowledgeMap itKnowledge = objectKnowledges_.begin(); itKnowledge != objectKnowledges_.end(); ++itKnowledge )
        pKnowledgeListView_->AddKnowledge( *itKnowledge->second );
}


// -----------------------------------------------------------------------------
// Name: MOS_Team::ShouldDrawKnowledges
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
bool MOS_Team::ShouldDrawKnowledges() const
{
    return bShouldDrawKnowledges_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Team::NotifyShouldDrawKnowledges
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void MOS_Team::NotifyShouldDrawKnowledges( bool bDraw )
{
    bShouldDrawKnowledges_ = bDraw;    
}

// -----------------------------------------------------------------------------
// Name: MOS_Team::Draw
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void MOS_Team::Draw()
{
    if( !bShouldDrawKnowledges_ )
        return;

    for( CIT_ObjectKnowledgeMap itKnowledge = objectKnowledges_.begin(); itKnowledge != objectKnowledges_.end(); ++itKnowledge )
        itKnowledge->second->Draw();
}


