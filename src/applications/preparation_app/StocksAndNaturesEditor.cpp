// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "StocksAndNaturesEditor.h"
#include "moc_StocksAndNaturesEditor.cpp"
#include "StockResourcesTable.h"
#include "MaxStockNaturesTable.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/ObjectTypes.h"
#include "preparation/StaticModel.h"

// -----------------------------------------------------------------------------
// Name: StocksAndNaturesEditor constructor
// Created: JSR 2014-03-04
// -----------------------------------------------------------------------------
StocksAndNaturesEditor::StocksAndNaturesEditor( QWidget* parent, const StaticModel& model )
    : QWidget( parent )
{
    stocksTableView_ = new StockResourcesTable( "stocksTable", this, model );
    maxStocksTableView_ = new MaxStockNaturesTable( "maxStocksTable", this, model.objectTypes_ );

    QVBoxLayout* layout = new QVBoxLayout;
    setLayout( layout );
    layout->addWidget( stocksTableView_ );
    layout->addWidget( maxStocksTableView_ );
    layout->setStretch( 0, 2 );
    layout->setStretch( 1, 1 );

    connect( stocksTableView_, SIGNAL( ResourceValueChanged() ), SLOT( NotifyStocksUserChange() ) );
}

// -----------------------------------------------------------------------------
// Name: StocksAndNaturesEditor destructor
// Created: JSR 2014-03-04
// -----------------------------------------------------------------------------
StocksAndNaturesEditor::~StocksAndNaturesEditor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: StocksAndNaturesEditor::Initialize
// Created: JSR 2014-03-05
// -----------------------------------------------------------------------------
void StocksAndNaturesEditor::Initialize( const kernel::Entity_ABC& entity )
{
    stocksTableView_->OnClearItems();
    maxStocksTableView_->UpdateMaxStocks( entity );
    stocksTableView_->UpdateInitStocks( entity );
}

// -----------------------------------------------------------------------------
// Name: StocksAndNaturesEditor::Update
// Created: JSR 2014-03-04
// -----------------------------------------------------------------------------
void StocksAndNaturesEditor::Update( const MaxStockNaturesTable::T_WeightVolumes& currentValues )
{
    std::set< std::string > allowedNatures;
    maxStocksTableView_->Update( currentValues, allowedNatures );
    stocksTableView_->SetAllowedNatures( allowedNatures );
}

// -----------------------------------------------------------------------------
// Name: StocksAndNaturesEditor::SupplyStocks
// Created: JSR 2014-03-04
// -----------------------------------------------------------------------------
void StocksAndNaturesEditor::SupplyStocks( kernel::Entity_ABC& entity ) const
{
    stocksTableView_->SupplyStocks( entity );
}

// -----------------------------------------------------------------------------
// Name: StocksAndNaturesEditor::NotifyAutomaticStocks
// Created: JSR 2014-03-04
// -----------------------------------------------------------------------------
void StocksAndNaturesEditor::NotifyAutomaticStocks( const LogisticEditor::T_Requirements& stocks )
{
    stocksTableView_->UpdateStocks( stocks );
}

// -----------------------------------------------------------------------------
// Name: StocksAndNaturesEditor::NotifyStocksUserChange
// Created: JSR 2014-03-04
// -----------------------------------------------------------------------------
void StocksAndNaturesEditor::NotifyStocksUserChange()
{
    std::map< std::string, MaxStockNaturesTable::WeightVolume > valuesByNature;
    ComputeStocksByNature( valuesByNature );
    Update( valuesByNature );
}

// -----------------------------------------------------------------------------
// Name: StocksAndNaturesEditor::ComputeStocksByNature
// Created: JSR 2014-03-04
// -----------------------------------------------------------------------------
void StocksAndNaturesEditor::ComputeStocksByNature( std::map< std::string, MaxStockNaturesTable::WeightVolume >& result ) const
{
    // todo mettre dans stocksTableView_
    LogisticEditor::T_Requirements dotations;
    stocksTableView_->ComputeValueByDotation( dotations );
    for( auto it = dotations.begin(); it != dotations.end(); ++it )
    {
        const auto& curDotation = *it->first;
        double value = it->second;
        result[ curDotation.GetNature() ].weight_ += value * curDotation.GetUnitWeight();
        result[ curDotation.GetNature() ].volume_ += value * curDotation.GetUnitVolume();
    }
}
