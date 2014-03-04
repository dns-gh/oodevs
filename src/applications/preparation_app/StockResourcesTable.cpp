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
#include "clients_kernel/AgentType.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Tools.h"
#include "preparation/Dotation.h"
#include "preparation/Stocks.h"

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
StockResourcesTable::StockResourcesTable( const QString& objectName, QWidget* parent, const kernel::Resolver2< kernel::DotationType >& dotationsType ) 
    : ResourcesEditorTable_ABC( GetHeaders(), objectName, parent, dotationsType )
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
