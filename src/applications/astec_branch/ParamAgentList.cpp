// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-03-24 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ParamAgentList.cpp $
// $Author: Ape $
// $Modtime: 21/09/04 15:58 $
// $Revision: 7 $
// $Workfile: ParamAgentList.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "ParamAgentList.h"
#include "moc_ParamAgentList.cpp"

#include "Agent.h"
#include "ActionContext.h"

// -----------------------------------------------------------------------------
// Name: ParamAgentList constructor
// Created: APE 2004-03-24
// -----------------------------------------------------------------------------
ParamAgentList::ParamAgentList( ASN1T_ListAgent& asnListAgent, const std::string strLabel, const std::string strMenuText, int nMinAgents, int nMaxAgents, QWidget* pParent, bool bOptional )
    : ParamListView( strLabel, true, pParent )
    , Param_ABC ( bOptional )
    , strMenuText_  ( strMenuText )
    , asnListAgent_ ( asnListAgent )
    , pAsnOIDList_  ( 0 )
    , nMinAgents_   ( nMinAgents )
    , nMaxAgents_   ( nMaxAgents )
    , pPopupAgent_  ( 0 )
{
    asnListAgent_.elem = 0;
}


// -----------------------------------------------------------------------------
// Name: ParamAgentList destructor
// Created: APE 2004-03-24
// -----------------------------------------------------------------------------
ParamAgentList::~ParamAgentList()
{
    delete[] pAsnOIDList_;
}


// -----------------------------------------------------------------------------
// Name: ParamAgentList::FillRemotePopupMenu
// Created: APE 2004-03-24
// -----------------------------------------------------------------------------
void ParamAgentList::FillRemotePopupMenu( QPopupMenu& popupMenu, const ActionContext& context )
{
    if( context.selectedElement_.pAgent_ == 0 )
        return;

    QListViewItem* pItem = this->firstChild();
    while( pItem != 0 )
    {
        if( ((MT_ValuedListViewItem< Agent* >*)pItem)->GetValue() == context.selectedElement_.pAgent_ )
            return;
        pItem = pItem->nextSibling();
    }

    pPopupAgent_ = context.selectedElement_.pAgent_;

    popupMenu.insertItem( strMenuText_.c_str(), this, SLOT( AcceptPopupMenuAgent() ) );
}


// -----------------------------------------------------------------------------
// Name: ParamAgentList::CheckValidity
// Created: APE 2004-03-24
// -----------------------------------------------------------------------------
bool ParamAgentList::CheckValidity()
{
    if( this->childCount() >= nMinAgents_ && this->childCount() <= nMaxAgents_ )
        return true;

    this->TurnHeaderRed( 3000 );
    return false;
}


// -----------------------------------------------------------------------------
// Name: ParamAgentList::WriteMsg
// Created: APE 2004-03-24
// -----------------------------------------------------------------------------
void ParamAgentList::WriteMsg( std::stringstream& strMsg )
{
    strMsg << this->header()->label(0) << ": " << this->childCount() << " agents. [";

    asnListAgent_.n    = this->childCount();

    assert( !( this->childCount() == 0 && !IsOptional() ) );
    if( this->childCount() == 0 && IsOptional() )
        return;

    delete[] pAsnOIDList_;
    pAsnOIDList_ = new ASN1T_OID[ this->childCount() ];
    asnListAgent_.elem = pAsnOIDList_;

    int i = 0;
    QListViewItem* pItem = this->firstChild();
    while( pItem != 0 )
    {
        MT_ValuedListViewItem< Agent* >* pCastItem = (MT_ValuedListViewItem< Agent* >*)pItem;
        asnListAgent_.elem[i] = pCastItem->GetValue()->GetID();
        strMsg << "#" << pCastItem->GetValue()->GetID() << (pItem->nextSibling() != 0 ? ", " : "");
        pItem = pItem->nextSibling();
        ++i;
    }
    strMsg << "]";
}


// -----------------------------------------------------------------------------
// Name: ParamAgentList::AcceptPopupMenuAgent
// Created: APE 2004-03-24
// -----------------------------------------------------------------------------
void ParamAgentList::AcceptPopupMenuAgent()
{
    new MT_ValuedListViewItem< Agent* >( pPopupAgent_, this, pPopupAgent_->GetName().c_str() );
}

