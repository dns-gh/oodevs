// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
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

    QWidget* transportersWidget = new QWidget();
    QVBoxLayout* transporterslayout = new QVBoxLayout( transportersWidget );
    transporterslayout->addWidget( new SupplyTransportersListView( this, controllers ) );

    QWidget* statusWidget = new QWidget();
    QVBoxLayout* statusLayout = new QVBoxLayout( statusWidget );
    statusLayout->addWidget( new SupplyStatusWidget( this, controllers, factory ) );

    supplyQuotasWidget_ = new SupplyQuotasWidget( this, controllers, factory );
    tabs_->addTab( new LogisticConsignsWidget< LogSupplyConsign, LogSupplyConsigns >( this, controllers, extractor ), tools::translate( "InfoSupplyDialog", "Consigns" ) );
    tabs_->addTab( new SupplyStocksListView( this, controllers ), tools::translate( "InfoSupplyDialog", "Stocks" ) );
    tabs_->addTab( supplyQuotasWidget_, tools::translate( "InfoSupplyDialog", "Quotas" ) );
    tabs_->addTab( transportersWidget, tools::translate( "InfoSupplyDialog", "Transporters" ) );
    tabs_->addTab( statusWidget, tools::translate( "SupplyStates", "Chain status" ) );
    setMinimumWidth( 520 );
}

// -----------------------------------------------------------------------------
// Name: InfoSupplyDialog destructor
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
InfoSupplyDialog::~InfoSupplyDialog()
{
    // NOTHING
}

namespace
{
    struct SupplyRevelant
    {
        SupplyRevelant() {}
        bool operator()( const kernel::Entity_ABC& element )
        {
            const LogSupplyConsigns* consigns = element.Retrieve< LogSupplyConsigns >();
            return ( ( consigns && consigns->IsRelevant() ) || element.Retrieve< SupplyStates >() );
        }
    };
}

// -----------------------------------------------------------------------------
// Name: InfoSupplyDialog::ShouldDisplay
// Created: SBO 2007-03-30
// -----------------------------------------------------------------------------
bool InfoSupplyDialog::ShouldDisplay( const Entity_ABC& entity ) const
{
    SupplyRevelant supplyRevelant;
    return logistic_helpers::CheckEntityAndSubordinatesUpToBaseLog( entity, supplyRevelant );
}

// -----------------------------------------------------------------------------
// Name: InfoSupplyDialog::NotifySelected
// Created: MMC 2012-10-01
// -----------------------------------------------------------------------------
void InfoSupplyDialog::NotifySelected( const kernel::Entity_ABC* entity )
{
    if( entity )
    {
        if( kernel::EntityHelpers::IsLogisticBase( *entity ) )
        {
            if( tabs_->widget( 2 ) != supplyQuotasWidget_ )
                tabs_->insertTab( 2, supplyQuotasWidget_, tools::translate( "InfoSupplyDialog", "Quotas" ) );
        }
        else if( tabs_->widget( 2 ) == supplyQuotasWidget_ )
            tabs_->removeTab( 2 );
        InfoDialog< SupplyStates >::NotifySelected( entity );
    }
}
