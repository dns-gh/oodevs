// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "SupplyQuotasListView.h"

// -----------------------------------------------------------------------------
// Name: SupplyQuotasListView constructor
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
SupplyQuotasListView::SupplyQuotasListView( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
    : QuotasAvailabilitiesListView_ABC( parent, controllers, factory )
{
    AddColumn( QListView::tr( "Type" ) )
    .AddColumn( QListView::tr( "Quota" ) );
}

// -----------------------------------------------------------------------------
// Name: SupplyQuotasListView destructor
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
SupplyQuotasListView::~SupplyQuotasListView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SupplyQuotasListView::NotifyUpdated
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
void SupplyQuotasListView::NotifyUpdated( const Quotas& a )
{
    if( ShouldUpdate( a ) )
        DeleteTail( DisplayList( a.quotas_.begin(), a.quotas_.end() ) );
}
