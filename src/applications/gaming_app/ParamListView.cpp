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
ParamListView::ParamListView( QObject* parent, const QString& name )
    : QObject    ( parent )
    , Param_ABC  ( name )
    , list_      ( 0 )
{
    // NOTHING
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
// Name: ParamListView::BuildInterface
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
void ParamListView::BuildInterface( QWidget* parent )
{
    list_ = new QListView( parent );
    list_->addColumn( GetName() );
    list_->setResizeMode( QListView::LastColumn );
    connect( list_, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int ) ), SLOT( OnRequestPopup( QListViewItem*, const QPoint& ) ) );
}

// -----------------------------------------------------------------------------
// Name: ParamListView::TurnHeaderRed
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
bool ParamListView::Invalid()
{
    if( list_ )
        list_->header()->setPaletteForegroundColor( Qt::red );
    QTimer::singleShot( 3000, this, SLOT( TurnHeaderBlack() ) );
    return false;
}

// -----------------------------------------------------------------------------
// Name: ParamListView::OnRequestPopup
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
void ParamListView::OnRequestPopup( QListViewItem* item, const QPoint& pos )
{
    QPopupMenu* menu = new QPopupMenu( list_ );
    if( item )
        menu->insertItem( tr( "Remove" ), this, SLOT( OnDeleteSelectedItem() ) );
    menu->insertItem( tr( "Clear list" ), this, SLOT( OnClearList() ) );
    menu->popup( pos );
}

// -----------------------------------------------------------------------------
// Name: ParamListView::OnDeleteSelectedItem
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
void ParamListView::OnDeleteSelectedItem()
{
    if( list_ )
        delete list_->selectedItem();
}

// -----------------------------------------------------------------------------
// Name: ParamListView::OnClearList
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
void ParamListView::OnClearList()
{
    if( list_ )
        list_->clear();
}

// -----------------------------------------------------------------------------
// Name: ParamListView::TurnHeaderBlack
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
void ParamListView::TurnHeaderBlack()
{
    if( list_ )
        list_->header()->setPaletteForegroundColor( Qt::black );
}

// -----------------------------------------------------------------------------
// Name: ParamListView::ListView
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
QListView* ParamListView::ListView() const
{
    return list_;
}
