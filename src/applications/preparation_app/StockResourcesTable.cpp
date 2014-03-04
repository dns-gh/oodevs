// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "StockResourcesTable.h"
#include "moc_StockResourcesTable.cpp"
#include "clients_gui/CommonDelegate.h"
#include "clients_gui/LogisticHelpers.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/AgentComposition.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/ComponentType.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/LogisticSupplyClass.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Tools.h"
#include "preparation/Dotation.h"
#include "preparation/Stocks.h"
#include "preparation/StaticModel.h"

namespace
{
    QStringList GetHeaders()
    {
        return QStringList() << tools::translate( "StockResourcesTable", "Resource" )
                             << tools::translate( "StockResourcesTable", "Nature" )
                             << tools::translate( "StockResourcesTable", "Weight (T)" )
                             << tools::translate( "StockResourcesTable", "Volume (m3)" )
                             << tools::translate( "StockResourcesTable", "Quantity" );
    }
}

// -----------------------------------------------------------------------------
// Name: StockResourcesTable Constructor
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
StockResourcesTable::StockResourcesTable( const QString& objectName, QWidget* parent, const StaticModel& staticModel ) 
    : ResourcesEditorTable_ABC( GetHeaders(), objectName, parent, staticModel.objectTypes_ )
    , staticModel_( staticModel )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: StockResourcesTable Destructor
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
StockResourcesTable::~StockResourcesTable()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: StockResourcesTable::UpdateLine
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void StockResourcesTable::UpdateLine( int row, int value )
{
    const kernel::DotationType* pDotation = GetDotation( row );
    SetData( row, 2, QString::number( value * pDotation->GetUnitWeight(), 'f', 2 ) );
    SetData( row, 3, QString::number( value * pDotation->GetUnitVolume(), 'f', 2 ) );
}

namespace
{
    void FindStocks( const kernel::Entity_ABC& rootEntity , const kernel::Entity_ABC& entity, std::vector< const kernel::Agent_ABC* >& entStocks )
    {
        const kernel::TacticalHierarchies* pTacticalHierarchies = entity.Retrieve< kernel::TacticalHierarchies >();
        if( !pTacticalHierarchies )
            return;
        if( entity.GetId() != rootEntity.GetId() && logistic_helpers::IsLogisticBase( entity ) )
            return;
        auto child = pTacticalHierarchies->CreateSubordinateIterator();
        while( child.HasMoreElements() )
        {
            const kernel::Entity_ABC& childEntity = child.NextElement();
            if( const kernel::Agent_ABC* pAgent = dynamic_cast< const kernel::Agent_ABC* >( &childEntity ) )
            {
                if( pAgent->GetType().IsLogisticSupply() )
                {
                    Stocks* stocks = const_cast< Stocks* >( pAgent->Retrieve< Stocks >() );
                    if( stocks && ( std::find( entStocks.begin(), entStocks.end(), pAgent ) == entStocks.end() ) )
                        entStocks.push_back( pAgent );
                }
            }
            else
                FindStocks( rootEntity, childEntity, entStocks );
        }
    }

