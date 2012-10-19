// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ActionsListView.h"
#include "moc_ActionsListView.cpp"
#include "icons.h"
#include "actions/Action_ABC.h"
#include "actions/Parameter_ABC.h"
#include "clients_gui/LinkItemDelegate.h"
#include "clients_gui/Tools.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/DisplayExtractor_ABC.h"


// -----------------------------------------------------------------------------
// Name: ActionsListView constructor
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
ActionsListView::ActionsListView( QWidget* parent, kernel::Controllers& controllers, kernel::DisplayExtractor_ABC& extractor )
    : QTreeWidget( parent )
    , controllers_( controllers )
    , extractor_( extractor )
    , parameter_( MAKE_PIXMAP( parameter2 ) )
{
    setColumnCount( 2 );
    setMouseTracking( true );
    gui::LinkItemDelegate* delegate = new gui::LinkItemDelegate( this );
    setItemDelegateForColumn( 1, delegate );
    setAllColumnsShowFocus( true );
    QStringList headers;
    headers << tools::translate( "Parameter", "Parameter" ) << tools::translate( "Parameter", "Value" );
    setHeaderLabels( headers );
    setRootIsDecorated( false );
    //header()->setResizeMode( 0, QHeaderView::ResizeToContents );
    connect( delegate, SIGNAL( LinkClicked( const QString&, const QModelIndex& ) ), SLOT( OnLinkClicked( const QString&, const QModelIndex& ) ) );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ActionsListView destructor
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
ActionsListView::~ActionsListView()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ActionsListView::NotifyUpdated
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
void ActionsListView::NotifyUpdated( const actions::Action_ABC& action )
{
    RecursiveDisplay( action, invisibleRootItem() );
}

// -----------------------------------------------------------------------------
// Name: ActionsListView::DisplayParameter
// Created: JSR 2012-10-17
// -----------------------------------------------------------------------------
void ActionsListView::DisplayParameter( const actions::Parameter_ABC& param, QTreeWidgetItem* item )
{
    item->setIcon( 0, parameter_ );
    item->setText( 0, param.GetName() );
    item->setText( 1, param.GetDisplayName( extractor_ ) );
    RecursiveDisplay( param, item );
}

// -----------------------------------------------------------------------------
// Name: ActionsListView::RecursiveDisplay
// Created: JSR 2012-10-19
// -----------------------------------------------------------------------------
template< typename T >
void ActionsListView::RecursiveDisplay( const T& element, QTreeWidgetItem* item )
{
    const int count = element.Count();
    while( item->childCount() > count )
        delete item->takeChild( count );
    while( item->childCount() < count )
        item->addChild( new QTreeWidgetItem );
    tools::Iterator< const actions::Parameter_ABC& > it = element.CreateIterator();
    int row = 0;
    while( it.HasMoreElements() )
        DisplayParameter( it.NextElement(), item->child( row++ ) );
}

// -----------------------------------------------------------------------------
// Name: ActionsListView::OnLinkClicked
// Created: JSR 2012-10-19
// -----------------------------------------------------------------------------
void ActionsListView::OnLinkClicked( const QString& url, const QModelIndex& index )
{
    selectionModel()->setCurrentIndex( index, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows );
    extractor_.NotifyLinkClicked( url );
}

// -----------------------------------------------------------------------------
// Name: ActionsListView::NotifySelected
// Created: SBO 2010-05-03
// -----------------------------------------------------------------------------
void ActionsListView::NotifySelected( const actions::Action_ABC* action )
{
    if( action )
        NotifyUpdated( *action );
    else
        selectionModel()->clearSelection();
}
