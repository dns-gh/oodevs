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

namespace
{
    struct MergeAvailabilities
    {
        std::map< std::string, kernel::Availability > availabilities_;

        void operator()( const kernel::Entity_ABC& element )
        {
            if( const MedicalStates* pState = element.Retrieve< MedicalStates >() )
                for( unsigned int i = 0; i < pState->dispoReleveAmbulances_.size(); ++i )
                {
                    kernel::Availability curAvailability( 0
                        , pState->dispoReleveAmbulances_[ i ].total_
                        , pState->dispoReleveAmbulances_[ i ].available_
                        , pState->dispoReleveAmbulances_[ i ].atWork_
                        , pState->dispoReleveAmbulances_[ i ].atRest_ );
                    availabilities_[ pState->dispoReleveAmbulances_[ i ].type_->GetName() ] += curAvailability;
                }
        }
    };
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

    MergeAvailabilities merged;
    logistic_helpers::VisitEntityAndSubordinatesUpToBaseLog< MergeAvailabilities >( *selected_, merged );
    DisplayModelWithAvailabilities( merged.availabilities_ );
}
