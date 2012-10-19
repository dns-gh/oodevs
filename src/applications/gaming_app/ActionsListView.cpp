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
    QStringList headers;
    headers << tools::translate( "Parameter", "Parameter" ) << tools::translate( "Parameter", "Value" );
    setHeaderLabels( headers );
    setRootIsDecorated( false );
    header()->setResizeMode( 0, QHeaderView::ResizeToContents );
    connect( this, SIGNAL( itemClicked( QTreeWidgetItem*, int ) ), SLOT( OnItemClicked( QTreeWidgetItem*, int ) ) );
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
    QString link = param.GetLink( extractor_ );
    if( !link.isEmpty())
    {
        QFont font = item->font( 1 );
        font.setUnderline( true );
        item->setFont( 1, font );
        item->setTextColor( 1, Qt::blue );
        item->setData( 1, Qt::UserRole + 1, link );
    }
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
// Name: ActionsListView::OnItemClicked
// Created: JSR 2012-10-18
// -----------------------------------------------------------------------------
void ActionsListView::OnItemClicked( QTreeWidgetItem *item, int column )
{
    if( column == 1 && item->data( 1, Qt::UserRole + 1 ).isValid() )
        extractor_.NotifyLinkClicked( item->data( 1, Qt::UserRole + 1 ).toString() );
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
