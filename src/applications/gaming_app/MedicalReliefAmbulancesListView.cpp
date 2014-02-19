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

#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/Tools.h"
#include "gaming/MedicalStates.h"

// -----------------------------------------------------------------------------
// Name: MedicalReliefAmbulancesListView constructor
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
MedicalReliefAmbulancesListView::MedicalReliefAmbulancesListView( QWidget* parent, kernel::Controllers& controllers )
    : LogisticResourcesListView_ABC< MedicalStates >( "medical_relief_ambulances_listview", parent, controllers )
{
    QStringList list;
    list.append( tools::translate( "MedicalReliefAmbulancesListView", "Relief ambulances" ) );
    list.append( tools::translate( "MedicalReliefAmbulancesListView", "Unit" ) );
    list.append( tools::translate( "MedicalReliefAmbulancesListView", "Total" ) );
    list.append( tools::translate( "MedicalReliefAmbulancesListView", "Available" ) );
    list.append( tools::translate( "MedicalReliefAmbulancesListView", "Working" ) );
    list.append( tools::translate( "MedicalReliefAmbulancesListView", "Resting" ) );
    list.append( tools::translate( "MedicalReliefAmbulancesListView", "Lent" ) );
    model_.setHorizontalHeaderLabels( list );
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
// Name: MedicalReliefAmbulancesListView::GetAvailabilities
// Created: MMC 2013-01-28
// -----------------------------------------------------------------------------
const std::vector< kernel::Availability >* MedicalReliefAmbulancesListView::GetAvailabilities( const MedicalStates& states ) const
{
    return &states.dispoReleveAmbulances_;
}
