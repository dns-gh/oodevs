// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-05-10 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ParamAgentKnowledgeList.cpp $
// $Author: Ape $
// $Modtime: 23/09/04 17:43 $
// $Revision: 6 $
// $Workfile: ParamAgentKnowledgeList.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "ParamAgentKnowledgeList.h"
#include "moc_ParamAgentKnowledgeList.cpp"

#include "App.h"
#include "Agent.h"
#include "Agent_ABC.h"
#include "Gtia.h"
#include "ActionContext.h"
#include "AgentKnowledge.h"
#include "OptionalParamFunctor_ABC.h"

// -----------------------------------------------------------------------------
// Name: ParamAgentKnowledgeList constructor
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
ParamAgentKnowledgeList::ParamAgentKnowledgeList( ASN1T_ListKnowledgeAgent& asnListKnowledge, Agent_ABC& agent, const std::string strLabel, const std::string strMenuText, int nMinItems, int nMaxItems, QWidget* pParent, OptionalParamFunctor_ABC* pOptional )
    : ParamListView( strLabel, true, pParent )
    , Param_ABC     ( pOptional )
    , strMenuText_      ( strMenuText )
    , asnListKnowledge_ ( asnListKnowledge )
    , pAsnOIDList_      ( 0 )
    , agent_            ( agent )
    , nMinItems_        ( nMinItems )
    , nMaxItems_        ( nMaxItems )
    , pPopupKnowledge_  ( 0 )
{
    connect( &App::GetApp(), SIGNAL( AgentKnowledgeDeleted( Gtia&, AgentKnowledge& ) ), this, SLOT( OnAgentKnowledgeDeleted( Gtia&, AgentKnowledge& ) ) );
}


// -----------------------------------------------------------------------------
// Name: ParamAgentKnowledgeList destructor
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
ParamAgentKnowledgeList::~ParamAgentKnowledgeList()
{
    delete[] pAsnOIDList_;
}


// -----------------------------------------------------------------------------
// Name: ParamAgentKnowledgeList::FillRemotePopupMenu
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
void ParamAgentKnowledgeList::FillRemotePopupMenu( QPopupMenu& popupMenu, const ActionContext& context )
{
    if(    context.selectedElement_.pAgent_ == 0
        && context.selectedElement_.pAgentKnowledge_ == 0 )
        return;

    QListViewItem* pItem = this->firstChild();
    while( pItem != 0 )
    {
        MT_ValuedListViewItem< AgentKnowledge* >* pCastItem = (MT_ValuedListViewItem< AgentKnowledge* >*)pItem;
        if(    ( context.selectedElement_.pAgentKnowledge_ != 0 && pCastItem->GetValue() == context.selectedElement_.pAgentKnowledge_ )
            || ( context.selectedElement_.pAgent_          != 0 && &(pCastItem->GetValue()->GetRealAgent()) == context.selectedElement_.pAgent_ ) )
            return;
        pItem = pItem->nextSibling();
    }

    if( context.selectedElement_.pAgent_ != 0 )
    {
        pPopupKnowledge_ = dynamic_cast<Agent*>(&agent_)->GetGtia().FindKnowledgeOnAgent( *(context.selectedElement_.pAgent_) );
        if( pPopupKnowledge_ == 0 )
            return;
    }
    else
        pPopupKnowledge_ = context.selectedElement_.pAgentKnowledge_;

    popupMenu.insertItem( strMenuText_.c_str(), this, SLOT( AcceptPopupMenuKnowledge() ) );
}


// -----------------------------------------------------------------------------
// Name: ParamAgentKnowledgeList::CheckValidity
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
bool ParamAgentKnowledgeList::CheckValidity()
{
    if( this->childCount() >= nMinItems_ && this->childCount() <= nMaxItems_ )
        return true;

    if( IsOptional() )
        return true;

    this->TurnHeaderRed( 3000 );
    return false;
}


// -----------------------------------------------------------------------------
// Name: ParamAgentKnowledgeList::WriteMsg
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
void ParamAgentKnowledgeList::WriteMsg( std::stringstream& strMsg )
{
    strMsg << this->header()->label(0).latin1() << ": " << this->childCount() << " agent knowledges. [";

    asnListKnowledge_.n = this->childCount();

    if( this->childCount() == 0 )
        return;

    if( pOptional_ )
        pOptional_->SetOptionalPresent();

    delete[] pAsnOIDList_;
    pAsnOIDList_ = new ASN1T_OID[ this->childCount() ];
    asnListKnowledge_.elem = pAsnOIDList_;

    int i = 0;
    QListViewItem* pItem = this->firstChild();
    while( pItem != 0 )
    {
        MT_ValuedListViewItem< AgentKnowledge* >* pCastItem = (MT_ValuedListViewItem< AgentKnowledge* >*)pItem;
        asnListKnowledge_.elem[i] = pCastItem->GetValue()->GetID();
        strMsg << "#" << pCastItem->GetValue()->GetID() << (pItem->nextSibling() != 0 ? ", " : "");
        pItem = pItem->nextSibling();
        ++i;
    }
    strMsg << "]";
}


// -----------------------------------------------------------------------------
// Name: ParamAgentKnowledgeList::AcceptPopupMenuKnowledge
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
void ParamAgentKnowledgeList::AcceptPopupMenuKnowledge()
{
    // The knowledge could have been deleted between the popup's appearance
    // and validation.
    if( pPopupKnowledge_ == 0 )
        return;

    new MT_ValuedListViewItem< AgentKnowledge* >( pPopupKnowledge_, this, pPopupKnowledge_->GetRealAgent().GetName().c_str() );
}


// -----------------------------------------------------------------------------
// Name: ParamAgentKnowledgeList::OnAgentKnowledgeDeleted
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
void ParamAgentKnowledgeList::OnAgentKnowledgeDeleted( Gtia& /*gtia*/, AgentKnowledge& knowledge )
{
    // Hide the list's popupmenu to avoid erasing an item that's already gone.
    pPopupMenu_->hide();

    if( pPopupKnowledge_ == &knowledge )
        pPopupKnowledge_ = 0;

    QListViewItem* pItem = this->firstChild();
    while( pItem != 0 )
    {
        MT_ValuedListViewItem< AgentKnowledge* >* pCastItem = (MT_ValuedListViewItem< AgentKnowledge* >*)pItem;
        if( pCastItem->GetValue() == &knowledge )
        {
            delete pItem;
            return;
        }
        pItem = pItem->nextSibling();
    }
}
