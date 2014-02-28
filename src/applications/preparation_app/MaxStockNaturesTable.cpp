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
// Name: MaxStockNaturesTable::ComputeMaxWeightVolumeByNature
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void MaxStockNaturesTable::ComputeMaxWeightVolumeByNature( const kernel::Agent_ABC& agent, T_WeightVolumes& maxValues ) const
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
            auto& curNature = maxValues[ carrying->stockNature_ ];
            curNature.weight_ += nEquipments * carrying->stockMaxWeightCapacity_;
            curNature.volume_ += nEquipments * carrying->stockMaxVolumeCapacity_;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: MaxStockNaturesTable::ComputeMaxStocksByNature
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void MaxStockNaturesTable::ComputeMaxStocksByNature( const kernel::Entity_ABC& rootEntity, const kernel::Entity_ABC& entity, T_WeightVolumes& maxValues ) const
{
    const kernel::TacticalHierarchies* pTacticalHierarchies = entity.Retrieve< kernel::TacticalHierarchies >();
    if( !pTacticalHierarchies )
        return;
    if( entity.GetId() == rootEntity.GetId() )
    {
        if( const kernel::Agent_ABC* pAgent = dynamic_cast< const kernel::Agent_ABC* >( &entity ) )
        {
            ComputeMaxWeightVolumeByNature( *pAgent, maxValues );
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
            ComputeMaxWeightVolumeByNature( *pChildAgent, maxValues );
        else
            ComputeMaxStocksByNature( rootEntity, childEntity, maxValues );
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

bool IsMaxExceeded( const std::string& key, const MaxStockNaturesTable::T_WeightVolumes& values, const MaxStockNaturesTable::T_WeightVolumes& maxValues )
{
    MaxStockNaturesTable::WeightVolume value, maxValue;
    auto it = values.find( key );
    if( it != values.end() )
        value = it->second;
    auto itMax = maxValues.find( key );
    if( itMax != maxValues.end() )
        maxValue = itMax->second;
    if( maxValue.weight_ <= 0. || maxValue.volume_ <= 0. )
        return true;
    return ( value.weight_ > maxValue.weight_ ) || ( value.volume_ > maxValue.volume_ );
}
}

// -----------------------------------------------------------------------------
// Name: MaxStockNaturesTable::Update
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void MaxStockNaturesTable::Update( const kernel::Entity_ABC& logBase, const T_WeightVolumes& currentValues )
{
    dataModel_->clear();
    allowedNatures_.clear();
    InitHeader();

    T_WeightVolumes maxValues;
    ComputeMaxStocksByNature( logBase, logBase, maxValues );

    std::set< std::string > dotationNatures;
    for( auto it = currentValues.begin(); it != currentValues.end(); ++it )
        dotationNatures.insert( it->first );
    for( auto it = maxValues.begin(); it != maxValues.end(); ++it )
    {
        dotationNatures.insert( it->first );
        allowedNatures_.insert( it->first );
    }
    for( auto it = dotationNatures.begin(); it != dotationNatures.end(); ++it )
    {
        int newRowIndex = dataModel_->rowCount();
        dataModel_->setRowCount( newRowIndex + 1 );
        dataModel_->setData( dataModel_->index( newRowIndex, 0 ), QString::fromStdString( *it ) );
        dataModel_->setData( dataModel_->index( newRowIndex, 1 ), FormatValue( GetWeight( *it, currentValues ) ) );
        dataModel_->setData( dataModel_->index( newRowIndex, 2 ), FormatValue( GetWeight( *it, maxValues ) ) );
        dataModel_->setData( dataModel_->index( newRowIndex, 3 ), FormatValue( GetVolume( *it, currentValues ) ) );
        dataModel_->setData( dataModel_->index( newRowIndex, 4 ), FormatValue( GetVolume( *it, maxValues ) ) );

        QColor color = IsMaxExceeded( *it, currentValues, maxValues ) ? QColor( Qt::red ) : foregroundColor();
        for( int i = 0; i < 5; ++i )
        {
            if( i > 0 )
                dataModel_->item( newRowIndex, i )->setTextAlignment( Qt::AlignRight | Qt::AlignVCenter );
            dataModel_->item( newRowIndex, i )->setForeground( color );
        }
   }
}

// -----------------------------------------------------------------------------
// Name: MaxStockNaturesTable::GetAllowedNatures
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
const std::set< std::string >& MaxStockNaturesTable::GetAllowedNatures() const
{
    return allowedNatures_;
}
