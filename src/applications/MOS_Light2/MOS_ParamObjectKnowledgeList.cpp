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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ParamObjectKnowledgeList.cpp $
// $Author: Ape $
// $Modtime: 23/09/04 17:43 $
// $Revision: 7 $
// $Workfile: MOS_ParamObjectKnowledgeList.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_pch.h"
#include "MOS_ParamObjectKnowledgeList.h"
#include "moc_MOS_ParamObjectKnowledgeList.cpp"

#include "MOS_App.h"
#include "MOS_Object_ABC.h"
#include "MOS_ActionContext.h"
#include "MOS_ObjectKnowledge.h"
#include "MOS_Agent.h"
#include "MOS_Team.h"

#ifndef MOS_USE_INLINE
#   include "MOS_ParamObjectKnowledgeList.inl"
#endif


// -----------------------------------------------------------------------------
// Name: MOS_ParamObjectKnowledgeList constructor
/** @param  asnListKnowledge 
    @param  strLabel 
    @param  strMenuText 
    @param  nMinItems 
    @param  nMaxItems 
    @param  pParent 
*/
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
MOS_ParamObjectKnowledgeList::MOS_ParamObjectKnowledgeList( ASN1T_ListKnowledgeObject& asnListKnowledge, MOS_Agent& agent, const std::string strLabel, const std::string strMenuText, int nMinItems, int nMaxItems, QWidget* pParent )
    : MOS_ParamListView( strLabel, true, pParent )
    , MOS_Param_ABC     ()
    , strMenuText_      ( strMenuText )
    , asnListKnowledge_ ( asnListKnowledge )
    , agent_            ( agent )
    , pAsnOIDList_      ( 0 )
    , nMinItems_        ( nMinItems )
    , nMaxItems_        ( nMaxItems )
    , pPopupKnowledge_  ( 0 )
{
    connect( &MOS_App::GetApp(), SIGNAL( ObjectKnowledgeDeleted( MOS_Team&, MOS_ObjectKnowledge& ) ), this, SLOT( OnObjectKnowledgeDeleted( MOS_Team&, MOS_ObjectKnowledge& ) ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamObjectKnowledgeList destructor
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
MOS_ParamObjectKnowledgeList::~MOS_ParamObjectKnowledgeList()
{
    delete[] pAsnOIDList_;
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamObjectKnowledgeList::FillRemotePopupMenu
/** @param  popupMenu 
    @param  context 
*/
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
void MOS_ParamObjectKnowledgeList::FillRemotePopupMenu( QPopupMenu& popupMenu, const MOS_ActionContext& context )
{
    if( context.selectedElement_.pObjectKnowledge_ == 0 && context.selectedElement_.pObject_ == 0 )
        return;

    QListViewItem* pItem = this->firstChild();
    while( pItem != 0 )
    {
        MT_ValuedListViewItem< MOS_ObjectKnowledge* >* pCastItem = (MT_ValuedListViewItem< MOS_ObjectKnowledge* >*)pItem;
        if(    ( context.selectedElement_.pObjectKnowledge_!= 0 && pCastItem->GetValue() == context.selectedElement_.pObjectKnowledge_ )
            || ( context.selectedElement_.pObject_         != 0 && pCastItem->GetValue()->GetRealObject() == context.selectedElement_.pObject_ ) )
            return;

        pItem = pItem->nextSibling();
    }

    if( context.selectedElement_.pObject_ != 0 )
    {
        pPopupKnowledge_ = agent_.GetTeam().FindKnowledgeOnObject( *(context.selectedElement_.pObject_) );
        if( pPopupKnowledge_ == 0 )
            return;
    }
    else
        pPopupKnowledge_ = context.selectedElement_.pObjectKnowledge_;

    popupMenu.insertItem( strMenuText_.c_str(), this, SLOT( AcceptPopupMenuKnowledge() ) );

}


// -----------------------------------------------------------------------------
// Name: MOS_ParamObjectKnowledgeList::CheckValidity
/** @return 
*/
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
bool MOS_ParamObjectKnowledgeList::CheckValidity()
{
    if( this->childCount() >= nMinItems_ && this->childCount() <= nMaxItems_ )
        return true;

    this->TurnHeaderRed( 3000 );
    return false;
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamObjectKnowledgeList::WriteMsg
/** @param  strMsg 
*/
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
void MOS_ParamObjectKnowledgeList::WriteMsg( std::stringstream& strMsg )
{
    strMsg << this->header()->label(0) << ": " << this->childCount() << " object knowledges. [";

    asnListKnowledge_.n = this->childCount();

    delete[] pAsnOIDList_;
    pAsnOIDList_ = new ASN1T_OID[ this->childCount() ];
    asnListKnowledge_.elem = pAsnOIDList_;

    int i = 0;
    QListViewItem* pItem = this->firstChild();
    while( pItem != 0 )
    {
        MT_ValuedListViewItem< MOS_ObjectKnowledge* >* pCastItem = (MT_ValuedListViewItem< MOS_ObjectKnowledge* >*)pItem;
        asnListKnowledge_.elem[i] = pCastItem->GetValue()->GetID();
        strMsg << "#" << pCastItem->GetValue()->GetID() << (pItem->nextSibling() != 0 ? ", " : "");
        pItem = pItem->nextSibling();
        ++i;
    }
    strMsg << "]";
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamObjectKnowledgeList::AcceptPopupMenuKnowledge
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
void MOS_ParamObjectKnowledgeList::AcceptPopupMenuKnowledge()
{
    // The knowledge could have been deleted between the popup's appearance
    // and validation.
    if( pPopupKnowledge_ == 0 )
        return;

    new MT_ValuedListViewItem< MOS_ObjectKnowledge* >( pPopupKnowledge_, this, QString::number( pPopupKnowledge_->GetID() ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamObjectKnowledgeList::OnObjectKnowledgeDeleted
/** @param  gtia 
    @param  knowledge 
*/
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
void MOS_ParamObjectKnowledgeList::OnObjectKnowledgeDeleted( MOS_Team& /*team*/, MOS_ObjectKnowledge& knowledge )
{
    // Hide the list's popupmenu to avoid erasing an item that's already gone.
    pPopupMenu_->hide();

    if( pPopupKnowledge_ == &knowledge )
        pPopupKnowledge_ = 0;

    QListViewItem* pItem = this->firstChild();
    while( pItem != 0 )
    {
        MT_ValuedListViewItem< MOS_ObjectKnowledge* >* pCastItem = (MT_ValuedListViewItem< MOS_ObjectKnowledge* >*)pItem;
        if( pCastItem->GetValue() == &knowledge )
        {
            delete pItem;
            return;
        }
        pItem = pItem->nextSibling();
    }
}
