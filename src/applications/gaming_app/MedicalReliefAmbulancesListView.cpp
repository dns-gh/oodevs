// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "MedicalReliefAmbulancesListView.h"

// -----------------------------------------------------------------------------
// Name: MedicalReliefAmbulancesListView constructor
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
MedicalReliefAmbulancesListView::MedicalReliefAmbulancesListView( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
    : MedicalAvailabilitiesListView_ABC( parent, controllers, factory )
{
    AddColumn( QListView::tr( "Relief ambulances" ) )
    .AddColumn( QListView::tr( "Total" ) )
    .AddColumn( QListView::tr( "Available" ) )
    .AddColumn( QListView::tr( "Working" ) )
    .AddColumn( QListView::tr( "Resting" ) );
}

// -----------------------------------------------------------------------------
// Name: MedicalReliefAmbulancesListView destructor
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
MedicalReliefAmbulancesListView::~MedicalReliefAmbulancesListView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MedicalReliefAmbulancesListView::NotifyUpdated
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
void MedicalReliefAmbulancesListView::NotifyUpdated( const MedicalStates& a )
{
    if( ShouldUpdate( a ) )
        DeleteTail( DisplayList( a.dispoReleveAmbulances_.begin(), a.dispoReleveAmbulances_.end() ) );
}
