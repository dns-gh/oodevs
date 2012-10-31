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
// Name: MedicalCollectAmbulancesListView::NotifyUpdated
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
void MedicalCollectAmbulancesListView::NotifyUpdated( const MedicalStates& a )
{
    if( ShouldUpdate( a ) )
    {
        ResizeModelOnNewContent( static_cast< int >( a.dispoRamassageAmbulances_.size() ) );
        for( unsigned int i = 0; i < a.dispoRamassageAmbulances_.size(); ++i )
        {
            model_.item( i, 0 )->setText( QString( a.dispoRamassageAmbulances_[ i ].type_->GetName().c_str() ) );
            model_.item( i, 1 )->setText( QString::number( a.dispoRamassageAmbulances_[ i ].total_ ) );
            model_.item( i, 2 )->setText( QString::number( a.dispoRamassageAmbulances_[ i ].available_ ) );
            model_.item( i, 3 )->setText( QString::number( a.dispoRamassageAmbulances_[ i ].atWork_ ) );
            model_.item( i, 4 )->setText( QString::number( a.dispoRamassageAmbulances_[ i ].atRest_ ) );
        }
    }
}
