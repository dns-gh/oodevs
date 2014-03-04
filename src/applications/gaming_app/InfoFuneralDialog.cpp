// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "InfoFuneralDialog.h"
#include "LogisticsRequestsFuneralWidget.h"
#include "clients_kernel/Tools.h"
#include "gaming/LogisticConsigns.h"
#include "gaming/LogisticHelpers.h"
#include "gaming/LogFuneralConsign.h"
#include "gaming/SupplyStates.h"

// -----------------------------------------------------------------------------
// Name: InfoFuneralDialog constructor
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
InfoFuneralDialog::InfoFuneralDialog( QWidget* parent, kernel::Controllers& controllers
    , gui::DisplayExtractor& extractor, const kernel::Profile_ABC& profile, const SimulationController& simulationController, Model& model )
    : InfoDialog_Base( controllers, parent, tools::translate( "InfoFuneralDialog", "Funeral system" ) )
    , widget_( 0 )
{
    QTabWidget* tabs = new QTabWidget( RootWidget() );
    widget_ =  new LogisticsRequestsFuneralWidget( tabs, controllers, extractor, profile, simulationController, model );
    tabs->addTab( widget_, tools::translate( "InfoFuneralDialog", "Instructions" ) );
}

// -----------------------------------------------------------------------------
// Name: InfoFuneralDialog destructor
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
InfoFuneralDialog::~InfoFuneralDialog()
{
    // NOTHING
}

namespace
{
    struct FuneralRelevant
    {
        FuneralRelevant() {}
        bool operator()( const kernel::Entity_ABC& element )
        {
            const LogFuneralConsigns* consigns = element.Retrieve< LogFuneralConsigns >();
            return ( consigns && consigns->IsRelevant() || element.Retrieve< SupplyStates >() );
        }
    };
}

// -----------------------------------------------------------------------------
// Name: InfoFuneralDialog::ShouldDisplay
// Created: SBO 2007-03-30
// -----------------------------------------------------------------------------
bool InfoFuneralDialog::ShouldDisplay( const kernel::Entity_ABC& element ) const
{
    FuneralRelevant funeralRelevant;
    return logistic_helpers::CheckEntityAndSubordinatesUpToBaseLog( element, funeralRelevant );
}

// -----------------------------------------------------------------------------
// Name: InfoFuneralDialog::NotifyUpdated
// Created: NPT 2013-05-17
// -----------------------------------------------------------------------------
void InfoFuneralDialog::NotifyUpdated( const Troops& /*troops*/ )
{
    if( selected_ )
        SetEnabled( selected_ && ShouldDisplay( *selected_ ) );
}

// -----------------------------------------------------------------------------
// Name: InfoFuneralDialog::Purge
// Created: LGY 2013-12-11
// -----------------------------------------------------------------------------
void InfoFuneralDialog::Purge()
{
    widget_->Purge();
}

// -----------------------------------------------------------------------------
// Name: InfoFuneralDialog::FillCurrentModel
// Created: LGY 2013-12-11
// -----------------------------------------------------------------------------
void InfoFuneralDialog::FillCurrentModel( const kernel::Entity_ABC& entity )
{
    widget_->FillCurrentModel( entity );
}

// -----------------------------------------------------------------------------
// Name: InfoFuneralDialog::FillHistoryModel
// Created: LGY 2013-12-11
// -----------------------------------------------------------------------------
void InfoFuneralDialog::FillHistoryModel()
{
    widget_->FillHistoryModel();
}

// -----------------------------------------------------------------------------
// Name: InfoFuneralDialog::IsVisible
// Created: LGY 2014-02-05
// -----------------------------------------------------------------------------
bool InfoFuneralDialog::IsVisible() const
{
    return isVisible();
}
