// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "AfterActionRequestList.h"
#include "moc_AfterActionRequestList.cpp"
#include "clients_kernel/Controllers.h"
#include "clients_gui/DragAndDropHelpers.h"
#include "clients_gui/ItemFactory_ABC.h"
#include "clients_gui/ListItemToolTip.h"
#include "gaming/IndicatorRequest.h"
#include "gaming/Simulation.h"
#include "IndicatorPlotFactory.h"
#include "icons.h"

#pragma warning( disable : 4355 )

Q_DECLARE_METATYPE( const IndicatorRequest* )

using namespace kernel;
using namespace gui;

namespace
{
    class MyList : public QTreeWidget
    {
    public:
        MyList( AfterActionRequestList* parent )
            : QTreeWidget( parent )
        {
            setColumnCount( 2 );
            setDragEnabled( true );
            setRootIsDecorated( false );
            setAllColumnsShowFocus( true );
            header()->setResizeMode( 1, QHeaderView::ResizeToContents );
            setContextMenuPolicy( Qt::CustomContextMenu );
        }
        virtual void startDrag( Qt::DropActions /*supportedActions*/ )
        {
            const QModelIndex index = selectionModel()->currentIndex();
            if( index.isValid() )
                if( const IndicatorRequest* request = index.model()->data( index.model()->index( index.row(), 0, index.parent() ), Qt::UserRole ).value< const IndicatorRequest* >() )
                    dnd::CreateDragObject( request, this );
        }
    };
}

// -----------------------------------------------------------------------------
// Name: AfterActionRequestList constructor
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
AfterActionRequestList::AfterActionRequestList( QWidget* parent, Controllers& controllers, IndicatorPlotFactory& plotFactory )
    : Q3VBox( parent, "AfterActionRequestList" )
    , controllers_  ( controllers )
    , plotFactory_  ( plotFactory )
    , pendingPixmap_( MAKE_PIXMAP( aaa_pending ) )
    , donePixmap_   ( MAKE_PIXMAP( aaa_valid ) )
    , failedPixmap_ ( MAKE_PIXMAP( aaa_broken ) )
    , popupMenu_( new kernel::ContextMenu( this ) )
{
    requests_ = new MyList( this );
    QStringList headers;
    headers << tr( "Request" ) << tr( "Status" );
    requests_->setHeaderLabels( headers );
    connect( requests_, SIGNAL( customContextMenuRequested( const QPoint& ) ), SLOT( OnRequestPopup( const QPoint& ) ) );
    connect( requests_, SIGNAL( itemDoubleClicked( QTreeWidgetItem*, int ) ), SLOT( OnDoubleClicked() ) );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: AfterActionRequestList destructor
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
AfterActionRequestList::~AfterActionRequestList()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: AfterActionRequestList::OnDoubleClicked
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
void AfterActionRequestList::OnDoubleClicked()
{
    const QModelIndex index = requests_->currentIndex();
    if( index.isValid() )
    {
        if( const IndicatorRequest* request = index.model()->data( index.model()->index( index.row(), 0, index.parent() ), Qt::UserRole ).value< const IndicatorRequest* >() )
        {
            if( request->IsDone() )
                plotFactory_.CreatePlot( *request );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: AfterActionRequestList::OnRequestPopup
// Created: ABR 2011-02-17
// -----------------------------------------------------------------------------
void AfterActionRequestList::OnRequestPopup( const QPoint& pos )
{
    popupMenu_->clear();
    const QModelIndex index = requests_->currentIndex();
    if( index.isValid() )
    {
        popupMenu_->insertItem( tr( "Delete request" ), this, SLOT( OnRemoveItem() ) );
        popupMenu_->insertSeparator();
    }
    popupMenu_->insertItem( tr( "Clear list" ), this, SLOT( ClearList() ) );
    popupMenu_->popup( requests_->viewport()->mapToGlobal( pos ) );
}

// -----------------------------------------------------------------------------
// Name: AfterActionRequestList::ClearList
// Created: JSR 2012-10-26
// -----------------------------------------------------------------------------
void AfterActionRequestList::ClearList()
{
    while( requests_->topLevelItemCount() > 0 )
        delete requests_->takeTopLevelItem( 0 );
}

// -----------------------------------------------------------------------------
// Name: AfterActionRequestList::OnRemoveItem
// Created: ABR 2011-02-17
// -----------------------------------------------------------------------------
void AfterActionRequestList::OnRemoveItem()
{
    const QModelIndex index = requests_->currentIndex();
    if( index.isValid() )
        delete requests_->takeTopLevelItem( index.row() );
}

// -----------------------------------------------------------------------------
// Name: AfterActionRequestList::NotifyCreated
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
void AfterActionRequestList::NotifyCreated( const IndicatorRequest& request )
{
    QTreeWidgetItem* item = new QTreeWidgetItem();
    requests_->addTopLevelItem( item );
    Display( request, item );
}

// -----------------------------------------------------------------------------
// Name: AfterActionRequestList::NotifyUpdated
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
void AfterActionRequestList::NotifyUpdated( const IndicatorRequest& request )
{
    for( int i = 0; i < requests_->topLevelItemCount(); ++i )
    {
        QTreeWidgetItem* item = requests_->topLevelItem( i );
        if( item->data( 0, Qt::UserRole ).value< const IndicatorRequest* >() == &request )
        {
            Display( request, item );
            break;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: AfterActionRequestList::NotifyUpdated
// Created: JSR 2010-06-02
// -----------------------------------------------------------------------------
void AfterActionRequestList::NotifyUpdated( const Simulation& simulation )
{
    if( !simulation.IsConnected() )
        ClearList();
}

// -----------------------------------------------------------------------------
// Name: AfterActionRequestList::Display
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
void AfterActionRequestList::Display( const IndicatorRequest& request, QTreeWidgetItem* item )
{
    item->setText( 0, request.GetName() );
    item->setData( 0, Qt::UserRole, QVariant::fromValue( &request ) );
    item->setIcon( 1, request.IsPending() ? pendingPixmap_ :
                        request.IsDone() ? donePixmap_ :
                        request.IsFailed() ? failedPixmap_ :
                        QPixmap() );
    item->setToolTip( 0, request.ErrorMessage() );
    item->setToolTip( 1, request.ErrorMessage() );
    if( request.IsDone() )
        item->setFlags( item->flags() | Qt::ItemIsDragEnabled );
    else
        item->setFlags( item->flags() ^ Qt::ItemIsDragEnabled );
}
