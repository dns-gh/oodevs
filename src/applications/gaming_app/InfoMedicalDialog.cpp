// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "InfoMedicalDialog.h"
#include "LogisticsRequestsMedicalWidget.h"
#include "LogisticStatusWidget.h"
#include "MedicalCollectAmbulancesListView.h"
#include "MedicalDoctorsListView.h"
#include "MedicalReliefAmbulancesListView.h"
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/Tools.h"
#include "gaming/LogisticConsigns.h"
#include "gaming/LogisticHelpers.h"
#include "gaming/LogMedicalConsign.h"
#include "gaming/LogisticsModel.h"

// -----------------------------------------------------------------------------
// Name: InfoMedicalDialog constructor
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
InfoMedicalDialog::InfoMedicalDialog( QWidget* parent, kernel::Controllers& controllers,
                                      gui::DisplayExtractor& extractor, const kernel::Profile_ABC& profile,
                                      const SimulationController& simulationController, Model& model )
    : InfoDialog< MedicalStates >( parent, controllers, tools::translate( "InfoMedicalDialog", "Medical system" ) )
    , widget_( 0 )
{
    QTabWidget* tabs = new QTabWidget( RootWidget() );
    QVBoxLayout* ambulancesDoctorsLayout = new QVBoxLayout();
    ambulancesDoctorsLayout->addWidget( new MedicalReliefAmbulancesListView( tabs, controllers ) );
    ambulancesDoctorsLayout->addWidget( new MedicalCollectAmbulancesListView( tabs, controllers ) );
    ambulancesDoctorsLayout->addWidget( new MedicalDoctorsListView( tabs, controllers ) );
    QWidget* ambulancesDoctorsWidget = new QWidget( tabs );
    ambulancesDoctorsWidget->setLayout( ambulancesDoctorsLayout );

    QWidget* statusWidget = new QWidget();
    QVBoxLayout* statusLayout = new QVBoxLayout( statusWidget );
    statusLayout->addWidget( new MedicalStatusWidget( tabs, controllers ) );

    widget_= new LogisticsRequestsMedicalWidget( tabs, controllers, extractor, profile, simulationController, model );
    tabs->addTab( widget_, tools::translate( "InfoMedicalDialog", "Instructions" ) );
    tabs->addTab( ambulancesDoctorsWidget, tools::translate( "InfoMedicalDialog", "Equipment availabilities" ) );
    tabs->addTab( statusWidget, tools::translate( "InfoMedicalDialog", "Chain status" ) );
}

// -----------------------------------------------------------------------------
// Name: InfoMedicalDialog destructor
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
InfoMedicalDialog::~InfoMedicalDialog()
{
    // NOTHING
}

namespace
{
    struct MedicalRelevant
    {
        MedicalRelevant() {}
        bool operator()( const kernel::Entity_ABC& element )
        {
            const LogMedicalConsigns* consigns = element.Retrieve< LogMedicalConsigns >();
            return ( ( consigns && consigns->IsRelevant() ) || element.Retrieve< MedicalStates >() );
        }
    };
}

// -----------------------------------------------------------------------------
// Name: InfoMedicalDialog::ShouldDisplay
// Created: SBO 2007-03-30
// -----------------------------------------------------------------------------
bool InfoMedicalDialog::ShouldDisplay( const kernel::Entity_ABC& element ) const
{
    MedicalRelevant medicalRelevant;
    return logistic_helpers::CheckEntityAndSubordinatesUpToBaseLog( element, medicalRelevant );
}

// -----------------------------------------------------------------------------
// Name: InfoMedicalDialog::NotifySelected
// Created: MMC 2013-01-09
// -----------------------------------------------------------------------------
void InfoMedicalDialog::NotifySelected( const kernel::Entity_ABC* entity )
{
    selected_ = entity;
    if( entity )
        InfoDialog< MedicalStates >::NotifySelected( entity );
}

// -----------------------------------------------------------------------------
// Name: InfoMedicalDialog::NotifyUpdated
// Created: NPT 2013-05-17
// -----------------------------------------------------------------------------
void InfoMedicalDialog::NotifyUpdated( const Troops& /*troops*/ )
{
    if( selected_ )
        InfoDialog< MedicalStates >::NotifyUpdated( selected_ );
}

// -----------------------------------------------------------------------------
// Name: InfoMedicalDialog::Purge
// Created: LGY 2013-12-11
// -----------------------------------------------------------------------------
void InfoMedicalDialog::Purge()
{
    widget_->Purge();
}

// -----------------------------------------------------------------------------
// Name: InfoMedicalDialog::FillCurrentModel
// Created: LGY 2013-12-11
// -----------------------------------------------------------------------------
void InfoMedicalDialog::FillCurrentModel( const kernel::Entity_ABC& entity )
{
    widget_->FillCurrentModel( entity );
}

// -----------------------------------------------------------------------------
// Name: InfoMedicalDialog::FillHistoryModel
// Created: LGY 2013-12-11
// -----------------------------------------------------------------------------
void InfoMedicalDialog::FillHistoryModel()
{
    widget_->FillHistoryModel();
}

// -----------------------------------------------------------------------------
// Name: InfoMedicalDialog::IsVisible
// Created: LGY 2014-02-05
// -----------------------------------------------------------------------------
bool InfoMedicalDialog::IsVisible() const
{
    return isVisible();
}
