// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "MedicalDoctorsListView.h"

// -----------------------------------------------------------------------------
// Name: MedicalDoctorsListView constructor
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
MedicalDoctorsListView::MedicalDoctorsListView( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
    : MedicalAvailabilitiesListView_ABC( parent, controllers, factory )
{
    AddColumn( QListView::tr( "Doctors" ) )
    .AddColumn( QListView::tr( "Total" ) )
    .AddColumn( QListView::tr( "Available" ) )
    .AddColumn( QListView::tr( "Working" ) )
    .AddColumn( QListView::tr( "Resting" ) );
}

// -----------------------------------------------------------------------------
// Name: MedicalDoctorsListView destructor
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
MedicalDoctorsListView::~MedicalDoctorsListView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MedicalDoctorsListView::NotifyUpdated
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
void MedicalDoctorsListView::NotifyUpdated( const MedicalStates& a )
{
    if( ShouldUpdate( a ) )
        DeleteTail( DisplayList( a.dispoDoctors_.begin(), a.dispoDoctors_.end() ) );
}
