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
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Tools.h"
#include "gaming/Dotation.h"
#include "gaming/Tools.h"
#include <boost/bind.hpp>
#include <vector>

using namespace kernel;
using namespace logistic_helpers;

// -----------------------------------------------------------------------------
// Name: SupplyStocksListView constructor
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
SupplyStocksListView::SupplyStocksListView( QWidget* parent, Controllers& controllers )
    : ResourcesListView_ABC< SupplyStates >( parent, controllers )
{
    QStringList list;
    list.append( tools::translate( "SupplyStocksListView", "Stock" ) );
    list.append( tools::translate( "SupplyStocksListView", "Quantity" ) );
    model_.setHorizontalHeaderLabels( list );
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
        model_.item( i, 0 )->setText( QString( dotation.type_->GetName().c_str() ) );
        model_.item( i, 1 )->setText( QString::number( dotation.quantity_ ) );
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
// Name: InfoSupplyDialog::NotifySelected
// Created: MMC 2012-10-01
// -----------------------------------------------------------------------------
void SupplyStocksListView::NotifySelected( const Entity_ABC* entity )
{
    UpdateSelected( entity );
}

// -----------------------------------------------------------------------------
// Name: InfoSupplyDialog::NotifySelected
// Created: MMC 2013-01-23
// -----------------------------------------------------------------------------
void SupplyStocksListView::UpdateSelected( const kernel::Entity_ABC* entity )
{
    selected_ = entity;
    if( !entity )
        return;
    dotations_.clear();
    tools::Resolver< Dotation > dotations;
    VisitBaseStocksDotations( *entity, boost::bind( &SupplyStocksListView::TotalizeStocks, this, _1 ) );
    for( std::map< unsigned long, Dotation >::iterator it = dotations_.begin(); it != dotations_.end(); ++it )
        dotations.Register( it->second.type_->GetId(), it->second );
    Update( dotations );
}

// -----------------------------------------------------------------------------
// Name: InfoSupplyDialog::TotalizeStocks
// Created: MMC 2012-10-10
// -----------------------------------------------------------------------------
void SupplyStocksListView::TotalizeStocks( const Dotation& dotation )
{
    if( dotation.type_ )
    {
        Dotation& foundDotation = dotations_[ dotation.type_->GetId() ];
        foundDotation.type_ = dotation.type_;
        foundDotation.quantity_ += dotation.quantity_;
    }
}

// -----------------------------------------------------------------------------
// Name: InfoSupplyDialog::showEvent
// Created: LGY 2014-02-10
// -----------------------------------------------------------------------------
void SupplyStocksListView::showEvent( QShowEvent* event )
{
    UpdateSelected( selected_ );
    QTreeView::showEvent( event );
}
