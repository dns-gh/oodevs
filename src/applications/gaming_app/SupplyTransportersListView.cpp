// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "SupplyTransportersListView.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/Availability.h"

// -----------------------------------------------------------------------------
// Name: SupplyTransportersListView constructor
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
SupplyTransportersListView::SupplyTransportersListView( QWidget* parent, kernel::Controllers& controllers )
    : ResourcesListView_ABC< SupplyStates >( parent, controllers )
{
    QStringList list;
    list.append( tools::translate( "SupplyTransportersListView", "Transporters" ) );
    list.append( tools::translate( "SupplyTransportersListView", "Total" ) );
    list.append( tools::translate( "SupplyTransportersListView", "Available" ) );
    list.append( tools::translate( "SupplyTransportersListView", "Working" ) );
    list.append( tools::translate( "SupplyTransportersListView", "Resting" ) );
    model_.setHorizontalHeaderLabels( list );
}

// -----------------------------------------------------------------------------
// Name: SupplyTransportersListView destructor
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
SupplyTransportersListView::~SupplyTransportersListView()
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
            if( const SupplyStates* pState = element.Retrieve< SupplyStates >() )
                for( unsigned int i = 0; i < pState->dispoTransporters_.size(); ++i )
                {
                    kernel::Availability curAvailability( 0
                        , pState->dispoTransporters_[ i ].total_
                        , pState->dispoTransporters_[ i ].available_
                        , pState->dispoTransporters_[ i ].atWork_
                        , pState->dispoTransporters_[ i ].atRest_ );
                    availabilities_[ pState->dispoTransporters_[ i ].type_->GetName() ] += curAvailability;
                }
        }
    };
}

// -----------------------------------------------------------------------------
// Name: SupplyTransportersListView::NotifyUpdated
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
void SupplyTransportersListView::NotifyUpdated( const SupplyStates& a )
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
// Name: SupplyTransportersListView::NotifyUpdated
// Created: MMC 2013-01-28
// -----------------------------------------------------------------------------
void SupplyTransportersListView::NotifySelected( const kernel::Entity_ABC* entity )
{
    UpdateSelected( entity );
}

// -----------------------------------------------------------------------------
// Name: SupplyTransportersListView::NotifyUpdated
// Created: MMC 2013-01-28
// -----------------------------------------------------------------------------
void SupplyTransportersListView::UpdateSelected( const kernel::Entity_ABC* entity )
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
