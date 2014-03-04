// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "LogisticStockEditor.h"
#include "moc_LogisticStockEditor.cpp"
#include "clients_gui/CommonDelegate.h"
#include "clients_gui/LogisticBase.h"
#include "clients_gui/LogisticHelpers.h"
#include "clients_gui/LogisticHierarchiesBase.h"
#include "clients_gui/RichPushButton.h"
#include "clients_gui/RichWidget.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/AgentComposition.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/ComponentType.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/DotationCapacityType.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/LogisticSupplyClass.h"
#include "clients_kernel/tools.h"
#include "preparation/Dotation.h"
#include "preparation/LogisticBaseStates.h"
#include "preparation/StaticModel.h"
#include "preparation/Stocks.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: LogisticStockEditor constructor
// Created: MMC 2011-07-21
// -----------------------------------------------------------------------------
LogisticStockEditor::LogisticStockEditor( QWidget* parent, Controllers& controllers, const ::StaticModel& staticModel )
    : LogisticEditor( parent, "LogisticStockEditor", controllers, staticModel )
{
    setCaption( tools::translate( "StocksEditionDialog", "Stocks Edition" ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticStockEditor destructor
// Created: MMC 2011-07-21
// -----------------------------------------------------------------------------
LogisticStockEditor::~LogisticStockEditor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticStockEditor::SupplyHierarchy
// Created: SLI 2014-02-19
// -----------------------------------------------------------------------------
void LogisticStockEditor::SupplyHierarchy( const kernel::Entity_ABC& entity, const gui::LogisticHierarchiesBase& logHierarchy )
{
    std::set< const Agent_ABC* > entStocks;
    FindStocks( entity, entity, entStocks );
    T_Requirements requirements;
    auto itLogClass = staticModel_.objectTypes_.tools::StringResolver< kernel::LogisticSupplyClass >::CreateIterator();
    for( int row = 0; itLogClass.HasMoreElements(); ++row )
    {
        const kernel::LogisticSupplyClass& supplyClass = itLogClass.NextElement();
        if( dataModel_->item( row )->checkState() == Qt::Checked )
        {
            T_Requirements curRequirements;
            SupplyLogisticBaseStocks( logHierarchy.GetEntity(), supplyClass, curRequirements );
            for( auto itReq = curRequirements.begin(); itReq != curRequirements.end(); ++itReq )
                requirements[ itReq->first ] += GetQuantity( *dataModel_, row, itReq->second );
        }
    }
    emit DotationsStocksComputed( requirements );}

// -----------------------------------------------------------------------------
// Name: LogisticStockEditor::FindStocks
// Created: MMC 2011-08-31
// -----------------------------------------------------------------------------
void LogisticStockEditor::FindStocks( const Entity_ABC& rootEntity , const Entity_ABC& entity, std::set< const Agent_ABC* >& entStocks )
{
    const TacticalHierarchies* pTacticalHierarchies = entity.Retrieve< TacticalHierarchies >();
    if( !pTacticalHierarchies )
        return;

    if( entity.GetId() != rootEntity.GetId() && logistic_helpers::IsLogisticBase( entity ) )
        return;

    auto children = pTacticalHierarchies->CreateSubordinateIterator();
    while( children.HasMoreElements() )
    {
        const Entity_ABC& childrenEntity = children.NextElement();
        const Agent_ABC* pAgent = dynamic_cast< const Agent_ABC* >( &childrenEntity );
        if( pAgent )
        {
            if( pAgent->GetType().IsLogisticSupply() )
            {
                Stocks* stocks = const_cast< Stocks* >( pAgent->Retrieve< Stocks >() );
                if( stocks )
                    entStocks.insert( pAgent );
            }
        }
        else
            FindStocks( rootEntity, childrenEntity, entStocks );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticStockEditor::DoDotationDistribution
// Created: MMC 2012-02-17
// -----------------------------------------------------------------------------
double LogisticStockEditor::DoDotationDistribution( std::set< const Agent_ABC* >& entStocks, const DotationType& dotationType, double quantity )
{
    if( entStocks.empty() || quantity < entStocks.size() )
        return quantity;

    int quantityMean = static_cast< int >( quantity / entStocks.size() );

    std::set< const Agent_ABC* > toErase;
    for( auto itEnt = entStocks.begin(); itEnt != entStocks.end(); ++itEnt )
    {
        const Agent_ABC& entStock = **itEnt;
        double weightCapacity, volumeCapacity;
        ComputeAvailableCapacity( entStock, dotationType, weightCapacity, volumeCapacity );
        double dotationWeight = quantityMean * dotationType.GetUnitWeight();
        double dotationVolume = quantityMean * dotationType.GetUnitVolume();
        if( dotationWeight > 0 && dotationVolume > 0 && ( dotationWeight >= weightCapacity || dotationVolume >= volumeCapacity ) )
        {
            Stocks& stocks = const_cast< Stocks& >( entStock.Get< Stocks >() );
            int quantitySupply = static_cast< int >( std::min( weightCapacity / dotationType.GetUnitWeight(), volumeCapacity / dotationType.GetUnitVolume() ) );
            stocks.SetDotation( dotationType, quantitySupply, true );
            toErase.insert( *itEnt );
            quantity -= quantitySupply;
        }
    }

    if( toErase.empty() )
    {
        for( auto itEnt = entStocks.begin(); itEnt != entStocks.end(); ++itEnt )
        {
            const Agent_ABC& entStock = **itEnt;
            Stocks& stocks = const_cast< Stocks& >( entStock.Get< Stocks >() );
            stocks.SetDotation( dotationType, quantityMean, true );
            quantity -= quantityMean;
        }
        return quantity;
    }

    for( auto itErase = toErase.begin(); itErase != toErase.end(); ++itErase )
        entStocks.erase( *itErase );

    return DoDotationDistribution( entStocks, dotationType, quantity );
};

// -----------------------------------------------------------------------------
// Name: LogisticStockEditor::ComputeAvailableCapacity
// Created: MMC 2012-02-17
// -----------------------------------------------------------------------------
void LogisticStockEditor::ComputeAvailableCapacity( const Agent_ABC& entStock, const DotationType& dotationType, double& weight, double& volume )
{
    weight = volume = 0.;
    double weightCapacity = 0., volumeCapacity = 0.;
    auto itComposition = entStock.GetType().CreateIterator();
    while( itComposition.HasMoreElements() )
    {
        const AgentComposition& agentComposition = itComposition.NextElement();
        const ComponentType& equipment = agentComposition.GetType();
        const EquipmentType& equipmentType = staticModel_.objectTypes_.Resolver2< EquipmentType >::Get( equipment.GetId() );
        if( const EquipmentType::CarryingSupplyFunction* carrying = equipmentType.GetLogSupplyFunctionCarrying() )
            if( carrying->stockNature_ == dotationType.GetNature() )
            {
                const unsigned int nEquipments = agentComposition. GetCount();
                weightCapacity += nEquipments * carrying->stockMaxWeightCapacity_;
                volumeCapacity += nEquipments * carrying->stockMaxVolumeCapacity_;
            }
    }
    Stocks& stocks = const_cast< Stocks& >( entStock.Get< Stocks >() );
    double stockWeight;
    double stockVolume;
    stocks.ComputeWeightAndVolume( dotationType.GetNature(), stockWeight, stockVolume );
    weight = std::max( weightCapacity - stockWeight, 0. );
    volume = std::max( volumeCapacity - stockVolume, 0. );
}
