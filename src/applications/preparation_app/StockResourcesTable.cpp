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
#include "clients_kernel/ObjectTypes.h"
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
    SetData( row, 2, QString::number( value * pDotation->GetUnitWeight(), 'f', 2 ), Qt::DisplayRole, true );
    SetData( row, 3, QString::number( value * pDotation->GetUnitVolume(), 'f', 2 ), Qt::DisplayRole, true );
}

namespace
{
    void ComputeInitStocks( const kernel::Entity_ABC& entity, std::map< const kernel::DotationType*, unsigned int >& stocks )
    {
        logistic_helpers::VisitAgentsWithLogisticSupply( entity, [&]( const kernel::Agent_ABC& agent )
        {
            auto dotationIt = agent.Get< Stocks >().CreateIterator();
            while( dotationIt.HasMoreElements() )
            {
                const auto& curDotation = dotationIt.NextElement();
                stocks[ &curDotation.type_ ] += curDotation.quantity_;
            }
        } );
    }
}

// -----------------------------------------------------------------------------
// Name: StockResourcesTable::UpdateInitStocks
// Created: JSR 2014-03-04
// -----------------------------------------------------------------------------
void StockResourcesTable::UpdateInitStocks( const kernel::Entity_ABC& entity )
{
    std::map< const kernel::DotationType*, unsigned int > initialStocks;
    ComputeInitStocks( entity, initialStocks );
    UpdateStocks( initialStocks );
}

// -----------------------------------------------------------------------------
// Name: StockResourcesTable::UpdateStocks
// Created: JSR 2014-03-04
// -----------------------------------------------------------------------------
void StockResourcesTable::UpdateStocks( const std::map< const kernel::DotationType*, unsigned int >& stocks )
{
    Disconnect();
    OnClearItems();
    for( auto it = stocks.begin(); it != stocks.end(); ++it )
        AddResource( *it->first, it->second );
    emit ResourceValueChanged();
    Connect();
}

// -----------------------------------------------------------------------------
// Name: StockResourcesTable::IsStockValid
// Created: JSR 2014-03-04
// -----------------------------------------------------------------------------
bool StockResourcesTable::IsStockValid( const kernel::Agent_ABC& agent, const kernel::DotationType& dotation ) const
{
    if( agent.Retrieve< Stocks> () == 0 )
        return false;
    const tools::Resolver< kernel::AgentType >& agentTypes = staticModel_.types_;
    const kernel::AgentType& agentType = agentTypes.Get( agent.GetType().GetId() );
    return agentType.IsStockCategoryDefined( dotation.GetLogisticSupplyClass() );
}

// -----------------------------------------------------------------------------
// Name: StockResourcesTable::ComputeStockWeightVolumeLeft
// Created: JSR 2014-03-04
// -----------------------------------------------------------------------------
void StockResourcesTable::ComputeStockWeightVolumeLeft( const kernel::Agent_ABC& agent, const std::string& nature, double& weightResult, double& volumeResult ) const
{
    const kernel::Resolver2< kernel::EquipmentType >& equipments = staticModel_.objectTypes_;
    double maxWeight = 0;
    double maxVolume = 0;
    tools::Iterator< const kernel::AgentComposition& > itComposition = agent.GetType().CreateIterator();
    while( itComposition.HasMoreElements() )
    {
        const kernel::AgentComposition& agentComposition = itComposition.NextElement();
        const kernel::EquipmentType& equipmentType = equipments.Get( agentComposition.GetType().GetId() );
        auto carrying = equipmentType.GetLogSupplyFunctionCarrying();
        if( carrying && carrying->stockNature_ == nature )
        {
            const unsigned int nEquipments = agentComposition. GetCount();
            maxWeight += nEquipments * carrying->stockMaxWeightCapacity_;
            maxVolume += nEquipments * carrying->stockMaxVolumeCapacity_;
        }
    }
    double weightUsed = 0;
    double volumeUsed = 0;
    agent.Get< Stocks >().ComputeWeightAndVolume( nature, weightUsed, volumeUsed );
    weightResult = std::max( 0., maxWeight - weightUsed );
    volumeResult = std::max( 0., maxVolume - volumeUsed );
}

