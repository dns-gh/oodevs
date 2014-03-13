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
    stockResourcesTable_ = new StockResourcesTable( "stocksTable", this, model );
    maxStockNaturesTable_ = new MaxStockNaturesTable( "maxStocksTable", this, model.objectTypes_ );

    QVBoxLayout* layout = new QVBoxLayout;
    setLayout( layout );

    QSplitter* splitter = new QSplitter( Qt::Vertical, this );
    splitter->setChildrenCollapsible( false );
    splitter->addWidget( stockResourcesTable_ );
    splitter->addWidget( maxStockNaturesTable_ );
    splitter->setStretchFactor( 0, 2 );
    splitter->setStretchFactor( 1, 1 );

    layout->addWidget( splitter );

    connect( stockResourcesTable_, SIGNAL( ResourceValueChanged() ), SLOT( NotifyStocksUserChange() ) );
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
    stockResourcesTable_->OnClearItems();
    maxStockNaturesTable_->UpdateMaxStocks( entity );
    stockResourcesTable_->UpdateAllowedSupplyClasses( entity );
    stockResourcesTable_->UpdateInitStocks( entity );
}

// -----------------------------------------------------------------------------
// Name: StocksAndNaturesEditor::SupplyStocks
// Created: JSR 2014-03-04
// -----------------------------------------------------------------------------
void StocksAndNaturesEditor::SupplyStocks( kernel::Entity_ABC& entity ) const
{
    stockResourcesTable_->SupplyStocks( entity );
}

// -----------------------------------------------------------------------------
// Name: StocksAndNaturesEditor::NotifyAutomaticStocks
// Created: JSR 2014-03-04
// -----------------------------------------------------------------------------
void StocksAndNaturesEditor::NotifyAutomaticStocks( const std::map< const kernel::DotationType*, unsigned int >& stocks )
{
    stockResourcesTable_->UpdateStocks( stocks );
}

// -----------------------------------------------------------------------------
// Name: StocksAndNaturesEditor::NotifyStocksUserChange
// Created: JSR 2014-03-04
// -----------------------------------------------------------------------------
void StocksAndNaturesEditor::NotifyStocksUserChange()
{
    std::map< const kernel::DotationType*, unsigned int > dotations;
    stockResourcesTable_->ComputeValueByDotation( dotations );
    std::set< std::string > allowedNatures;
    maxStockNaturesTable_->Update( dotations, allowedNatures );
    stockResourcesTable_->SetAllowedNatures( allowedNatures );
}
