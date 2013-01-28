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
    struct DispoRepairers
    {
        unsigned int total_;
        unsigned int available_;
        unsigned int atWork_;
        unsigned int atRest_;

        DispoRepairers(): total_(0), available_(0), atWork_(0), atRest_(0) {}
        DispoRepairers( int total, int available, int atWork, int atRest )
            : total_( total ), available_( available ), atWork_( atWork ), atRest_( atRest ) {}

        void operator+=( const DispoRepairers& other )
        { 
            total_ += other.total_;
            available_ += other.available_;
            atWork_+= other.atWork_;
            atRest_+= other.atRest_;
        }
    };
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
