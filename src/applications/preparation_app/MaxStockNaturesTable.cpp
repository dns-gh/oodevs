// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "MaxStockNaturesTable.h"
#include "moc_MaxStockNaturesTable.cpp"
#include "clients_gui/CommonDelegate.h"
#include "clients_gui/LogisticHelpers.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/AgentComposition.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/ComponentType.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/TacticalHierarchies.h"

// -----------------------------------------------------------------------------
// Name: MaxStockNaturesTable Constructor
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
MaxStockNaturesTable::MaxStockNaturesTable( const QString& objectName, QWidget* parent, const kernel::Resolver2< kernel::EquipmentType >& equipmentTypes ) 
    : gui::RichWidget< QTableView >( objectName, parent )
    , equipments_( equipmentTypes )
{
    dataModel_  = new QStandardItemModel( parent );
    gui::CommonDelegate* delegate = new gui::CommonDelegate( parent );

    setModel( dataModel_ );
    setItemDelegate( delegate );
    setAlternatingRowColors( true );
    setSelectionMode( NoSelection );
    verticalHeader()->hide();
    InitHeader();
}

// -----------------------------------------------------------------------------
// Name: MaxStockNaturesTable Destructor
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
MaxStockNaturesTable::~MaxStockNaturesTable()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MaxStockNaturesTable::InitHeader
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void MaxStockNaturesTable::InitHeader()
{
    dataModel_->setHorizontalHeaderLabels( QStringList() << tr( "Resource" ) << tr( "Weight (T)" ) << tr( "Max weight (T)" ) << tr( "Volume (m3)" ) << tr( "Max volume (m3)" ) );
    horizontalHeader()->setResizeMode( QHeaderView::Stretch );
    horizontalHeader()->setResizeMode( 0, QHeaderView::ResizeToContents );
}

// -----------------------------------------------------------------------------
// Name: MaxStockNaturesTable::UpdateMaxWeightVolumeByNature
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void MaxStockNaturesTable::UpdateMaxWeightVolumeByNature( const kernel::Agent_ABC& agent )
{
    if( !agent.GetType().IsLogisticSupply() )
        return;
    tools::Iterator< const kernel::AgentComposition& > itComposition = agent.GetType().CreateIterator();
    while( itComposition.HasMoreElements() )
    {
        const kernel::AgentComposition& agentComposition = itComposition.NextElement();
        const kernel::EquipmentType& equipmentType = equipments_.Get( agentComposition.GetType().GetId() );
        if( const kernel::EquipmentType::CarryingSupplyFunction* carrying = equipmentType.GetLogSupplyFunctionCarrying() )
        {
            const unsigned int nbr = agentComposition. GetCount();
            const std::string& nature = carrying->stockNature_;
            maxValues_[ nature ].weight_ += nbr * carrying->stockMaxWeightCapacity_;
            maxValues_[ nature ].volume_ += nbr * carrying->stockMaxVolumeCapacity_;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: MaxStockNaturesTable::UpdateMaxStocksByNature
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void MaxStockNaturesTable::UpdateMaxStocksByNature( const kernel::Entity_ABC& entity )
{
    if( entity.GetTypeName() == kernel::Agent_ABC::typeName_ )
    {
        UpdateMaxWeightVolumeByNature( static_cast< const kernel::Agent_ABC& >( entity ) );
        return;
    }
    const kernel::TacticalHierarchies* pTacticalHierarchies = entity.Retrieve< kernel::TacticalHierarchies >();
    if( !pTacticalHierarchies )
        return;
    tools::Iterator< const kernel::Entity_ABC& > children = pTacticalHierarchies->CreateSubordinateIterator();
    while( children.HasMoreElements() )
    {
        const kernel::Entity_ABC& child = children.NextElement();
        if( logistic_helpers::IsLogisticBase( child ) )
            continue;
        UpdateMaxStocksByNature( child );
    }
}

// -----------------------------------------------------------------------------
// Name: MaxStockNaturesTable::IsMaxExceeded
// Created: JSR 2014-03-04
// -----------------------------------------------------------------------------
bool MaxStockNaturesTable::IsMaxExceeded( const std::string& key, const WeightVolume& value ) const
{
    auto itMax = maxValues_.find( key );
    if( itMax == maxValues_.end() )
        return true;
    return value.weight_ > itMax->second.weight_ || value.volume_ > itMax->second.volume_;
}

namespace
{
    QStandardItem* CreateItem( double value, bool red )
    {
        QStandardItem* item = new QStandardItem( QString::number( value, 'f', 2 ) );
        if( red )
            item->setForeground( Qt::red );
        item->setTextAlignment( Qt::AlignRight | Qt::AlignVCenter );
        return item;
    }
}

// -----------------------------------------------------------------------------
// Name: MaxStockNaturesTable::Update
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void MaxStockNaturesTable::Update( const std::map< const kernel::DotationType*, unsigned int >& requirements, std::set< std::string >& allowedNatures )
{
    dataModel_->clear();
    InitHeader();

    T_WeightVolumes currentValues;
    for( auto it = maxValues_.begin(); it != maxValues_.end(); ++it )
    {
        allowedNatures.insert( it->first );
        currentValues[ it->first ] = WeightVolume();
    }
    for( auto it = requirements.begin(); it != requirements.end(); ++it )
    {
        const std::string& nature = it->first->GetNature();
        currentValues[ nature ].weight_ += it->second * it->first->GetUnitWeight();
        currentValues[ nature ].volume_ += it->second * it->first->GetUnitVolume();
    }
    for( auto it = currentValues.begin(); it != currentValues.end(); ++it )
    {
        WeightVolume maxValues;
        auto itMax = maxValues_.find( it->first );
        if( itMax != maxValues_.end() )
            maxValues = itMax->second;
        const bool red = IsMaxExceeded( it->first, it->second );
        QStandardItem* dotationItem = new QStandardItem( it->first.c_str() );
        if( red )
            dotationItem->setForeground( Qt::red );
        dataModel_->appendRow( QList< QStandardItem* >()
            << dotationItem
            << CreateItem( it->second.weight_, red )
            << CreateItem( maxValues.weight_ , red )
            << CreateItem( it->second.volume_, red )
            << CreateItem( maxValues.volume_ , red ) );
   }
}

// -----------------------------------------------------------------------------
// Name: MaxStockNaturesTable::UpdateMaxStocks
// Created: JSR 2014-03-04
// -----------------------------------------------------------------------------
void MaxStockNaturesTable::UpdateMaxStocks( const kernel::Entity_ABC& entity )
{
    maxValues_.clear();
    UpdateMaxStocksByNature( entity );
}
