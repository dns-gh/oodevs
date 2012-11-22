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
#include "LogisticConsignsWidget.h"
#include "SupplyStocksListView.h"
#include "SupplyQuotasWidget.h"
#include "SupplyTransportersListView.h"
#include "clients_kernel/EntityHelpers.h"
#include "clients_kernel/Tools.h"
#include "gaming/LogSupplyConsign.h"

using namespace kernel;
using namespace EntityHelpers;

// -----------------------------------------------------------------------------
// Name: InfoSupplyDialog constructor
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
InfoSupplyDialog::InfoSupplyDialog( QWidget* parent, Controllers& controllers, gui::ItemFactory_ABC& factory, kernel::DisplayExtractor_ABC& extractor )
    : InfoDialog< SupplyStates >( parent, controllers, tools::translate( "InfoSupplyDialog", "Supply system" ) )
{
    tabs_ = new QTabWidget( RootWidget() );
    tabs_->addTab( new LogisticConsignsWidget< LogSupplyConsign, LogSupplyConsigns >( tabs_, controllers, extractor ), tools::translate( "InfoSupplyDialog", "Consigns" ) );

    Q3VBox* sqbox = new Q3VBox( tabs_ );
    supplyStocksListView_ = new SupplyStocksListView( sqbox, controllers );
    supplyQuotasWidget_ = new SupplyQuotasWidget( sqbox, controllers, factory );
    tabs_->addTab( sqbox, tools::translate( "InfoSupplyDialog", "Stocks && Quotas" ) );

    Q3VBox* tbox = new Q3VBox( tabs_ );
    new SupplyTransportersListView( tbox, controllers );
    tabs_->addTab( tbox, tools::translate( "InfoSupplyDialog", "Transporters" ) );

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
bool InfoSupplyDialog::ShouldDisplay( const Entity_ABC& entity ) const
{
    return entity.Retrieve< SupplyStates >() || IsLogisticBase( entity );
}

// -----------------------------------------------------------------------------
// Name: InfoSupplyDialog::NotifySelected
// Created: MMC 2012-10-01
// -----------------------------------------------------------------------------
void InfoSupplyDialog::NotifySelected( const kernel::Entity_ABC* entity )
{
    InfoDialog< SupplyStates >::NotifySelected( entity );
    tabs_->setCurrentIndex( 0 );
}
