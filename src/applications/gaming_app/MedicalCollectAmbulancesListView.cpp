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

namespace
{
    struct MergeAvailabilities
    {
        std::map< std::string, kernel::Availability > availabilities_;

        void operator()( const kernel::Entity_ABC& element )
        {
            if( const MedicalStates* pState = element.Retrieve< MedicalStates >() )
                for( unsigned int i = 0; i < pState->dispoRamassageAmbulances_.size(); ++i )
                {
                    kernel::Availability curAvailability( 0
                        , pState->dispoRamassageAmbulances_[ i ].total_
                        , pState->dispoRamassageAmbulances_[ i ].available_
                        , pState->dispoRamassageAmbulances_[ i ].atWork_
                        , pState->dispoRamassageAmbulances_[ i ].atRest_ );
                    availabilities_[ pState->dispoRamassageAmbulances_[ i ].type_->GetName() ] += curAvailability;
                }
        }
    };
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
    MergeAvailabilities merged;
    logistic_helpers::VisitEntityAndSubordinatesUpToBaseLog< MergeAvailabilities >( *selected_, merged );
    DisplayModelWithAvailabilities( merged.availabilities_ );
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
    MergeAvailabilities merged;
    logistic_helpers::VisitEntityAndSubordinatesUpToBaseLog< MergeAvailabilities >( *selected_, merged );
    DisplayModelWithAvailabilities( merged.availabilities_ );
    show();
}
