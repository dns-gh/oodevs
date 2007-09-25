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
#include "clients_gui/resources.h"
#include "clients_gui/ListItemToolTip.h"
#include "gaming/AfterActionRequest.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: AfterActionRequestList constructor
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
AfterActionRequestList::AfterActionRequestList( QWidget* parent, Controllers& controllers, ItemFactory_ABC& factory )
    : QVBox( parent )
    , controllers_( controllers )
    , factory_( factory )
    , pendingPixmap_( MAKE_PIXMAP( autolog ) ) // $$$$ AGE 2007-09-25: 
    , donePixmap_   ( MAKE_PIXMAP( check ) )
    , failedPixmap_ ( MAKE_PIXMAP( conflict ) ) // $$$$ AGE 2007-09-25: 
{
    requests_ = new gui::ListDisplayer< AfterActionRequestList >( this, *this, factory );
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
void AfterActionRequestList::OnDoubleClicked( QListViewItem * )
{

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
}
