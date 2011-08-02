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
#include "LogisticStatusWidgets.h"
#include "MedicalConsignsWidget.h"
#include "MedicalReliefAmbulancesListView.h"
#include "MedicalCollectAmbulancesListView.h"
#include "MedicalDoctorsListView.h"
#include "gaming/Tools.h"

// -----------------------------------------------------------------------------
// Name: InfoMedicalDialog constructor
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
InfoMedicalDialog::InfoMedicalDialog( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
    : InfoDialog< MedicalStates >( parent, controllers, tools::translate( "InfoMedicalDialog", "Medical system" ) )
{
    QTabWidget* tabs = new QTabWidget( RootWidget() );
    tabs->addTab( new MedicalConsignsWidget( tabs, controllers, factory ), tools::translate( "InfoMedicalDialog", "Consigns" ) );
    Q3VBox* box = new Q3VBox( tabs );
    new MedicalReliefAmbulancesListView( box, controllers, factory );
    new MedicalCollectAmbulancesListView( box, controllers, factory );
    new MedicalDoctorsListView( box, controllers, factory );
    tabs->addTab( box, tools::translate( "InfoMedicalDialog", "Equipment availabilities" ) );
    new MedicalStatusWidget( RootWidget(), controllers, factory );
}

// -----------------------------------------------------------------------------
// Name: InfoMedicalDialog destructor
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
InfoMedicalDialog::~InfoMedicalDialog()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InfoMedicalDialog::ShouldDisplay
// Created: SBO 2007-03-30
// -----------------------------------------------------------------------------
bool InfoMedicalDialog::ShouldDisplay( const kernel::Entity_ABC& element ) const
{
    const LogMedicalConsigns* consigns = element.Retrieve< LogMedicalConsigns >();
    return ( consigns && consigns->IsRelevant() ) || element.Retrieve< MedicalStates >();
}
