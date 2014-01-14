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
#include "clients_kernel/Tools.h"
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/Availability.h"
#include <boost/bind.hpp>

// -----------------------------------------------------------------------------
// Name: MedicalDoctorsListView constructor
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
MedicalDoctorsListView::MedicalDoctorsListView( QWidget* parent, kernel::Controllers& controllers )
    : ResourcesListView_ABC< MedicalStates >( parent, controllers )
{
    QStringList list;
    list.append( tools::translate( "MedicalDoctorsListView", "Doctors" ) );
    list.append( tools::translate( "MedicalDoctorsListView", "Total" ) );
    list.append( tools::translate( "MedicalDoctorsListView", "Available" ) );
    list.append( tools::translate( "MedicalDoctorsListView", "Working" ) );
    list.append( tools::translate( "MedicalDoctorsListView", "Resting" ) );
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

// -----------------------------------------------------------------------------
// Name: MedicalDoctorsListView::NotifyUpdated
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
void MedicalDoctorsListView::NotifyUpdated( const MedicalStates& a )
{
    if( !isVisible() || !selected_ )
        return;
    if( !HasRetrieveForLogistic( *selected_, a ) )
        return;
    DisplaySelectionAvailabilities();
}

// -----------------------------------------------------------------------------
// Name: MedicalDoctorsListView::NotifyUpdated
// Created: MMC 2013-01-28
// -----------------------------------------------------------------------------
void MedicalDoctorsListView::NotifySelected( const kernel::Entity_ABC* entity )
{
    UpdateSelected( entity );
}

// -----------------------------------------------------------------------------
// Name: MedicalDoctorsListView::NotifyUpdated
// Created: MMC 2013-01-28
// -----------------------------------------------------------------------------
void MedicalDoctorsListView::UpdateSelected( const kernel::Entity_ABC* entity )
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
