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
    QSortFilterProxyModel* proxyModel = new QSortFilterProxyModel( this );
    proxyModel->setSourceModel( dataModel_ );
    proxyModel->setSortRole( Qt::UserRole + 1 );
    gui::CommonDelegate* delegate = new gui::CommonDelegate( parent );

    setModel( proxyModel );
    setItemDelegate( delegate );
    setAlternatingRowColors( true );
    setSelectionMode( NoSelection );
    setSortingEnabled( true );
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
    dataModel_->setHorizontalHeaderLabels( QStringList() << tr( "Nature" ) << tr( "Weight (T)" ) << tr( "Max weight (T)" ) << tr( "Volume (m3)" ) << tr( "Max volume (m3)" ) );
    horizontalHeader()->setResizeMode( QHeaderView::Interactive );
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

namespace
{
    QStandardItem* CreateItem( double value, bool red )
    {
        QStandardItem* item = new QStandardItem( QString::number( value, 'f', 2 ) );
        item->setData( value );
        item->setTextAlignment( Qt::AlignRight | Qt::AlignVCenter );
        if( red )
            item->setForeground( Qt::red );
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
        const bool invalid = ( itMax == maxValues_.end() );
        if( !invalid )
            maxValues = itMax->second;
        const bool overWeight = invalid || it->second.weight_ > maxValues.weight_;
        const bool overVolume = invalid || it->second.volume_ > maxValues.volume_;
        const QString name = it->first.c_str();
        QStandardItem* dotationItem = new QStandardItem( name );
        dotationItem->setData( name );
        if( invalid )
            dotationItem->setForeground( Qt::red );
        dataModel_->appendRow( QList< QStandardItem* >()
            << dotationItem
            << CreateItem( it->second.weight_, overWeight )
            << CreateItem( maxValues.weight_ , invalid )
            << CreateItem( it->second.volume_, overVolume )
            << CreateItem( maxValues.volume_ , invalid ) );
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
