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
// $Archive: /MVW_v10/Build/SDK/Light2/src/ParamListView.cpp $
// $Author: Ape $
// $Modtime: 23/09/04 17:43 $
// $Revision: 4 $
// $Workfile: ParamListView.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "ParamListView.h"
#include "moc_ParamListView.cpp"

// -----------------------------------------------------------------------------
// Name: ParamListView constructor
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
ParamListView::ParamListView( const std::string& strLabel, bool bPopupMenu, QWidget* pParent )
    : QListView     ( pParent )
    , pPopupMenu_   ( new QPopupMenu( this ) )
{
    this->addColumn( strLabel.c_str() );
    this->setResizeMode( QListView::LastColumn );

    if( bPopupMenu )
        connect( this, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnRequestPopup( QListViewItem*, const QPoint& ) ) );

}


// -----------------------------------------------------------------------------
// Name: ParamListView destructor
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
ParamListView::~ParamListView()
{
}


// -----------------------------------------------------------------------------
// Name: ParamListView::TurnHeaderRed
/** @param  nMSec 
*/
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
void ParamListView::TurnHeaderRed( int nMSec )
{
    this->header()->setPaletteForegroundColor( Qt::red );
    QTimer::singleShot( nMSec, this, SLOT( TurnHeaderBlack() ) );
}


// -----------------------------------------------------------------------------
// Name: ParamListView::OnRequestPopup
/** @param  pItem 
    @param  pos 
*/
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
void ParamListView::OnRequestPopup( QListViewItem* pItem, const QPoint& pos )
{
    pPopupMenu_->clear();
    if( pItem != 0 )
        pPopupMenu_->insertItem( tr( "Effacer" ), this, SLOT( OnDeleteSelectedItem() ) );
    pPopupMenu_->insertItem( tr( "Effacer la liste" ), this, SLOT( OnClearList() ) );
    pPopupMenu_->popup( pos );
}


// -----------------------------------------------------------------------------
// Name: ParamListView::OnDeleteSelectedItem
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
void ParamListView::OnDeleteSelectedItem()
{
    assert( this->currentItem() != 0 );
    delete this->currentItem();
}


// -----------------------------------------------------------------------------
// Name: ParamListView::OnClearList
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
void ParamListView::OnClearList()
{
    this->clear();
}


// -----------------------------------------------------------------------------
// Name: ParamListView::TurnHeaderBlack
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
void ParamListView::TurnHeaderBlack()
{
    this->header()->setPaletteForegroundColor( Qt::black );
}

