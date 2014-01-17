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
#include "clients_kernel/Tools.h"
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/Availability.h"
#include <boost/bind.hpp>

// -----------------------------------------------------------------------------
// Name: MedicalCollectAmbulancesListView constructor
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
MedicalCollectAmbulancesListView::MedicalCollectAmbulancesListView( QWidget* parent, kernel::Controllers& controllers )
    : ResourcesListView_ABC< MedicalStates >( parent, controllers )
{
    QStringList list;
    list.append( tools::translate( "MedicalCollectAmbulancesListView", "Collect ambulances" ) );
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

// -----------------------------------------------------------------------------
// Name: MedicalCollectAmbulancesListView::NotifyUpdated
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
void MedicalCollectAmbulancesListView::NotifyUpdated( const MedicalStates& a )
{
    if( !isVisible() || !selected_ )
        return;
    if( !HasRetrieveForLogistic( *selected_, a ) )
        return;
    DisplaySelectionAvailabilities();
}

// -----------------------------------------------------------------------------
// Name: MedicalCollectAmbulancesListView::NotifyUpdated
// Created: MMC 2013-01-28
// -----------------------------------------------------------------------------
void MedicalCollectAmbulancesListView::NotifySelected( const kernel::Entity_ABC* entity )
{
    UpdateSelected( entity );
}

// -----------------------------------------------------------------------------
// Name: MedicalCollectAmbulancesListView::NotifyUpdated
// Created: MMC 2013-01-28
// -----------------------------------------------------------------------------
void MedicalCollectAmbulancesListView::UpdateSelected( const kernel::Entity_ABC* entity )
{
    selected_ = entity;
    if( !entity )
        return;
    if( !HasRetrieveForLogistic( *selected_ ) )
    {
        hide();
        return;
    }
    DisplaySelectionAvailabilities();
    show();
}
