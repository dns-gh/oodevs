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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ParamListView.cpp $
// $Author: Ape $
// $Modtime: 23/09/04 17:43 $
// $Revision: 4 $
// $Workfile: MOS_ParamListView.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_Pch.h"
#include "MOS_ParamListView.h"
#include "moc_MOS_ParamListView.cpp"

#ifndef MOS_USE_INLINE
#   include "MOS_ParamListView.inl"
#endif


// -----------------------------------------------------------------------------
// Name: MOS_ParamListView constructor
/** @param  bRequired 
    @param  bPopupMenu 
    @param  pParent 
*/
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
MOS_ParamListView::MOS_ParamListView( const std::string& strLabel, bool bPopupMenu, QWidget* pParent )
    : QListView     ( pParent )
    , pPopupMenu_   ( new QPopupMenu( this ) )
{
    this->addColumn( strLabel.c_str() );
    this->setResizeMode( QListView::LastColumn );

    if( bPopupMenu )
        connect( this, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnRequestPopup( QListViewItem*, const QPoint& ) ) );

}


// -----------------------------------------------------------------------------
// Name: MOS_ParamListView destructor
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
MOS_ParamListView::~MOS_ParamListView()
{
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamListView::TurnHeaderRed
/** @param  nMSec 
*/
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
void MOS_ParamListView::TurnHeaderRed( int nMSec )
{
    this->header()->setPaletteForegroundColor( Qt::red );
    QTimer::singleShot( nMSec, this, SLOT( TurnHeaderBlack() ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamListView::OnRequestPopup
/** @param  pItem 
    @param  pos 
*/
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
void MOS_ParamListView::OnRequestPopup( QListViewItem* pItem, const QPoint& pos )
{
    pPopupMenu_->clear();
    if( pItem != 0 )
        pPopupMenu_->insertItem( tr( "Effacer" ), this, SLOT( OnDeleteSelectedItem() ) );
    pPopupMenu_->insertItem( tr( "Effacer la liste" ), this, SLOT( OnClearList() ) );
    pPopupMenu_->popup( pos );
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamListView::OnDeleteSelectedItem
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
void MOS_ParamListView::OnDeleteSelectedItem()
{
    assert( this->currentItem() != 0 );
    delete this->currentItem();
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamListView::OnClearList
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
void MOS_ParamListView::OnClearList()
{
    this->clear();
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamListView::TurnHeaderBlack
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
void MOS_ParamListView::TurnHeaderBlack()
{
    this->header()->setPaletteForegroundColor( Qt::black );
}

