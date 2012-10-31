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
// Name: MedicalReliefAmbulancesListView::NotifyUpdated
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
void MedicalReliefAmbulancesListView::NotifyUpdated( const MedicalStates& a )
{
    if( ShouldUpdate( a ) )
    {
        ResizeModelOnNewContent( static_cast< int >( a.dispoReleveAmbulances_.size() ) );
        for( int i = 0; i < a.dispoReleveAmbulances_.size(); ++i )
        {
            model_.item( i, 0 )->setText( QString( a.dispoReleveAmbulances_[ i ].type_->GetName().c_str() ) );
            model_.item( i, 1 )->setText( QString::number( a.dispoReleveAmbulances_[ i ].total_ ) );
            model_.item( i, 2 )->setText( QString::number( a.dispoReleveAmbulances_[ i ].available_ ) );
            model_.item( i, 3 )->setText( QString::number( a.dispoReleveAmbulances_[ i ].atWork_ ) );
            model_.item( i, 4 )->setText( QString::number( a.dispoReleveAmbulances_[ i ].atRest_ ) );
        }
    }
}
