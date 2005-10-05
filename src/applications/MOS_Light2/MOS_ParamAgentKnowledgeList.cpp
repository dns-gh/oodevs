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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ParamAgentKnowledgeList.cpp $
// $Author: Ape $
// $Modtime: 23/09/04 17:43 $
// $Revision: 6 $
// $Workfile: MOS_ParamAgentKnowledgeList.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_pch.h"
#include "MOS_ParamAgentKnowledgeList.h"
#include "moc_MOS_ParamAgentKnowledgeList.cpp"

#include "MOS_App.h"
#include "MOS_Agent.h"
#include "MOS_Agent_ABC.h"
#include "MOS_Gtia.h"
#include "MOS_ActionContext.h"
#include "MOS_AgentKnowledge.h"

// -----------------------------------------------------------------------------
// Name: MOS_ParamAgentKnowledgeList constructor
/** @param  asnListKnowledge 
    @param  strLabel 
    @param  strMenuText 
    @param  nMinItems 
    @param  nMaxItems 
    @param  pParent 
*/
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
MOS_ParamAgentKnowledgeList::MOS_ParamAgentKnowledgeList( ASN1T_ListKnowledgeAgent& asnListKnowledge, MOS_Agent_ABC& agent, const std::string strLabel, const std::string strMenuText, int nMinItems, int nMaxItems, QWidget* pParent )
    : MOS_ParamListView( strLabel, true, pParent )
    , MOS_Param_ABC     ()
    , strMenuText_      ( strMenuText )
    , asnListKnowledge_ ( asnListKnowledge )
    , pAsnOIDList_      ( 0 )
    , agent_            ( agent )
    , nMinItems_        ( nMinItems )
    , nMaxItems_        ( nMaxItems )
    , pPopupKnowledge_  ( 0 )
{
    connect( &MOS_App::GetApp(), SIGNAL( AgentKnowledgeDeleted( MOS_Gtia&, MOS_AgentKnowledge& ) ), this, SLOT( OnAgentKnowledgeDeleted( MOS_Gtia&, MOS_AgentKnowledge& ) ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamAgentKnowledgeList destructor
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
MOS_ParamAgentKnowledgeList::~MOS_ParamAgentKnowledgeList()
{
    delete[] pAsnOIDList_;
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamAgentKnowledgeList::FillRemotePopupMenu
/** @param  popupMenu 
    @param  context 
*/
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
void MOS_ParamAgentKnowledgeList::FillRemotePopupMenu( QPopupMenu& popupMenu, const MOS_ActionContext& context )
{
    if(    context.selectedElement_.pAgent_ == 0
        && context.selectedElement_.pAgentKnowledge_ == 0 )
        return;

    QListViewItem* pItem = this->firstChild();
    while( pItem != 0 )
    {
        MT_ValuedListViewItem< MOS_AgentKnowledge* >* pCastItem = (MT_ValuedListViewItem< MOS_AgentKnowledge* >*)pItem;
        if(    ( context.selectedElement_.pAgentKnowledge_ != 0 && pCastItem->GetValue() == context.selectedElement_.pAgentKnowledge_ )
            || ( context.selectedElement_.pAgent_          != 0 && &(pCastItem->GetValue()->GetRealAgent()) == context.selectedElement_.pAgent_ ) )
            return;
        pItem = pItem->nextSibling();
    }

    if( context.selectedElement_.pAgent_ != 0 )
    {
        pPopupKnowledge_ = dynamic_cast<MOS_Agent*>(&agent_)->GetGtia().FindKnowledgeOnAgent( *(context.selectedElement_.pAgent_) );
        if( pPopupKnowledge_ == 0 )
            return;
    }
    else
        pPopupKnowledge_ = context.selectedElement_.pAgentKnowledge_;

    popupMenu.insertItem( strMenuText_.c_str(), this, SLOT( AcceptPopupMenuKnowledge() ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamAgentKnowledgeList::CheckValidity
/** @return 
*/
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
bool MOS_ParamAgentKnowledgeList::CheckValidity()
{
    if( this->childCount() >= nMinItems_ && this->childCount() <= nMaxItems_ )
        return true;

    this->TurnHeaderRed( 3000 );
    return false;
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamAgentKnowledgeList::WriteMsg
/** @param  strMsg 
*/
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
void MOS_ParamAgentKnowledgeList::WriteMsg( std::stringstream& strMsg )
{
    strMsg << this->header()->label(0).latin1() << ": " << this->childCount() << " agent knowledges. [";

    asnListKnowledge_.n = this->childCount();

    delete[] pAsnOIDList_;
    pAsnOIDList_ = new ASN1T_OID[ this->childCount() ];
    asnListKnowledge_.elem = pAsnOIDList_;

    int i = 0;
    QListViewItem* pItem = this->firstChild();
    while( pItem != 0 )
    {
        MT_ValuedListViewItem< MOS_AgentKnowledge* >* pCastItem = (MT_ValuedListViewItem< MOS_AgentKnowledge* >*)pItem;
        asnListKnowledge_.elem[i] = pCastItem->GetValue()->GetID();
        strMsg << "#" << pCastItem->GetValue()->GetID() << (pItem->nextSibling() != 0 ? ", " : "");
        pItem = pItem->nextSibling();
        ++i;
    }
    strMsg << "]";
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamAgentKnowledgeList::AcceptPopupMenuKnowledge
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
void MOS_ParamAgentKnowledgeList::AcceptPopupMenuKnowledge()
{
    // The knowledge could have been deleted between the popup's appearance
    // and validation.
    if( pPopupKnowledge_ == 0 )
        return;

    new MT_ValuedListViewItem< MOS_AgentKnowledge* >( pPopupKnowledge_, this, pPopupKnowledge_->GetRealAgent().GetName().c_str() );
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamAgentKnowledgeList::OnAgentKnowledgeDeleted
/** @param  gtia 
    @param  knowledge 
*/
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
void MOS_ParamAgentKnowledgeList::OnAgentKnowledgeDeleted( MOS_Gtia& /*gtia*/, MOS_AgentKnowledge& knowledge )
{
    // Hide the list's popupmenu to avoid erasing an item that's already gone.
    pPopupMenu_->hide();

    if( pPopupKnowledge_ == &knowledge )
        pPopupKnowledge_ = 0;

    QListViewItem* pItem = this->firstChild();
    while( pItem != 0 )
    {
        MT_ValuedListViewItem< MOS_AgentKnowledge* >* pCastItem = (MT_ValuedListViewItem< MOS_AgentKnowledge* >*)pItem;
        if( pCastItem->GetValue() == &knowledge )
        {
            delete pItem;
            return;
        }
        pItem = pItem->nextSibling();
    }
}
