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
#include "SupplyQuotasWidget.h"
#include "SupplyTransportersListView.h"
#include "gaming/Tools.h"

// -----------------------------------------------------------------------------
// Name: InfoSupplyDialog constructor
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
InfoSupplyDialog::InfoSupplyDialog( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
    : InfoDialog< SupplyStates >( parent, controllers, tools::translate( "InfoSupplyDialog", "Supply system" ) )
{
    QTabWidget* tabs = new QTabWidget( RootWidget() );
    tabs->addTab( new SupplyConsignsWidget( tabs, controllers, factory ), tools::translate( "InfoSupplyDialog", "Consigns" ) );
    Q3VBox* sqbox = new Q3VBox( tabs );
    new SupplyStocksListView( sqbox, controllers, factory );
    //new SupplyQuotasListView( sqbox, controllers, factory );

    new SupplyQuotasWidget( sqbox, controllers, factory );
    tabs->addTab( sqbox, tools::translate( "InfoSupplyDialog", "Stocks && Quotas" ) );
    Q3VBox* tbox = new Q3VBox( tabs );
    new SupplyTransportersListView( tbox, controllers, factory );
    tabs->addTab( tbox, tools::translate( "InfoSupplyDialog", "Transporters" ) );
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
    const LogSupplyConsigns* consigns = element.Retrieve< LogSupplyConsigns >();
    //const Quotas* quotas = element.Retrieve< Quotas >();
    return ( consigns && consigns->IsRelevant() )
      //  || ( quotas && quotas->IsRelevant() )
        || element.Retrieve< SupplyStates >();
}
