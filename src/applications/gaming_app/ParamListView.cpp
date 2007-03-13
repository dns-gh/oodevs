// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamListView.h"
#include "moc_ParamListView.cpp"

// -----------------------------------------------------------------------------
// Name: ParamListView constructor
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
ParamListView::ParamListView( QWidget* parent, const QString& label )
    : QObject    ( parent )
    , list_      ( new QListView( parent ) ) 
    , pPopupMenu_( new QPopupMenu( list_ ) )
{
    list_->addColumn( label );
    list_->setResizeMode( QListView::LastColumn );
    connect( list_, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int ) ), SLOT( OnRequestPopup( QListViewItem*, const QPoint& ) ) );
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
    list_->header()->setPaletteForegroundColor( Qt::red );
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
        pPopupMenu_->insertItem( tr( "Remove" ), this, SLOT( OnDeleteSelectedItem() ) );
    pPopupMenu_->insertItem( tr( "Clear list" ), this, SLOT( OnClearList() ) );
    pPopupMenu_->popup( pos );
}

// -----------------------------------------------------------------------------
// Name: ParamListView::OnDeleteSelectedItem
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
void ParamListView::OnDeleteSelectedItem()
{
    delete list_->selectedItem();
}

// -----------------------------------------------------------------------------
// Name: ParamListView::OnClearList
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
void ParamListView::OnClearList()
{
    list_->clear();
}

// -----------------------------------------------------------------------------
// Name: ParamListView::TurnHeaderBlack
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
void ParamListView::TurnHeaderBlack()
{
    list_->header()->setPaletteForegroundColor( Qt::black );
}

// -----------------------------------------------------------------------------
// Name: ParamListView::ListView
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
QListView* ParamListView::ListView()
{
    return list_;
}
