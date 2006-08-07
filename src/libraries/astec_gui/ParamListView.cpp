// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_gui_pch.h"
#include "ParamListView.h"
#include "moc_ParamListView.cpp"

// -----------------------------------------------------------------------------
// Name: ParamListView constructor
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
ParamListView::ParamListView( QWidget* pParent, const QString& label )
    : QListView     ( pParent )
    , pPopupMenu_   ( new QPopupMenu( this ) )
{
    addColumn( label );
    setResizeMode( QListView::LastColumn );
    connect( this, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnRequestPopup( QListViewItem*, const QPoint& ) ) );
}

// -----------------------------------------------------------------------------
// Name: ParamListView destructor
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
ParamListView::~ParamListView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamListView::TurnHeaderRed
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
bool ParamListView::Invalid()
{
    header()->setPaletteForegroundColor( Qt::red );
    QTimer::singleShot( 3000, this, SLOT( TurnHeaderBlack() ) );
    return false;
}

// -----------------------------------------------------------------------------
// Name: ParamListView::OnRequestPopup
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
    delete selectedItem();
}

// -----------------------------------------------------------------------------
// Name: ParamListView::OnClearList
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
void ParamListView::OnClearList()
{
    clear();
}

// -----------------------------------------------------------------------------
// Name: ParamListView::TurnHeaderBlack
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
void ParamListView::TurnHeaderBlack()
{
    header()->setPaletteForegroundColor( Qt::black );
}
