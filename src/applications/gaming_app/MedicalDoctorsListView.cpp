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
// Name: MedicalDoctorsListView::NotifyUpdated
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
void MedicalDoctorsListView::NotifyUpdated( const MedicalStates& a )
{
    if( ShouldUpdate( a ) )
    {
        ResizeModelOnNewContent( static_cast< int >( a.dispoDoctors_.size() ) );
        for( int i = 0; i < a.dispoDoctors_.size(); ++i )
        {
            model_.item( i, 0 )->setText( QString( a.dispoDoctors_[ i ].type_->GetName().c_str() ) );
            model_.item( i, 1 )->setText( QString::number( a.dispoDoctors_[ i ].total_ ) );
            model_.item( i, 2 )->setText( QString::number( a.dispoDoctors_[ i ].available_ ) );
            model_.item( i, 3 )->setText( QString::number( a.dispoDoctors_[ i ].atWork_ ) );
            model_.item( i, 4 )->setText( QString::number( a.dispoDoctors_[ i ].atRest_ ) );
        }
    }
}
