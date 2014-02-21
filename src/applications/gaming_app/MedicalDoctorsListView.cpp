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

#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/Tools.h"
#include "gaming/MedicalStates.h"

// -----------------------------------------------------------------------------
// Name: MedicalDoctorsListView constructor
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
MedicalDoctorsListView::MedicalDoctorsListView( QWidget* parent, kernel::Controllers& controllers )
    : LogisticResourcesListView_ABC< MedicalStates >( "medical_doctors_listview", parent, controllers )
{
    QStringList list;
    list.append( tools::translate( "MedicalDoctorsListView", "Doctors" ) );
    list.append( tools::translate( "MedicalDoctorsListView", "Unit" ) );
    list.append( tools::translate( "MedicalDoctorsListView", "Total" ) );
    list.append( tools::translate( "MedicalDoctorsListView", "Available" ) );
    list.append( tools::translate( "MedicalDoctorsListView", "Working" ) );
    list.append( tools::translate( "MedicalDoctorsListView", "Resting" ) );
    list.append( tools::translate( "MedicalDoctorsListView", "Lent" ) );
    model_.setHorizontalHeaderLabels( list );
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
// Name: MedicalDoctorsListView::GetAvailabilities
// Created: MMC 2013-01-28
// -----------------------------------------------------------------------------
const std::vector< kernel::Availability >* MedicalDoctorsListView::GetAvailabilities( const MedicalStates& states ) const
{
    return &states.dispoDoctors_;
}
