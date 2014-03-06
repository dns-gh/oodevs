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
void StocksAndNaturesEditor::NotifyAutomaticStocks( const std::map< const kernel::DotationType*, unsigned int >& stocks )
{
    stocksTableView_->UpdateStocks( stocks );
}

// -----------------------------------------------------------------------------
// Name: StocksAndNaturesEditor::NotifyStocksUserChange
// Created: JSR 2014-03-04
// -----------------------------------------------------------------------------
void StocksAndNaturesEditor::NotifyStocksUserChange()
{
    std::map< const kernel::DotationType*, unsigned int > dotations;
    stocksTableView_->ComputeValueByDotation( dotations );
    std::set< std::string > allowedNatures;
    maxStocksTableView_->Update( dotations, allowedNatures );
    stocksTableView_->SetAllowedNatures( allowedNatures );
}
