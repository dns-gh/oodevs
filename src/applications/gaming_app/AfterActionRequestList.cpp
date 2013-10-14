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
#include "IndicatorPlotFactory.h"
#include "clients_gui/DragAndDropHelpers.h"
#include "clients_kernel/ContextMenu.h"
#include "clients_kernel/Controllers.h"
#include "gaming/IndicatorRequest.h"
#include "gaming/Simulation.h"
#include "icons.h"

Q_DECLARE_METATYPE( const IndicatorRequest* )

namespace
{
    class MyList : public QTreeWidget
    {
    public:
        MyList()
        {
            setColumnCount( 3 );
            setDragEnabled( true );
            setRootIsDecorated( false );
            setAllColumnsShowFocus( true );
            header()->setResizeMode( 0, QHeaderView::ResizeToContents );
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
AfterActionRequestList::AfterActionRequestList( QWidget* parent, kernel::Controllers& controllers, IndicatorPlotFactory& plotFactory )
    : QWidget( parent )
    , controllers_( controllers )
    , plotFactory_( plotFactory )
    , pendingPixmap_( MAKE_PIXMAP( aaa_pending ) )
    , donePixmap_( MAKE_PIXMAP( aaa_valid ) )
    , failedPixmap_( MAKE_PIXMAP( aaa_broken ) )
{
    setLayout( new QVBoxLayout );
    requests_ = new MyList();
    QStringList headers;
    headers << " " << tr( "Request" ) << tr( "Function" );
    requests_->setHeaderLabels( headers );
    connect( requests_, SIGNAL( customContextMenuRequested( const QPoint& ) ), SLOT( OnRequestPopup( const QPoint& ) ) );
    connect( requests_, SIGNAL( itemDoubleClicked( QTreeWidgetItem*, int ) ), SLOT( OnDoubleClicked() ) );
    layout()->addWidget( requests_ );
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
        const IndicatorRequest* request = index.model()->data( index.model()->index( index.row(), 0, index.parent() ), Qt::UserRole ).value< const IndicatorRequest* >();
        if( request && request->IsDone() )
            plotFactory_.CreatePlot( *request );
    }
}

// -----------------------------------------------------------------------------
// Name: AfterActionRequestList::OnRequestPopup
// Created: ABR 2011-02-17
// -----------------------------------------------------------------------------
void AfterActionRequestList::OnRequestPopup( const QPoint& pos )
{
    kernel::ContextMenu* menu = new kernel::ContextMenu( this );
    connect( menu, SIGNAL( aboutToHide() ), menu, SLOT( deleteLater() ) );
    const QModelIndex index = requests_->currentIndex();
    if( index.isValid() )
    {
        menu->insertItem( tr( "Delete request" ), this, SLOT( OnRemoveItem() ) );
        menu->insertSeparator();
    }
    menu->insertItem( tr( "Clear list" ), this, SLOT( ClearList() ) );
    menu->popup( requests_->viewport()->mapToGlobal( pos ) );
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
        if( item && item->data( 0, Qt::UserRole ).value< const IndicatorRequest* >() == &request )
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
    item->setIcon( 0, request.IsPending() ? pendingPixmap_ :
        request.IsDone() ? donePixmap_ :
        request.IsFailed() ? failedPixmap_ :
        QPixmap() );
    item->setData( 0, Qt::UserRole, QVariant::fromValue( &request ) );
    item->setText( 1, request.GetDisplayName() );
    item->setText( 2, request.GetName() );
    item->setToolTip( 0, request.ErrorMessage() );
    item->setToolTip( 1, request.ErrorMessage() );
    item->setToolTip( 2, request.ErrorMessage() );
    if( request.IsDone() )
        item->setFlags( item->flags() | Qt::ItemIsDragEnabled );
    else
        item->setFlags( item->flags() ^ Qt::ItemIsDragEnabled );
}
