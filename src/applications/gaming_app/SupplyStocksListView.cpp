// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "SupplyStocksListView.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/StaticModel.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Tools.h"
#include "clients_gui/LogisticHelpers.h"
#include "gaming/Dotation.h"
#include "gaming/Tools.h"
#include <boost/bind.hpp>
#include <vector>

using namespace kernel;

namespace
{
    enum
    {
        eStock,
        eQuantity,
        eNormalizedQuantity,
        eMass,
        eVolume,
    };

    float ComputeNormalizedQuantity( const kernel::StaticModel& staticModel, const kernel::Entity_ABC& logBase, const Dotation& dotation )
    {
        logistic_helpers::T_Requirements requirements;
        logistic_helpers::ComputeLogisticConsumptions( staticModel, logBase, dotation.type_->GetLogisticSupplyClass(), requirements, false );
        const float requirement = static_cast< float >( requirements[ dotation.type_ ] );
        return requirement > 0 ? dotation.quantity_ / requirement : 0;
    }
}

// -----------------------------------------------------------------------------
// Name: SupplyStocksListView constructor
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
SupplyStocksListView::SupplyStocksListView( QWidget* parent, Controllers& controllers, const kernel::StaticModel& staticModel )
    : ResourcesListView_ABC< SupplyStates >( parent, controllers )
    , staticModel_( staticModel )
{
    model_.setHorizontalHeaderLabels(
        QStringList() << tools::translate( "SupplyStocksListView", "Supplies" )
                      << tools::translate( "SupplyStocksListView", "Quantity" )
                      << tools::translate( "SupplyStocksListView", "Normalized quantity" )
                      << tools::translate( "SupplyStocksListView", "Mass (T)" )
                      << tools::translate( "SupplyStocksListView", "Volume (m3)" ) );
}

// -----------------------------------------------------------------------------
// Name: SupplyStocksListView destructor
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
SupplyStocksListView::~SupplyStocksListView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SupplyStocksListView::Update
// Created: MMC 2012-10-02
// -----------------------------------------------------------------------------
void SupplyStocksListView::Update( const tools::Resolver< Dotation >& dotations )
{
    ResizeModelOnNewContent( dotations.Count() );
    int i = 0;
    tools::Iterator< const Dotation& > iterator = dotations.CreateIterator();
    while( iterator.HasMoreElements() )
    {
        const Dotation& dotation = iterator.NextElement();
        model_.item( i, eStock )->setText( QString( dotation.type_->GetName().c_str() ) );
        model_.item( i, eQuantity )->setText( QString::number( dotation.quantity_ ) );
        model_.item( i, eNormalizedQuantity )->setText( locale().toString( ComputeNormalizedQuantity( staticModel_, *selected_, dotation ), 'f', 2 ) );
        model_.item( i, eMass )->setText( locale().toString( dotation.type_->GetUnitWeight() * dotation.quantity_, 'f', 2 ) );
        model_.item( i, eVolume )->setText( locale().toString( dotation.type_->GetUnitVolume() * dotation.quantity_, 'f', 2 ) );
        ++i;
    }
}

// -----------------------------------------------------------------------------
// Name: SupplyStocksListView::NotifyUpdated
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
void SupplyStocksListView::NotifyUpdated( const SupplyStates& supplyStates )
{
    if( !isVisible() )
        return;
    if( !selected_ )
        return;
    if( HasRetrieveForLogistic( *selected_, supplyStates ) )
        UpdateSelected( selected_ );
}

// -----------------------------------------------------------------------------
// Name: SupplyStocksListView::NotifySelected
// Created: MMC 2012-10-01
// -----------------------------------------------------------------------------
void SupplyStocksListView::NotifySelected( const Entity_ABC* entity )
{
    UpdateSelected( entity );
}

// -----------------------------------------------------------------------------
// Name: SupplyStocksListView::NotifySelected
// Created: MMC 2013-01-23
// -----------------------------------------------------------------------------
void SupplyStocksListView::UpdateSelected( const kernel::Entity_ABC* entity )
{
    selected_ = entity;
    if( !entity )
        return;
    dotations_.clear();
    tools::Resolver< Dotation > dotations;
    logistic_helpers::VisitBaseStocksDotations( *entity, [&]( const Dotation& dotation )
    {
        if( !dotation.type_ )
            return;
        Dotation& foundDotation = dotations_[ dotation.type_->GetId() ];
        foundDotation.type_ = dotation.type_;
        foundDotation.quantity_ += dotation.quantity_;
    } );
    for( auto it = dotations_.begin(); it != dotations_.end(); ++it )
        dotations.Register( it->second.type_->GetId(), it->second );
    Update( dotations );
}

// -----------------------------------------------------------------------------
// Name: SupplyStocksListView::showEvent
// Created: LGY 2014-02-10
// -----------------------------------------------------------------------------
void SupplyStocksListView::showEvent( QShowEvent* event )
{
    UpdateSelected( selected_ );
    QTreeView::showEvent( event );
}
