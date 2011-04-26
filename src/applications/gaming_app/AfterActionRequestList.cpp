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
#include "gaming/IndicatorRequest.h"
#include "gaming/Simulation.h"
#include "IndicatorPlotFactory.h"
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
        {
            // NOTHING
        }
        virtual QDragObject* dragObject()
        {
            ValuedListItem* item = static_cast< ValuedListItem* >( selectedItem() );
            if( !item )
                return 0;
            const IndicatorRequest* request = item->GetValue< const IndicatorRequest >();
            return new ValuedDragObject( request, this );
        }
    };
}

// -----------------------------------------------------------------------------
// Name: AfterActionRequestList constructor
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
AfterActionRequestList::AfterActionRequestList( QWidget* parent, Controllers& controllers, ItemFactory_ABC& factory, IndicatorPlotFactory& plotFactory )
    : QVBox( parent, "AfterActionRequestList" )
    , controllers_  ( controllers )
    , factory_      ( factory )
    , plotFactory_  ( plotFactory )
    , pendingPixmap_( MAKE_PIXMAP( aaa_pending ) )
    , donePixmap_   ( MAKE_PIXMAP( aaa_valid ) )
    , failedPixmap_ ( MAKE_PIXMAP( aaa_broken ) )
{
    requests_ = new MyList( this, factory );
    requests_->AddColumn( tr( "Request" ) );
    requests_->AddColumn( tr( "Status" ) );
    new ListItemToolTip( requests_->viewport(), *requests_ );

    connect( requests_, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int ) ), SLOT( OnRequestPopup( QListViewItem*, const QPoint& ) ) );
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
    if( ValuedListItem* item = static_cast< ValuedListItem* >( i ) )
    {
        const IndicatorRequest* request = item->GetValue< const IndicatorRequest >();
        if( request && request->IsDone() )
            plotFactory_.CreatePlot( *request );
    }
}

// -----------------------------------------------------------------------------
// Name: AfterActionRequestList::OnRequestPopup
// Created: ABR 2011-02-17
// -----------------------------------------------------------------------------
void AfterActionRequestList::OnRequestPopup( QListViewItem* pItem, const QPoint& pos )
{
    popupMenu_.clear();
    if( pItem != 0 )
    {
        popupMenu_.insertItem( tr( "Delete request" ), this, SLOT( OnRemoveItem() ) );
        popupMenu_.insertSeparator();
    }
    popupMenu_.insertItem( tr( "Clear list" ), requests_, SLOT( clear() ) );
    popupMenu_.popup( pos );
}

// -----------------------------------------------------------------------------
// Name: AfterActionRequestList::OnRemoveItem
// Created: ABR 2011-02-17
// -----------------------------------------------------------------------------
void AfterActionRequestList::OnRemoveItem()
{
    requests_->takeItem( requests_->selectedItem() );
}

// -----------------------------------------------------------------------------
// Name: AfterActionRequestList::NotifyCreated
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
void AfterActionRequestList::NotifyCreated( const IndicatorRequest& request )
{
    ValuedListItem* item = factory_.CreateItem( requests_ );
    Display( request, item );
}

// -----------------------------------------------------------------------------
// Name: AfterActionRequestList::NotifyUpdated
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
void AfterActionRequestList::NotifyUpdated( const IndicatorRequest& request )
{
    if( ValuedListItem* item = FindItem( &request, requests_->firstChild() ) )
        Display( request, item );
}

// -----------------------------------------------------------------------------
// Name: AfterActionRequestList::NotifyUpdated
// Created: JSR 2010-06-02
// -----------------------------------------------------------------------------
void AfterActionRequestList::NotifyUpdated( const Simulation& simulation )
{
    if( !simulation.IsConnected() )
        requests_->clear();
}

// -----------------------------------------------------------------------------
// Name: AfterActionRequestList::Display
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
void AfterActionRequestList::Display( const IndicatorRequest& request, ValuedListItem* item )
{
    item->SetNamed( request );
    item->setPixmap( 1, request.IsPending() ? pendingPixmap_ :
                        request.IsDone() ? donePixmap_ :
                        request.IsFailed() ? failedPixmap_ :
                        QPixmap() );
    item->SetToolTip( request.ErrorMessage() );
    item->setDragEnabled( request.IsDone() );
}
