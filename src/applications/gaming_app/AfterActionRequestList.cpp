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
#include "clients_gui/ItemFactory_ABC.h"
#include "clients_gui/ListItemToolTip.h"
#include "clients_gui/ValuedDragObject.h"
#include "gaming/AfterActionRequest.h"
#include "AfterActionPlot.h"
#include "icons.h"

using namespace kernel;
using namespace gui;

namespace
{
    // $$$$ AGE 2007-09-27: 
    class MyList : public ListDisplayer< AfterActionRequestList >
    {
    public:
        MyList( AfterActionRequestList* parent, ItemFactory_ABC& factory )
            : ListDisplayer< AfterActionRequestList >( parent, *parent, factory )
        {}
        virtual QDragObject* dragObject()
        {
            ValuedListItem* item = (ValuedListItem*)selectedItem();
            if( !item ) return 0;
            const AfterActionRequest* request = item->GetValue< const AfterActionRequest >();
            return new ValuedDragObject( request, this );
        }
    };
}

// -----------------------------------------------------------------------------
// Name: AfterActionRequestList constructor
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
AfterActionRequestList::AfterActionRequestList( QWidget* parent, QMainWindow* mainWindow, Controllers& controllers, ItemFactory_ABC& factory, Publisher_ABC& publisher )
    : QVBox( parent )
    , mainWindow_( mainWindow )
    , controllers_( controllers )
    , factory_( factory )
    , publisher_( publisher )
    , pendingPixmap_( MAKE_PIXMAP( aaa_pending ) )
    , donePixmap_   ( MAKE_PIXMAP( aaa_valid ) )
    , failedPixmap_ ( MAKE_PIXMAP( aaa_broken ) )
{
    requests_ = new MyList( this, factory );
    requests_->AddColumn( tr( "Request" ) );
    requests_->AddColumn( tr( "Status" ) );
    new ListItemToolTip( requests_->viewport(), *requests_ );

    connect( requests_, SIGNAL( doubleClicked( QListViewItem*, const QPoint&, int ) ), SLOT( OnDoubleClicked( QListViewItem* ) ) );

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
void AfterActionRequestList::OnDoubleClicked( QListViewItem * i )
{
    ValuedListItem* item = static_cast< ValuedListItem* >( i );
    if( item )
    {
        const AfterActionRequest* request = item->GetValue< const AfterActionRequest >();
        if( request && request->IsDone() )
        {
            AfterActionPlot* plot = CreateNewPlot();
            plot->Add( *request );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: AfterActionRequestList::CreateNewPlot
// Created: AGE 2007-09-26
// -----------------------------------------------------------------------------
AfterActionPlot* AfterActionRequestList::CreateNewPlot()
{
    QDockWindow* plotDock = new QDockWindow( mainWindow_, "plot", WDestructiveClose );
    QVBox* box = new QVBox( plotDock );
    AfterActionPlot* plot = new AfterActionPlot( box, controllers_, publisher_, plotDock );
    plotDock->setWidget( box );
    plotDock->setResizeEnabled( true );
    plotDock->setCloseMode( QDockWindow::Always );
    plotDock->setCaption( tr( "Plot" ) );
    plotDock->undock();
    mainWindow_->setAppropriate( plotDock, false );
    box->show();
    return plot;
}

// -----------------------------------------------------------------------------
// Name: AfterActionRequestList::NotifyCreated
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
void AfterActionRequestList::NotifyCreated( const AfterActionRequest& request )
{
    ValuedListItem* item = factory_.CreateItem( requests_ );
    Display( request, item );
}

// -----------------------------------------------------------------------------
// Name: AfterActionRequestList::NotifyUpdated
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
void AfterActionRequestList::NotifyUpdated( const AfterActionRequest& request )
{
    ValuedListItem* item = FindItem( &request, requests_->firstChild() );
    if( item )
        Display( request, item );
}

// -----------------------------------------------------------------------------
// Name: AfterActionRequestList::Display
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
void AfterActionRequestList::Display( const AfterActionRequest& request, gui::ValuedListItem* item )
{
    item->SetNamed( request );
    item->setPixmap( 1, request.IsPending() ? pendingPixmap_ :
                        request.IsDone() ? donePixmap_ :
                        request.IsFailed() ? failedPixmap_ :
                        QPixmap() );
    item->SetToolTip( request.ErrorMessage() );
    item->setDragEnabled( request.IsDone() );
}
