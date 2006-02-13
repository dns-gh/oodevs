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
// $Archive: /MVW_v10/Build/SDK/Light2/src/ParamLimaList.cpp $
// $Author: Ape $
// $Modtime: 21/07/04 11:34 $
// $Revision: 5 $
// $Workfile: ParamLimaList.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "ParamLimaList.h"
#include "moc_ParamLimaList.cpp"

#include "App.h"
#include "ActionContext.h"
#include "TacticalLine_ABC.h"

#include "qheader.h"

// -----------------------------------------------------------------------------
// Name: ParamLimaList constructor
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
ParamLimaList::ParamLimaList( ASN1T_ListOID& asnListLima, const std::string strLabel, const std::string strMenuText, QWidget* pParent, bool bOptional )
    : ParamListView( strLabel, true, pParent )
    , Param_ABC    ( bOptional )
    , asnListLima_     ( asnListLima )
    , pAsnOIDList_     ( 0 )
    , strMenuText_     ( strMenuText )
{
    connect( &App::GetApp(), SIGNAL( TacticalLineDeleted( TacticalLine_ABC& ) ), this, SLOT( OnTacticalLineDeleted( TacticalLine_ABC& ) ) );
}


// -----------------------------------------------------------------------------
// Name: ParamLimaList destructor
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
ParamLimaList::~ParamLimaList()
{
    delete[] pAsnOIDList_;
}


// -----------------------------------------------------------------------------
// Name: ParamLimaList::FillRemotePopupMenu
/** @param  popupMenu 
    @param  context 
*/
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
void ParamLimaList::FillRemotePopupMenu( QPopupMenu& popupMenu, const ActionContext& context )
{
    if( context.selectedElement_.pLine_ == 0 || context.selectedElement_.pLine_->GetLineType() != TacticalLine_ABC::eLima )
        return;

    pPopupLima_ = context.selectedElement_.pLine_;

    popupMenu.insertItem( strMenuText_.c_str(), this, SLOT( AcceptPopupMenuLima() ) );
}


// -----------------------------------------------------------------------------
// Name: ParamLimaList::CheckValidity
/** @return 
*/
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
bool ParamLimaList::CheckValidity()
{
    return true;
}


// -----------------------------------------------------------------------------
// Name: ParamLimaList::WriteMsg
/** @param  sParam 
*/
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
void ParamLimaList::WriteMsg( std::stringstream& strMsg )
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
        MT_ValuedListViewItem< TacticalLine_ABC* >* pCastItem = (MT_ValuedListViewItem< TacticalLine_ABC* >*)pItem;
        asnListLima_.elem[i] = pCastItem->GetValue()->GetID();
        strMsg << "#" << pCastItem->GetValue() << (pItem->nextSibling() != 0 ? ", " : "");
        pItem = pItem->nextSibling();
        ++i;
    }

    strMsg << "]";
}


// -----------------------------------------------------------------------------
// Name: ParamLimaList::AcceptPopupMenuLima
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
void ParamLimaList::AcceptPopupMenuLima()
{
    // The lima could have been deleted between the popup's appearance
    // and validation.
    if( pPopupLima_ == 0 )
        return;

    new MT_ValuedListViewItem< TacticalLine_ABC* >( pPopupLima_, this, pPopupLima_->GetName().c_str() );
}


// -----------------------------------------------------------------------------
// Name: ParamLimaList::OnTacticalLineDeleted
/** @param  line 
*/
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
void ParamLimaList::OnTacticalLineDeleted( TacticalLine_ABC& line )
{
    // Hide the list's popupmenu to avoid erasing an item that's already gone.
    pPopupMenu_->hide();

    if( pPopupLima_ == &line )
        pPopupLima_ = 0;

    QListViewItem* pItem = this->firstChild();
    while( pItem != 0 )
    {
        MT_ValuedListViewItem< TacticalLine_ABC* >* pCastItem = (MT_ValuedListViewItem< TacticalLine_ABC* >*)pItem;
        if( pCastItem->GetValue() == &line )
        {
            delete pItem;
            return;
        }
        pItem = pItem->nextSibling();
    }
}
