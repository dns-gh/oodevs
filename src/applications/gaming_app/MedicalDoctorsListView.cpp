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

namespace
{
    struct MergeAvailabilities
    {
        std::map< std::string, kernel::Availability > availabilities_;

        void operator()( const kernel::Entity_ABC& element )
        {
            if( const MedicalStates* pState = element.Retrieve< MedicalStates >() )
                for( unsigned int i = 0; i < pState->dispoDoctors_.size(); ++i )
                {
                    kernel::Availability curAvailability( 0
                        , pState->dispoDoctors_[ i ].total_
                        , pState->dispoDoctors_[ i ].available_
                        , pState->dispoDoctors_[ i ].atWork_
                        , pState->dispoDoctors_[ i ].atRest_ );
                    availabilities_[ pState->dispoDoctors_[ i ].type_->GetName() ] += curAvailability;
                }
        }
    };
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

    MergeAvailabilities merged;
    logistic_helpers::VisitEntityAndSubordinatesUpToBaseLog< MergeAvailabilities >( *selected_, merged );
    DisplayModelWithAvailabilities( merged.availabilities_ );
}