    void ComputeInitStocks( const kernel::Entity_ABC& entity, LogisticEditor::T_Requirements& requirements )
    {
        std::vector< const kernel::Agent_ABC* > stocks;
        if( auto pAgent = dynamic_cast< const kernel::Agent_ABC* >( &entity ) )
        {
            if( pAgent->GetType().IsLogisticSupply() && const_cast< Stocks* >( pAgent->Retrieve< Stocks >() ) )
                stocks.push_back( pAgent );
        }
        else
            FindStocks( entity , entity, stocks );

        for( auto it = stocks.begin(); it != stocks.end(); ++it )
        {
            const auto& curStock = **it;
            auto pStocks = const_cast< Stocks* >( curStock.Retrieve< Stocks >() );
            if( !pStocks )
                continue;
            auto dotationIt = pStocks->CreateIterator();
            while( dotationIt.HasMoreElements() )
            {
                const auto& curDotation = dotationIt.NextElement();
                requirements[ &curDotation.type_ ] += curDotation.quantity_;
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: StockResourcesTable::UpdateInitStocks
// Created: JSR 2014-03-04
// -----------------------------------------------------------------------------
void StockResourcesTable::UpdateInitStocks( const kernel::Entity_ABC& entity )
{
    LogisticEditor::T_Requirements initialStocks;
    ComputeInitStocks( entity, initialStocks );
    UpdateStocks( initialStocks );
}

// -----------------------------------------------------------------------------
// Name: StockResourcesTable::UpdateStocks
// Created: JSR 2014-03-04
// -----------------------------------------------------------------------------
void StockResourcesTable::UpdateStocks( const LogisticEditor::T_Requirements& stocks )
{
    Disconnect();
    OnClearItems();
    for( auto it = stocks.begin(); it != stocks.end(); ++it )
        AddResource( *it->first, it->second );
    emit ResourceValueChanged();
    Connect();
}


// -----------------------------------------------------------------------------
// Name: StockResourcesTable::CleanStocks
// Created: JSR 2014-03-04
// -----------------------------------------------------------------------------
void StockResourcesTable::CleanStocks( std::vector< const kernel::Agent_ABC* >& entStocks ) const
{
    const tools::StringResolver< kernel::LogisticSupplyClass >& supplyClasses = staticModel_.objectTypes_;

    for( auto it = entStocks.begin(); it != entStocks.end(); ++it )
    {
        Stocks& stocks = const_cast< Stocks& >( ( *it )->Get< Stocks >() );
        std::vector< const kernel::DotationType* > toReset;
        auto itDotation = stocks.CreateIterator();

        while( itDotation.HasMoreElements() )
        {
            const Dotation& curDotation = itDotation.NextElement();
            auto itLogClass = supplyClasses.CreateIterator();
            while( itLogClass.HasMoreElements() )
            {
                const kernel::LogisticSupplyClass& supplyClass = itLogClass.NextElement();
                if( supplyClass.GetId() == curDotation.type_.GetLogisticSupplyClass().GetId() )
                    break;
            }
            toReset.push_back( &curDotation.type_ );
        }
        for( std::vector< const kernel::DotationType* >::iterator it = toReset.begin(); it!= toReset.end(); ++it )
            stocks.SetDotation( **it, 0, false );
    }
}

// -----------------------------------------------------------------------------
// Name: StockResourcesTable::IsStockValid
// Created: JSR 2014-03-04
// -----------------------------------------------------------------------------
bool StockResourcesTable::IsStockValid( const kernel::Agent_ABC& stockUnit, const kernel::DotationType& dotation ) const
{
    const tools::Resolver< kernel::AgentType >& agentTypes = staticModel_.types_;
    const kernel::AgentType& agentType = agentTypes.Get( stockUnit.GetType().GetId() );
    return agentType.IsStockCategoryDefined( dotation.GetLogisticSupplyClass() );
}

// -----------------------------------------------------------------------------
// Name: StockResourcesTable::ComputeStockWeightVolumeLeft
// Created: JSR 2014-03-04
// -----------------------------------------------------------------------------
void StockResourcesTable::ComputeStockWeightVolumeLeft( const kernel::Agent_ABC& stockUnit, std::string nature, MaxStockNaturesTable::WeightVolume& result ) const
{
    const kernel::Resolver2< kernel::EquipmentType >& equipments = staticModel_.objectTypes_;
    if( !stockUnit.GetType().IsLogisticSupply() )
        return;
    auto pStocks = stockUnit.Retrieve< Stocks >();
    if( !pStocks )
        return;
    MaxStockNaturesTable::WeightVolume maxCapacity;
    tools::Iterator< const kernel::AgentComposition& > itComposition = stockUnit.GetType().CreateIterator();
    while( itComposition.HasMoreElements() )
    {
        const kernel::AgentComposition& agentComposition = itComposition.NextElement();
        const kernel::ComponentType& equipment = agentComposition.GetType();
        const kernel::EquipmentType& equipmentType = equipments.Get( equipment.GetId() );
        if( const kernel::EquipmentType::CarryingSupplyFunction* carrying = equipmentType.GetLogSupplyFunctionCarrying() )
        {
            unsigned int nEquipments = agentComposition. GetCount();
            if( nature == carrying->stockNature_ )
            {
                maxCapacity.weight_ += nEquipments * carrying->stockMaxWeightCapacity_;
                maxCapacity.volume_ += nEquipments * carrying->stockMaxVolumeCapacity_;
            }
        }
    }
    MaxStockNaturesTable::WeightVolume usedCapacity;
    double weightUsed = 0;
    double volumeUsed = 0;
    pStocks->ComputeWeightAndVolume( nature, weightUsed, volumeUsed );
    if( weightUsed < maxCapacity.weight_ )
        result.weight_ = maxCapacity.weight_ - weightUsed;
    if( volumeUsed < maxCapacity.volume_ )
        result.volume_ = maxCapacity.volume_ - volumeUsed;
}

// -----------------------------------------------------------------------------
// Name: StockResourcesTable::SupplyStocks
// Created: JSR 2014-03-04
// -----------------------------------------------------------------------------
void StockResourcesTable::SupplyStocks( kernel::Entity_ABC& entity ) const
{
    // TODO CLEAN!!!

    LogisticEditor::T_Requirements requirements;
    ComputeValueByDotation( requirements );

    if( auto pAgent = dynamic_cast< const kernel::Agent_ABC* >( &entity ) )
    {
        std::vector< const kernel::Agent_ABC* > entStock;
        entStock.push_back( pAgent );
        CleanStocks( entStock );
        for( auto itRequired = requirements.begin(); itRequired != requirements.end(); ++itRequired )
        {
            const kernel::DotationType& dotationType = *itRequired->first;
            if( IsStockValid( *pAgent, dotationType ) )
                if( auto pStocks = const_cast< Stocks* >( pAgent->Retrieve< Stocks >() ) )
                    pStocks->SetDotation( dotationType, itRequired->second, true );
        }
    }
    else
    {
        std::vector< const kernel::Agent_ABC* > stocks;
        FindStocks( entity , entity, stocks );
        CleanStocks( stocks );

        LogisticEditor::T_Requirements dotationToStocks( requirements );
        for( auto itRequired = dotationToStocks.begin(); itRequired != dotationToStocks.end(); ++itRequired )
        {
            const kernel::DotationType& dotationType = *itRequired->first;
            for( auto it = stocks.begin(); it != stocks.end(); ++it )
            {
                if( itRequired->second <= 0 )
                    break;
                const auto& curStock = **it;
                auto pStocks = const_cast< Stocks* >( curStock.Retrieve< Stocks >() );
                if( !pStocks )
                    continue;
                if( !IsStockValid( curStock, dotationType ) )
                    continue;
                MaxStockNaturesTable::WeightVolume stockLeft;
                ComputeStockWeightVolumeLeft( **it, dotationType.GetNature(), stockLeft );
                unsigned int maxQuantityForWeight = itRequired->second;
                if( dotationType.GetUnitWeight() > 0 )
                    maxQuantityForWeight = static_cast< unsigned int >( stockLeft.weight_ / dotationType.GetUnitWeight() );
                unsigned int maxQuantityForVolume = itRequired->second;
                if( dotationType.GetUnitVolume() > 0 )
                    maxQuantityForVolume = static_cast< unsigned int >( stockLeft.volume_ / dotationType.GetUnitVolume() );
                unsigned int q = std::min( itRequired->second, std::min( maxQuantityForWeight, maxQuantityForVolume ) );
                pStocks->SetDotation( dotationType, q, true );
                itRequired->second = std::max( 0u, itRequired->second - q );
            }
        }

        bool bDotationsToStockLeft = false;
        LogisticEditor::T_Requirements stocksByDotation;
        for( auto itRequired = dotationToStocks.begin(); itRequired != dotationToStocks.end(); ++itRequired )
        {
            const kernel::DotationType& dotationType = *itRequired->first;
            if( itRequired->second > 0 )
                bDotationsToStockLeft = true;
            for( auto it = stocks.begin(); it != stocks.end(); ++it )
                if( IsStockValid( **it, dotationType ) )
                    ++stocksByDotation[ &dotationType ];
        }
        if( !bDotationsToStockLeft )
            return;

        LogisticEditor::T_Requirements meansDotationsByStock;
        for( auto it = stocksByDotation.begin(); it != stocksByDotation.end(); ++it )
        {
            double quantityLeft = dotationToStocks[ it->first ];
            double stocksCount = it->second;
            if( quantityLeft > 0 && stocksCount > 0 )
                meansDotationsByStock[ it->first ] = static_cast< unsigned int >( quantityLeft / stocksCount );
        }

        for( auto itRequired = dotationToStocks.begin(); itRequired != dotationToStocks.end(); ++itRequired )
        {
            const kernel::DotationType& dotationType = *itRequired->first;
            auto itMean = meansDotationsByStock.find( &dotationType );
            if( itMean == meansDotationsByStock.end() )
                continue;
            double meanByStock = itMean->second;
            for( auto it = stocks.begin(); it != stocks.end(); ++it )
            {
                if( itRequired->second <= 0 )
                    continue;
                const auto& curStock = **it;
                if( !IsStockValid( curStock, dotationType ) )
                    continue;
                if( auto pStocks = const_cast< Stocks* >( curStock.Retrieve< Stocks >() ) )
                {
                    int quantityToStock = static_cast< int >( meanByStock + 0.5 );
                    pStocks->SetDotation( dotationType, quantityToStock, true );
                    itRequired->second = std::max( 0u,  itRequired->second - quantityToStock );
                }
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: StockResourcesTable::AddResource
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void StockResourcesTable::AddResource( const kernel::DotationType& resource, int value /*= 0.0*/ )
{
    ResourcesEditorTable_ABC::AddResource( resource, value );
    const int rowIndex = model()->rowCount() - 1;
    SetData( rowIndex, 1 , QString::fromStdString( resource.GetNature() ) );
    SetData( rowIndex, 2, QString::number( value * resource.GetUnitWeight(), 'f', 2 ), Qt::DisplayRole, Qt::AlignRight | Qt::AlignVCenter );
    SetData( rowIndex, 3, QString::number( value * resource.GetUnitVolume(), 'f', 2 ), Qt::DisplayRole, Qt::AlignRight | Qt::AlignVCenter );
}

// -----------------------------------------------------------------------------
// Name: StockResourcesTable::CustomizeMenuAction
// Created: JSR 2014-03-04
// -----------------------------------------------------------------------------
void StockResourcesTable::CustomizeMenuAction( QAction* action, const kernel::DotationType& actionDotation ) const
{
    if( allowedNatures_.find( actionDotation.GetNature() ) == allowedNatures_.end() )
    {
        QFont font = action->font();
        font.setItalic( true );
        action->setFont( font );
    }
}

// -----------------------------------------------------------------------------
// Name: StockResourcesTable::SetAllowedNatures
// Created: JSR 2014-03-04
// -----------------------------------------------------------------------------
void StockResourcesTable::SetAllowedNatures( const std::set< std::string >& allowedNatures )
{
    allowedNatures_ = allowedNatures;
}
