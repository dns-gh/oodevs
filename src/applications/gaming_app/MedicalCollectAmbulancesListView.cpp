// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "MedicalCollectAmbulancesListView.h"

// -----------------------------------------------------------------------------
// Name: MedicalCollectAmbulancesListView constructor
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
MedicalCollectAmbulancesListView::MedicalCollectAmbulancesListView( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
    : MedicalAvailabilitiesListView_ABC( parent, controllers, factory )
{
    AddColumn( QListView::tr( "Collect ambulances" ) )
    .AddColumn( QListView::tr( "Total" ) )
    .AddColumn( QListView::tr( "Available" ) )
    .AddColumn( QListView::tr( "Working" ) )
    .AddColumn( QListView::tr( "Resting" ) );
}

// -----------------------------------------------------------------------------
// Name: MedicalCollectAmbulancesListView destructor
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
MedicalCollectAmbulancesListView::~MedicalCollectAmbulancesListView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MedicalCollectAmbulancesListView::NotifyUpdated
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
void MedicalCollectAmbulancesListView::NotifyUpdated( const MedicalStates& a )
{
    if( ShouldUpdate( a ) )
        DeleteTail( DisplayList( a.dispoRamassageAmbulances_.begin(), a.dispoRamassageAmbulances_.end() ) );
}
