// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "InfoSupplyDialog.h"
#include "LogisticStatusWidgets.h"
#include "SupplyConsignsWidget.h"
#include "SupplyStocksListView.h"
#include "SupplyQuotasListView.h"
#include "SupplyTransportersListView.h"

// -----------------------------------------------------------------------------
// Name: InfoSupplyDialog constructor
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
InfoSupplyDialog::InfoSupplyDialog( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
    : InfoDialog< SupplyStates >( parent, controllers, tr( "Supply system" ) )
{
    QTabWidget* tabs = new QTabWidget( RootWidget() );
    tabs->addTab( new SupplyConsignsWidget( tabs, controllers, factory ), tr( "Consigns" ) );
    QVBox* box = new QVBox( tabs );
    new SupplyStocksListView( box, controllers, factory );
    new SupplyQuotasListView( box, controllers, factory );
    tabs->addTab( box, tr( "Stocks && Quotas" ) );
    tabs->addTab( new SupplyTransportersListView( tabs, controllers, factory ), tr( "Transporters" ) );
    new SupplyStatusWidget( RootWidget(), controllers, factory );
}

// -----------------------------------------------------------------------------
// Name: InfoSupplyDialog destructor
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
InfoSupplyDialog::~InfoSupplyDialog()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InfoSupplyDialog::ShouldDisplay
// Created: SBO 2007-03-30
// -----------------------------------------------------------------------------
bool InfoSupplyDialog::ShouldDisplay( const kernel::Entity_ABC& element ) const
{
    return element.Retrieve< SupplyStates >()
        || element.Retrieve< LogSupplyConsigns >()
        || element.Retrieve< Quotas >();
}
