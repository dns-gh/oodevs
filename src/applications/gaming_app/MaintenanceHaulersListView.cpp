// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "MaintenanceHaulersListView.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/Controllers.h"

// -----------------------------------------------------------------------------
// Name: MaintenanceHaulersListView constructor
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
MaintenanceHaulersListView::MaintenanceHaulersListView( QWidget* parent, kernel::Controllers& controllers )
    : ResourcesListView_ABC< kernel::MaintenanceStates_ABC >( parent, controllers )
{
    QStringList list;
    list.append( tools::translate( "MaintenanceHaulersListView", "Tow trucks" ) );
    list.append( tools::translate( "MaintenanceHaulersListView", "Total" ) );
    list.append( tools::translate( "MaintenanceHaulersListView", "Available" ) );
    list.append( tools::translate( "MaintenanceHaulersListView", "Working" ) );
    list.append( tools::translate( "MaintenanceHaulersListView", "Resting" ) );
    model_.setHorizontalHeaderLabels( list );
}

// -----------------------------------------------------------------------------
// Name: MaintenanceHaulersListView destructor
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
MaintenanceHaulersListView::~MaintenanceHaulersListView()
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
            if( const kernel::MaintenanceStates_ABC* pState = element.Retrieve< kernel::MaintenanceStates_ABC >() )
                for( unsigned int i = 0; i < pState->GetDispoHaulers().size(); ++i )
                {
                    kernel::Availability curAvailability( 0
                        , pState->GetDispoHaulers()[ i ].total_
                        , pState->GetDispoHaulers()[ i ].available_
                        , pState->GetDispoHaulers()[ i ].atWork_
                        , pState->GetDispoHaulers()[ i ].atRest_ );
                    availabilities_[ pState->GetDispoHaulers()[ i ].type_->GetName() ] += curAvailability;
                }
        }
    };
}

// -----------------------------------------------------------------------------
// Name: MaintenanceHaulersListView::NotifyUpdated
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
void MaintenanceHaulersListView::NotifyUpdated( const kernel::MaintenanceStates_ABC& a )
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
// Name: MaintenanceHaulersListView::NotifyUpdated
// Created: MMC 2013-01-28
// -----------------------------------------------------------------------------
void MaintenanceHaulersListView::NotifySelected( const kernel::Entity_ABC* entity )
{
    UpdateSelected( entity );
}

// -----------------------------------------------------------------------------
// Name: MaintenanceHaulersListView::NotifyUpdated
// Created: MMC 2013-01-28
// -----------------------------------------------------------------------------
void MaintenanceHaulersListView::UpdateSelected( const kernel::Entity_ABC* entity )
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
