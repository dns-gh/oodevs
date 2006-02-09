// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-05-11 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ParamObjectKnowledgeList.cpp $
// $Author: Ape $
// $Modtime: 23/09/04 17:43 $
// $Revision: 7 $
// $Workfile: ParamObjectKnowledgeList.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "ParamObjectKnowledgeList.h"
#include "moc_ParamObjectKnowledgeList.cpp"

#include "App.h"
#include "Object_ABC.h"
#include "ActionContext.h"
#include "ObjectKnowledge.h"
#include "Agent.h"
#include "Agent_ABC.h"
#include "Team.h"

// -----------------------------------------------------------------------------
// Name: ParamObjectKnowledgeList constructor
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
ParamObjectKnowledgeList::ParamObjectKnowledgeList( ASN1T_ListKnowledgeObject& asnListKnowledge, Agent_ABC& agent, const std::string strLabel, const std::string strMenuText, int nMinItems, int nMaxItems, QWidget* pParent, bool bOptional )
    : ParamListView( strLabel, true, pParent )
    , Param_ABC     ( bOptional )
    , strMenuText_      ( strMenuText )
    , asnListKnowledge_ ( asnListKnowledge )
    , agent_            ( agent )
    , pAsnOIDList_      ( 0 )
    , nMinItems_        ( nMinItems )
    , nMaxItems_        ( nMaxItems )
    , pPopupKnowledge_  ( 0 )
{
    connect( &App::GetApp(), SIGNAL( ObjectKnowledgeDeleted( Team&, ObjectKnowledge& ) ), this, SLOT( OnObjectKnowledgeDeleted( Team&, ObjectKnowledge& ) ) );
}


// -----------------------------------------------------------------------------
// Name: ParamObjectKnowledgeList destructor
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
ParamObjectKnowledgeList::~ParamObjectKnowledgeList()
{
    delete[] pAsnOIDList_;
}


// -----------------------------------------------------------------------------
// Name: ParamObjectKnowledgeList::FillRemotePopupMenu
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
void ParamObjectKnowledgeList::FillRemotePopupMenu( QPopupMenu& popupMenu, const ActionContext& context )
{
    if( context.selectedElement_.pObjectKnowledge_ == 0 && context.selectedElement_.pObject_ == 0 )
        return;

    QListViewItem* pItem = this->firstChild();
    while( pItem != 0 )
    {
        MT_ValuedListViewItem< ObjectKnowledge* >* pCastItem = (MT_ValuedListViewItem< ObjectKnowledge* >*)pItem;
        if(    ( context.selectedElement_.pObjectKnowledge_!= 0 && pCastItem->GetValue() == context.selectedElement_.pObjectKnowledge_ )
            || ( context.selectedElement_.pObject_         != 0 && pCastItem->GetValue()->GetRealObject() == context.selectedElement_.pObject_ ) )
            return;

        pItem = pItem->nextSibling();
    }

    if( context.selectedElement_.pObject_ != 0 )
    {
        pPopupKnowledge_ = dynamic_cast<Agent*>(&agent_)->GetTeam().FindKnowledgeOnObject( *(context.selectedElement_.pObject_) );
        if( pPopupKnowledge_ == 0 )
            return;
    }
    else
        pPopupKnowledge_ = context.selectedElement_.pObjectKnowledge_;

    popupMenu.insertItem( strMenuText_.c_str(), this, SLOT( AcceptPopupMenuKnowledge() ) );

}


// -----------------------------------------------------------------------------
// Name: ParamObjectKnowledgeList::CheckValidity
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
bool ParamObjectKnowledgeList::CheckValidity()
{
    if( this->childCount() >= nMinItems_ && this->childCount() <= nMaxItems_ )
        return true;

    this->TurnHeaderRed( 3000 );
    return false;
}


// -----------------------------------------------------------------------------
// Name: ParamObjectKnowledgeList::WriteMsg
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
void ParamObjectKnowledgeList::WriteMsg( std::stringstream& strMsg )
{
    strMsg << this->header()->label(0) << ": " << this->childCount() << " object knowledges. [";

    asnListKnowledge_.n = this->childCount();

    assert( !( this->childCount() == 0 && !IsOptional() ) );
    if( this->childCount() == 0 && IsOptional() )
        return;

    delete[] pAsnOIDList_;
    pAsnOIDList_ = new ASN1T_OID[ this->childCount() ];
    asnListKnowledge_.elem = pAsnOIDList_;

    int i = 0;
    QListViewItem* pItem = this->firstChild();
    while( pItem != 0 )
    {
        MT_ValuedListViewItem< ObjectKnowledge* >* pCastItem = (MT_ValuedListViewItem< ObjectKnowledge* >*)pItem;
        asnListKnowledge_.elem[i] = pCastItem->GetValue()->GetID();
        strMsg << "#" << pCastItem->GetValue()->GetID() << (pItem->nextSibling() != 0 ? ", " : "");
        pItem = pItem->nextSibling();
        ++i;
    }
    strMsg << "]";
}


// -----------------------------------------------------------------------------
// Name: ParamObjectKnowledgeList::AcceptPopupMenuKnowledge
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
void ParamObjectKnowledgeList::AcceptPopupMenuKnowledge()
{
    // The knowledge could have been deleted between the popup's appearance
    // and validation.
    if( pPopupKnowledge_ == 0 )
        return;

    new MT_ValuedListViewItem< ObjectKnowledge* >( pPopupKnowledge_, this, QString::number( pPopupKnowledge_->GetID() ) );
}


// -----------------------------------------------------------------------------
// Name: ParamObjectKnowledgeList::OnObjectKnowledgeDeleted
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
void ParamObjectKnowledgeList::OnObjectKnowledgeDeleted( Team& /*team*/, ObjectKnowledge& knowledge )
{
    // Hide the list's popupmenu to avoid erasing an item that's already gone.
    pPopupMenu_->hide();

    if( pPopupKnowledge_ == &knowledge )
        pPopupKnowledge_ = 0;

    QListViewItem* pItem = this->firstChild();
    while( pItem != 0 )
    {
        MT_ValuedListViewItem< ObjectKnowledge* >* pCastItem = (MT_ValuedListViewItem< ObjectKnowledge* >*)pItem;
        if( pCastItem->GetValue() == &knowledge )
        {
            delete pItem;
            return;
        }
        pItem = pItem->nextSibling();
    }
}