// -----------------------------------------------------------------------------
// Name: StockResourcesTable::FillStockUntilMaxReached
// Created: JSR 2014-03-06
// -----------------------------------------------------------------------------
unsigned int StockResourcesTable::FillStockUntilMaxReached( kernel::Agent_ABC& agent, const kernel::DotationType& dotationType, unsigned int quantity ) const
{
    double stockLeftWeight = 0;
    double stockLeftVolume = 0;
    ComputeStockWeightVolumeLeft( agent, dotationType.GetNature(), stockLeftWeight, stockLeftVolume );

    unsigned int distributed = quantity;
    if( dotationType.GetUnitWeight() > 0 )
    {
        double notUsed = stockLeftWeight / dotationType.GetUnitWeight();
        distributed = std::min( distributed, static_cast< unsigned int >( notUsed ) );
    }
    if( dotationType.GetUnitVolume() > 0 )
    {
        double notUsed = stockLeftVolume / dotationType.GetUnitVolume();
        distributed = std::min( distributed, static_cast< unsigned int >( notUsed ) );
    }
    agent.Get< Stocks >().SetDotation( dotationType, distributed, true );
    return distributed;
}

// -----------------------------------------------------------------------------
// Name: StockResourcesTable::SupplyStocks
// Created: JSR 2014-03-04
// -----------------------------------------------------------------------------
void StockResourcesTable::SupplyStocks( kernel::Entity_ABC& entity ) const
{
    std::map< const kernel::DotationType*, unsigned int > dotations;
    ComputeValueByDotation( dotations );

    logistic_helpers::VisitAgentsWithLogisticSupply( entity, []( const kernel::Agent_ABC& agent )
        {
            const_cast< kernel::Agent_ABC& >( agent ).Get< Stocks >().DeleteAll();
        } );

    for( auto itDotation = dotations.begin(); itDotation != dotations.end(); ++itDotation )
    {
        unsigned int total = itDotation->second;
        if( total <= 0 )
            continue;

        const kernel::DotationType& dotationType = *itDotation->first;
        std::vector< kernel::Agent_ABC* > validAgents;
        logistic_helpers::VisitAgentsWithLogisticSupply( entity, [&]( const kernel::Agent_ABC& agent )
            {
                if( IsStockValid( agent, dotationType ) )
                    validAgents.push_back( const_cast< kernel::Agent_ABC* >( &agent ) );
            } );

        const unsigned int nbrAgents = static_cast< unsigned int >( validAgents.size() );
        if( nbrAgents == 0 )
            continue;

        // fill stocks until maximum weight or volume is reached
        for( auto it = validAgents.begin(); it != validAgents.end() && total > 0; ++it )
            total -= FillStockUntilMaxReached( **it, dotationType, total );
        if( total <= 0 )
            continue;

        // fill remaining resources
        const unsigned int mean = total / nbrAgents;
        unsigned int remainder = total - nbrAgents * mean;
        for( auto it = validAgents.begin(); it != validAgents.end(); ++it )
        {
            unsigned int quantity = mean;
            if( remainder > 0 )
            {
                ++quantity;
                --remainder;
            }
            if( quantity > 0 )
                ( *it )->Get< Stocks >().SetDotation( dotationType, quantity, true );
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
    const double weight = value * resource.GetUnitWeight();
    const double volume = value * resource.GetUnitVolume();
    SetData( rowIndex, 1, QString::fromStdString( resource.GetNature() ), Qt::DisplayRole, true );
    SetData( rowIndex, 2, QString::number( weight, 'f', 2 ) );
    SetData( rowIndex, 2, weight, Qt::UserRole + 1, true, Qt::AlignRight | Qt::AlignVCenter );
    SetData( rowIndex, 3, QString::number( volume, 'f', 2 ) );
    SetData( rowIndex, 3, volume, Qt::UserRole + 1, true, Qt::AlignRight | Qt::AlignVCenter );
    if( allowedNatures_.find( resource.GetNature() ) == allowedNatures_.end() )
        for( int col = 0; col < model()->columnCount(); ++col )
            SetData( rowIndex, col, Qt::red, Qt::ForegroundRole );
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
