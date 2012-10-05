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
#include "clients_kernel/EntityHelpers.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "gaming/Dotation.h"
#include "gaming/Tools.h"
#include <vector>

using namespace kernel;
using namespace EntityHelpers;

namespace
{
    std::vector< Dotation > displayDotations;

    void TotalizeStocks( const Entity_ABC& entity, std::vector< Dotation >& dotations )
    {
        if( entity.Retrieve< SupplyStates >() )
        {
            tools::Iterator< const Dotation& > it = entity.Get< SupplyStates >().CreateIterator();
            while( it.HasMoreElements() )
            {
                const Dotation& curDotation = it.NextElement();
                if( curDotation.type_ )
                {
                    bool dotationFound = false;
                    for( std::size_t i = 0; i < dotations.size(); ++i )
                        if( dotations[ i ].type_->GetId() == curDotation.type_->GetId() )
                        {
                            dotations[ i ].quantity_ += curDotation.quantity_;
                            dotationFound = true;
                            break;
                        }
                    if( !dotationFound )
                        dotations.push_back( Dotation( *curDotation.type_, curDotation.quantity_ ) );
                }
            }
            return;
        }

        if( entity.Retrieve< TacticalHierarchies >() )
        {
            tools::Iterator< const Entity_ABC& > tacticalChildIt = entity.Get< TacticalHierarchies >().CreateSubordinateIterator();
            while( tacticalChildIt.HasMoreElements() )
            {
                const Entity_ABC& tacticalChild = tacticalChildIt.NextElement();
                if( !IsLogisticBase( tacticalChild ) )
                    TotalizeStocks( tacticalChild, dotations );
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: SupplyStocksListView constructor
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
SupplyStocksListView::SupplyStocksListView( QWidget* parent, Controllers& controllers, gui::ItemFactory_ABC& factory )
    : SupplyAvailabilitiesListView_ABC( parent, controllers, factory )
{
    AddColumn( tools::translate( "SupplyStocksListView", "Stock" ) )
    .AddColumn( tools::translate( "SupplyStocksListView", "Quantity" ) );
    setColumnWidth( 0, 140 );
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
// Name: SupplyStocksListView::NotifyUpdated
// Created: MMC 2012-10-02
// -----------------------------------------------------------------------------
void SupplyStocksListView::NotifyUpdated( const tools::Resolver< Dotation >& dotations )
{
    DeleteTail( DisplayList( dotations.CreateIterator() ) );
}

// -----------------------------------------------------------------------------
// Name: SupplyStocksListView::NotifyUpdated
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
void SupplyStocksListView::NotifyUpdated( const SupplyStates& supplyStates )
{
    if( ShouldUpdate( supplyStates ) )
        DeleteTail( DisplayList( supplyStates.CreateIterator() ) );
}

// -----------------------------------------------------------------------------
// Name: InfoSupplyDialog::NotifySelected
// Created: MMC 2012-10-01
// -----------------------------------------------------------------------------
void SupplyStocksListView::NotifySelected( const Entity_ABC* entity )
{
    displayDotations.clear();
    if( entity )
    {
        if( entity->Retrieve< SupplyStates >() )
            NotifyUpdated( entity->Get< SupplyStates >() );
        else if( IsLogisticBase( *entity ) )
        {
            TotalizeStocks( *entity, displayDotations );
            tools::Resolver< Dotation > dotations;
            for( std::size_t i = 0; i < displayDotations.size(); ++i )
                dotations.Register( displayDotations[ i ].type_->GetId(), displayDotations[ i ] );
            NotifyUpdated( dotations );
        }
        show();
    }
    else
        hide();
    SupplyAvailabilitiesListView_ABC::UpdateSelected( entity );
}
