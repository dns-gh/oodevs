// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "MaintenanceRepairersListView.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/Availability.h"

// -----------------------------------------------------------------------------
// Name: MaintenanceRepairersListView constructor
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
MaintenanceRepairersListView::MaintenanceRepairersListView( QWidget* parent, kernel::Controllers& controllers )
    : ResourcesListView_ABC< kernel::MaintenanceStates_ABC >( parent, controllers )
{
    QStringList list;
    list.append( tools::translate( "MaintenanceRepairersListView", "Repairers" ) );
    list.append( tools::translate( "MaintenanceRepairersListView", "Total" ) );
    list.append( tools::translate( "MaintenanceRepairersListView", "Available" ) );
    list.append( tools::translate( "MaintenanceRepairersListView", "Working" ) );
    list.append( tools::translate( "MaintenanceRepairersListView", "Resting" ) );
    model_.setHorizontalHeaderLabels( list );
}

// -----------------------------------------------------------------------------
// Name: MaintenanceRepairersListView destructor
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
MaintenanceRepairersListView::~MaintenanceRepairersListView()
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
                for( unsigned int i = 0; i < pState->GetDispoRepairers().size(); ++i )
                {
                    kernel::Availability curAvailability( 0
                        , pState->GetDispoRepairers()[ i ].total_
                        , pState->GetDispoRepairers()[ i ].available_
                        , pState->GetDispoRepairers()[ i ].atWork_
                        , pState->GetDispoRepairers()[ i ].atRest_ );
                    availabilities_[ pState->GetDispoRepairers()[ i ].type_->GetName() ] += curAvailability;
                }
        }
    };
}

// -----------------------------------------------------------------------------
// Name: MaintenanceRepairersListView::NotifyUpdated
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
void MaintenanceRepairersListView::NotifyUpdated( const kernel::MaintenanceStates_ABC& a )
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
// Name: MaintenanceRepairersListView::NotifyUpdated
// Created: MMC 2013-01-28
// -----------------------------------------------------------------------------
void MaintenanceRepairersListView::NotifySelected( const kernel::Entity_ABC* entity )
{
    UpdateSelected( entity );
}

// -----------------------------------------------------------------------------
// Name: MaintenanceRepairersListView::NotifyUpdated
// Created: MMC 2013-01-28
// -----------------------------------------------------------------------------
void MaintenanceRepairersListView::UpdateSelected( const kernel::Entity_ABC* entity )
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
