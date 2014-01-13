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
#include "LogisticsRequestsSupplyWidget.h"
#include "LogisticStatusWidgets.h"
#include "SupplyQuotasWidget.h"
#include "SupplyStocksListView.h"
#include "SupplyTransportersListView.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/EntityHelpers.h"
#include "clients_kernel/Tools.h"
#include "gaming/LogisticConsigns.h"
#include "gaming/LogisticHelpers.h"
#include "gaming/LogSupplyConsign.h"

using namespace kernel;
using namespace EntityHelpers;

// -----------------------------------------------------------------------------
// Name: InfoSupplyDialog constructor
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
InfoSupplyDialog::InfoSupplyDialog( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory,
                                    gui::DisplayExtractor& extractor, const kernel::Profile_ABC& profile,
                                    const SimulationController& simulationController, Model& model )
    : InfoDialog< SupplyStates >( parent, controllers, tools::translate( "InfoSupplyDialog", "Supply system" ) )
    , widget_( 0 )
{
    tabs_ = new QTabWidget( RootWidget() );

    QWidget* transportersWidget = new QWidget();
    QVBoxLayout* transporterslayout = new QVBoxLayout( transportersWidget );
    transporterslayout->addWidget( new SupplyTransportersListView( this, controllers ) );

    QWidget* statusWidget = new QWidget();
    QVBoxLayout* statusLayout = new QVBoxLayout( statusWidget );
    statusLayout->addWidget( new SupplyStatusWidget( this, controllers ) );

    supplyQuotasWidget_ = new SupplyQuotasWidget( this, controllers, factory );
    widget_ = new LogisticsRequestsSupplyWidget( this, controllers, extractor, profile, simulationController, model );
    tabs_->addTab( widget_, tools::translate( "InfoSupplyDialog", "Instructions" ) );
    tabs_->addTab( new SupplyStocksListView( this, controllers ), tools::translate( "InfoSupplyDialog", "Stocks" ) );
    tabs_->addTab( supplyQuotasWidget_, tools::translate( "InfoSupplyDialog", "Quotas" ) );
    tabs_->addTab( transportersWidget, tools::translate( "InfoSupplyDialog", "Transporters" ) );
    tabs_->addTab( statusWidget, tools::translate( "InfoSupplyDialog", "Chain status" ) );
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
    struct SupplyRelevant
    {
        SupplyRelevant() {}
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
    SupplyRelevant supplyRelevant;
    return logistic_helpers::CheckEntityAndSubordinatesUpToBaseLog( entity, supplyRelevant );
}

// -----------------------------------------------------------------------------
// Name: InfoSupplyDialog::NotifySelected
// Created: MMC 2012-10-01
// -----------------------------------------------------------------------------
void InfoSupplyDialog::NotifySelected( const kernel::Entity_ABC* entity )
{
    selected_ = entity;
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

// -----------------------------------------------------------------------------
// Name: InfoSupplyDialog::NotifyUpdated
// Created: NPT 2013-05-17
// -----------------------------------------------------------------------------
void InfoSupplyDialog::NotifyUpdated( const SupplyStates& /*supplyStates*/ )
{
    if( selected_ )
        InfoDialog< SupplyStates >::NotifySelected( selected_ );
}

// -----------------------------------------------------------------------------
// Name: InfoSupplyDialog::Purge
// Created: LGY 2013-12-11
// -----------------------------------------------------------------------------
void InfoSupplyDialog::Purge()
{
    widget_->Purge();
}

// -----------------------------------------------------------------------------
// Name: InfoSupplyDialog::FillCurrentModel
// Created: LGY 2013-12-11
// -----------------------------------------------------------------------------
void InfoSupplyDialog::FillCurrentModel( const kernel::Entity_ABC& entity )
{
    widget_->FillCurrentModel( entity );
}

// -----------------------------------------------------------------------------
// Name: InfoSupplyDialog::FillHistoryModel
// Created: LGY 2013-12-11
// -----------------------------------------------------------------------------
void InfoSupplyDialog::FillHistoryModel()
{
    widget_->FillHistoryModel();
}
