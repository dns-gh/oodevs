// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "SupplyConsignsWidget.h"
#include "gaming/LogSupplyConsign.h"
#include "gaming/SupplyStates.h"
#include "gaming/SupplyRecipientResourcesRequest.h"
#include "gaming/SupplyResourceRequest.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/Automat_ABC.h"

// -----------------------------------------------------------------------------
// Name: SupplyConsignsWidget constructor
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
SupplyConsignsWidget::SupplyConsignsWidget( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
    : LogisticConsignsWidget_ABC< SupplyConsignsWidget, LogSupplyConsign, LogSupplyConsigns >( parent, controllers, factory )
    , factory_( factory )
{
    AddConsignColumn( tools::translate( "SupplyConsignsWidget", "Supplier:" ) );
    AddConsignColumn( tools::translate( "SupplyConsignsWidget", "Convoyer:" ) );
    AddConsignColumn( tools::translate( "SupplyConsignsWidget", "State:" ) );
    AddConsignColumn( tools::translate( "SupplyConsignsWidget", "Current state end :" ) );
}

// -----------------------------------------------------------------------------
// Name: SupplyConsignsWidget destructor
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
SupplyConsignsWidget::~SupplyConsignsWidget()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SupplyConsignsWidget::Display
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void SupplyConsignsWidget::Display( const LogSupplyConsign* consign, kernel::Displayer_ABC& displayer, gui::ValuedListItem* item )
{
    if( ! consign )
        return;

    LogisticConsignsWidget_ABC< SupplyConsignsWidget, LogSupplyConsign, LogSupplyConsigns >::Display( consign, displayer, item );

    // $$$$ AGE 2006-02-28: crado
    Q3ListViewItem* last  = item->firstChild();
    Q3ListViewItem* child = last;
    while( child && child->text( 0 ) != tools::translate( "SupplyConsignsWidget", "Recipients" ) )
    {
        last = child;
        child = child->nextSibling();
    }
    if( ! child )
    {
        child = factory_.CreateItem( item, last );
        child->setText( 0, tools::translate( "SupplyConsignsWidget", "Recipients" ) );
        child->setText( 1, "" );
    }

    // $$$$ AGE 2006-07-04:
    gui::ListDisplayer< SupplyConsignsWidget >* list = (gui::ListDisplayer< SupplyConsignsWidget >*)( child->listView() );

    list->DeleteTail(
        list->DisplayList( consign->CreateIterator(), child )
        );
}

// -----------------------------------------------------------------------------
// Name: SupplyConsignsWidget::Display
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
void SupplyConsignsWidget::Display( const SupplyRecipientResourcesRequest& request, kernel::Displayer_ABC& /*displayer*/, gui::ValuedListItem* child )
{
    child->setText( 0, tools::translate( "SupplyConsignsWidget", "Recipient %1" ).arg( request.recipient_.GetName() ) );
    child->setText( 1, tools::translate( "SupplyConsignsWidget", "Resources requested/granted/convoyed" ) );
    gui::ListDisplayer< SupplyConsignsWidget >* list = (gui::ListDisplayer< SupplyConsignsWidget >*)( child->listView() );
    list->DeleteTail(
        list->DisplayList( request.CreateIterator(), child )
        );
}

// -----------------------------------------------------------------------------
// Name: SupplyConsignsWidget::Display
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
void SupplyConsignsWidget::Display( const SupplyResourceRequest& request, kernel::Displayer_ABC& displayer, gui::ValuedListItem* )
{
    displayer.Display( 0, request.type_ )
             .Item( 0 ).Start( request.requested_ ).Add( "/" )
                         .Add( request.granted_ ).Add( "/" )
                         .Add( request.convoyed_ ).End();
}
