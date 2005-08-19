// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-04-19 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ParamLimaList.cpp $
// $Author: Ape $
// $Modtime: 21/07/04 11:34 $
// $Revision: 5 $
// $Workfile: MOS_ParamLimaList.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_pch.h"
#include "MOS_ParamLimaList.h"
#include "moc_MOS_ParamLimaList.cpp"

#include "MOS_App.h"
#include "MOS_ActionContext.h"
#include "MOS_TacticalLine_ABC.h"

#include "qheader.h"

#ifndef MOS_USE_INLINE
#   include "MOS_ParamLimaList.inl"
#endif


// -----------------------------------------------------------------------------
// Name: MOS_ParamLimaList constructor
/** @param  asnListLima 
    @param  strLabel 
    @param  strMenuText 
    @param  pParent 
*/
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
MOS_ParamLimaList::MOS_ParamLimaList( ASN1T_ListOID& asnListLima, const std::string strLabel, const std::string strMenuText, QWidget* pParent )
    : MOS_ParamListView( strLabel, true, pParent )
    , asnListLima_     ( asnListLima )
    , pAsnOIDList_     ( 0 )
    , strMenuText_     ( strMenuText )
{
    connect( &MOS_App::GetApp(), SIGNAL( TacticalLineDeleted( MOS_TacticalLine_ABC& ) ), this, SLOT( OnTacticalLineDeleted( MOS_TacticalLine_ABC& ) ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamLimaList destructor
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
MOS_ParamLimaList::~MOS_ParamLimaList()
{
    delete[] pAsnOIDList_;
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamLimaList::FillRemotePopupMenu
/** @param  popupMenu 
    @param  context 
*/
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
void MOS_ParamLimaList::FillRemotePopupMenu( QPopupMenu& popupMenu, const MOS_ActionContext& context )
{
    if( context.selectedElement_.pLine_ == 0 || context.selectedElement_.pLine_->GetLineType() != MOS_TacticalLine_ABC::eLima )
        return;

    pPopupLima_ = context.selectedElement_.pLine_;

    popupMenu.insertItem( strMenuText_.c_str(), this, SLOT( AcceptPopupMenuLima() ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamLimaList::CheckValidity
/** @return 
*/
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
bool MOS_ParamLimaList::CheckValidity()
{
    return true;
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamLimaList::WriteMsg
/** @param  sParam 
*/
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
void MOS_ParamLimaList::WriteMsg( std::stringstream& strMsg )
{
    strMsg << this->header()->label(0).latin1() << ": " << this->childCount() << " limas.";

    asnListLima_.n = this->childCount();
    if( asnListLima_.n == 0 )
        return;

    strMsg << " [";

    delete[] pAsnOIDList_;
    pAsnOIDList_ = new ASN1T_OID[ asnListLima_.n ];
    asnListLima_.elem = pAsnOIDList_;

    int i = 0;
    QListViewItem* pItem = this->firstChild();
    while( pItem != 0 )
    {
        MT_ValuedListViewItem< MOS_TacticalLine_ABC* >* pCastItem = (MT_ValuedListViewItem< MOS_TacticalLine_ABC* >*)pItem;
        asnListLima_.elem[i] = pCastItem->GetValue()->GetID();
        strMsg << "#" << pCastItem->GetValue() << (pItem->nextSibling() != 0 ? ", " : "");
        pItem = pItem->nextSibling();
        ++i;
    }

    strMsg << "]";
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamLimaList::AcceptPopupMenuLima
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
void MOS_ParamLimaList::AcceptPopupMenuLima()
{
    // The lima could have been deleted between the popup's appearance
    // and validation.
    if( pPopupLima_ == 0 )
        return;

    new MT_ValuedListViewItem< MOS_TacticalLine_ABC* >( pPopupLima_, this, pPopupLima_->GetName().c_str() );
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamLimaList::OnTacticalLineDeleted
/** @param  line 
*/
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
void MOS_ParamLimaList::OnTacticalLineDeleted( MOS_TacticalLine_ABC& line )
{
    // Hide the list's popupmenu to avoid erasing an item that's already gone.
    pPopupMenu_->hide();

    if( pPopupLima_ == &line )
        pPopupLima_ = 0;

    QListViewItem* pItem = this->firstChild();
    while( pItem != 0 )
    {
        MT_ValuedListViewItem< MOS_TacticalLine_ABC* >* pCastItem = (MT_ValuedListViewItem< MOS_TacticalLine_ABC* >*)pItem;
        if( pCastItem->GetValue() == &line )
        {
            delete pItem;
            return;
        }
        pItem = pItem->nextSibling();
    }
}
