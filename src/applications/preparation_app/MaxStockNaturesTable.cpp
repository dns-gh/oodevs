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
        const kernel::ComponentType& equipment = agentComposition.GetType();
        const kernel::EquipmentType& equipmentType = equipments_.Get( equipment.GetId() );
        if( const kernel::EquipmentType::CarryingSupplyFunction* carrying = equipmentType.GetLogSupplyFunctionCarrying() )
        {
            unsigned int nEquipments = agentComposition. GetCount();
            auto& curNature = maxValues_[ carrying->stockNature_ ];
            curNature.weight_ += nEquipments * carrying->stockMaxWeightCapacity_;
            curNature.volume_ += nEquipments * carrying->stockMaxVolumeCapacity_;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: MaxStockNaturesTable::UpdateMaxStocksByNature
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void MaxStockNaturesTable::UpdateMaxStocksByNature( const kernel::Entity_ABC& rootEntity, const kernel::Entity_ABC& entity )
{
    const kernel::TacticalHierarchies* pTacticalHierarchies = entity.Retrieve< kernel::TacticalHierarchies >();
    if( !pTacticalHierarchies )
        return;
    // todo à cleaner
    if( entity.GetId() == rootEntity.GetId() )
    {
        if( const kernel::Agent_ABC* pAgent = dynamic_cast< const kernel::Agent_ABC* >( &entity ) )
        {
            UpdateMaxWeightVolumeByNature( *pAgent );
            return;
        }
    }
    else if( logistic_helpers::IsLogisticBase( entity ) )
        return;
    tools::Iterator< const kernel::Entity_ABC& > children = pTacticalHierarchies->CreateSubordinateIterator();
    while( children.HasMoreElements() )
    {
        const kernel::Entity_ABC& childEntity = children.NextElement();
        if( const kernel::Agent_ABC* pChildAgent = dynamic_cast< const kernel::Agent_ABC* >( &childEntity ) )
            UpdateMaxWeightVolumeByNature( *pChildAgent );
        else
            UpdateMaxStocksByNature( rootEntity, childEntity );
    }
}

namespace
{
double GetWeight( const std::string& key, const MaxStockNaturesTable::T_WeightVolumes& values )
{
    auto it = values.find( key );
    return it == values.end() ? 0 : it->second.weight_;
}

double GetVolume( const std::string& key, const MaxStockNaturesTable::T_WeightVolumes& values )
{
    auto it = values.find( key );
    return it == values.end() ? 0 : it->second.volume_;
}

QString FormatValue( double value )
{
    return QString::number( value, 'f', 2 );
}

}

// -----------------------------------------------------------------------------
// Name: MaxStockNaturesTable::IsMaxExceeded
// Created: JSR 2014-03-04
// -----------------------------------------------------------------------------
bool MaxStockNaturesTable::IsMaxExceeded( const std::string& key, const MaxStockNaturesTable::T_WeightVolumes& values ) const
{
    MaxStockNaturesTable::WeightVolume value;
    MaxStockNaturesTable::WeightVolume maxValue;
    auto it = values.find( key );
    if( it != values.end() )
        value = it->second;
    auto itMax = maxValues_.find( key );
    if( itMax != maxValues_.end() )
        maxValue = itMax->second;
    if( maxValue.weight_ <= 0. || maxValue.volume_ <= 0. )
        return true;
    return ( value.weight_ > maxValue.weight_ ) || ( value.volume_ > maxValue.volume_ );
}

// -----------------------------------------------------------------------------
// Name: MaxStockNaturesTable::Update
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void MaxStockNaturesTable::Update( const T_WeightVolumes& currentValues, std::set< std::string >& allowedNatures )
{
    dataModel_->clear();
    InitHeader();

    std::set< std::string > dotationNatures;
    for( auto it = currentValues.begin(); it != currentValues.end(); ++it )
        dotationNatures.insert( it->first );
    for( auto it = maxValues_.begin(); it != maxValues_.end(); ++it )
    {
        dotationNatures.insert( it->first );
        allowedNatures.insert( it->first );
    }
    for( auto it = dotationNatures.begin(); it != dotationNatures.end(); ++it )
    {
        int newRowIndex = dataModel_->rowCount();
        dataModel_->setRowCount( newRowIndex + 1 );
        dataModel_->setData( dataModel_->index( newRowIndex, 0 ), QString::fromStdString( *it ) );
        dataModel_->setData( dataModel_->index( newRowIndex, 1 ), FormatValue( GetWeight( *it, currentValues ) ) );
        dataModel_->setData( dataModel_->index( newRowIndex, 2 ), FormatValue( GetWeight( *it, maxValues_ ) ) );
        dataModel_->setData( dataModel_->index( newRowIndex, 3 ), FormatValue( GetVolume( *it, currentValues ) ) );
        dataModel_->setData( dataModel_->index( newRowIndex, 4 ), FormatValue( GetVolume( *it, maxValues_ ) ) );

        QColor color = IsMaxExceeded( *it, currentValues ) ? QColor( Qt::red ) : foregroundColor();
        for( int i = 0; i < 5; ++i )
        {
            if( i > 0 )
                dataModel_->item( newRowIndex, i )->setTextAlignment( Qt::AlignRight | Qt::AlignVCenter );
            dataModel_->item( newRowIndex, i )->setForeground( color );
        }
   }
}

// -----------------------------------------------------------------------------
// Name: MaxStockNaturesTable::UpdateMaxStocks
// Created: JSR 2014-03-04
// -----------------------------------------------------------------------------
void MaxStockNaturesTable::UpdateMaxStocks( const kernel::Entity_ABC& entity )
{
    maxValues_.clear();
    UpdateMaxStocksByNature( entity, entity );
}
