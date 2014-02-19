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

#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/Tools.h"
#include "gaming/MedicalStates.h"

// -----------------------------------------------------------------------------
// Name: MedicalCollectAmbulancesListView constructor
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
MedicalCollectAmbulancesListView::MedicalCollectAmbulancesListView( QWidget* parent, kernel::Controllers& controllers )
    : LogisticResourcesListView_ABC< MedicalStates >( "medical_collect_ambulances_listview", parent, controllers )
{
    QStringList list;
    list.append( tools::translate( "MedicalCollectAmbulancesListView", "Collect ambulances" ) );
    list.append( tools::translate( "MedicalCollectAmbulancesListView", "Unit" ) );
    list.append( tools::translate( "MedicalCollectAmbulancesListView", "Total" ) );
    list.append( tools::translate( "MedicalCollectAmbulancesListView", "Available" ) );
    list.append( tools::translate( "MedicalCollectAmbulancesListView", "Working" ) );
    list.append( tools::translate( "MedicalCollectAmbulancesListView", "Resting" ) );
    list.append( tools::translate( "MedicalCollectAmbulancesListView", "Lent" ) );
    model_.setHorizontalHeaderLabels( list );
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
// Name: MedicalCollectAmbulancesListView::GetAvailabilities
// Created: MMC 2013-01-28
// -----------------------------------------------------------------------------
const std::vector< kernel::Availability >* MedicalCollectAmbulancesListView::GetAvailabilities( const MedicalStates& states ) const
{
    return &states.dispoRamassageAmbulances_;
}
