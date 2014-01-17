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
#include "clients_kernel/Tools.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/Availability.h"
#include <boost/bind.hpp>

// -----------------------------------------------------------------------------
// Name: MedicalReliefAmbulancesListView constructor
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
MedicalReliefAmbulancesListView::MedicalReliefAmbulancesListView( QWidget* parent, kernel::Controllers& controllers )
    : ResourcesListView_ABC< MedicalStates >( parent, controllers )
{
    QStringList list;
    list.append( tools::translate( "MedicalReliefAmbulancesListView", "Relief ambulances" ) );
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

// -----------------------------------------------------------------------------
// Name: MedicalReliefAmbulancesListView::NotifyUpdated
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
void MedicalReliefAmbulancesListView::NotifyUpdated( const MedicalStates& a )
{
    if( !isVisible() || !selected_ )
        return;
    if( !HasRetrieveForLogistic( *selected_, a ) )
        return;
    DisplaySelectionAvailabilities();
}

// -----------------------------------------------------------------------------
// Name: MedicalReliefAmbulancesListView::NotifyUpdated
// Created: MMC 2013-01-28
// -----------------------------------------------------------------------------
void MedicalReliefAmbulancesListView::NotifySelected( const kernel::Entity_ABC* entity )
{
    UpdateSelected( entity );
}

// -----------------------------------------------------------------------------
// Name: MedicalReliefAmbulancesListView::NotifyUpdated
// Created: MMC 2013-01-28
// -----------------------------------------------------------------------------
void MedicalReliefAmbulancesListView::UpdateSelected( const kernel::Entity_ABC* entity )
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
