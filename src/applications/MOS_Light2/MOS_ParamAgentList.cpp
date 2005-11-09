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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ParamAgentList.cpp $
// $Author: Ape $
// $Modtime: 21/09/04 15:58 $
// $Revision: 7 $
// $Workfile: MOS_ParamAgentList.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_pch.h"
#include "MOS_ParamAgentList.h"
#include "moc_MOS_ParamAgentList.cpp"

#include "MOS_Agent.h"
#include "MOS_ActionContext.h"

// -----------------------------------------------------------------------------
// Name: MOS_ParamAgentList constructor
// Created: APE 2004-03-24
// -----------------------------------------------------------------------------
MOS_ParamAgentList::MOS_ParamAgentList( ASN1T_ListAgent& asnListAgent, const std::string strLabel, const std::string strMenuText, int nMinAgents, int nMaxAgents, QWidget* pParent, bool bOptional )
    : MOS_ParamListView( strLabel, true, pParent )
    , MOS_Param_ABC ( bOptional )
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
// Name: MOS_ParamAgentList destructor
// Created: APE 2004-03-24
// -----------------------------------------------------------------------------
MOS_ParamAgentList::~MOS_ParamAgentList()
{
    delete[] pAsnOIDList_;
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamAgentList::FillRemotePopupMenu
// Created: APE 2004-03-24
// -----------------------------------------------------------------------------
void MOS_ParamAgentList::FillRemotePopupMenu( QPopupMenu& popupMenu, const MOS_ActionContext& context )
{
    if( context.selectedElement_.pAgent_ == 0 )
        return;

    QListViewItem* pItem = this->firstChild();
    while( pItem != 0 )
    {
        if( ((MT_ValuedListViewItem< MOS_Agent* >*)pItem)->GetValue() == context.selectedElement_.pAgent_ )
            return;
        pItem = pItem->nextSibling();
    }

    pPopupAgent_ = context.selectedElement_.pAgent_;

    popupMenu.insertItem( strMenuText_.c_str(), this, SLOT( AcceptPopupMenuAgent() ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamAgentList::CheckValidity
// Created: APE 2004-03-24
// -----------------------------------------------------------------------------
bool MOS_ParamAgentList::CheckValidity()
{
    if( this->childCount() >= nMinAgents_ && this->childCount() <= nMaxAgents_ )
        return true;

    this->TurnHeaderRed( 3000 );
    return false;
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamAgentList::WriteMsg
// Created: APE 2004-03-24
// -----------------------------------------------------------------------------
void MOS_ParamAgentList::WriteMsg( std::stringstream& strMsg )
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
        MT_ValuedListViewItem< MOS_Agent* >* pCastItem = (MT_ValuedListViewItem< MOS_Agent* >*)pItem;
        asnListAgent_.elem[i] = pCastItem->GetValue()->GetID();
        strMsg << "#" << pCastItem->GetValue()->GetID() << (pItem->nextSibling() != 0 ? ", " : "");
        pItem = pItem->nextSibling();
        ++i;
    }
    strMsg << "]";
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamAgentList::AcceptPopupMenuAgent
// Created: APE 2004-03-24
// -----------------------------------------------------------------------------
void MOS_ParamAgentList::AcceptPopupMenuAgent()
{
    new MT_ValuedListViewItem< MOS_Agent* >( pPopupAgent_, this, pPopupAgent_->GetName().c_str() );
}

