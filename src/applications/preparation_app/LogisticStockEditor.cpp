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
    : QDialog( parent, "StocksEditionDialog", 0, Qt::WStyle_Customize | Qt::WStyle_Title )
    , controllers_( controllers )
    , selected_   ( controllers )
    , staticModel_( staticModel )
    , showStocks_( true )
{
    setCaption( tools::translate( "StocksEditionDialog", "Stocks Edition" ) );
    gui::SubObjectName subObject( "LogisticStockEditor" );
    setMinimumSize( 350, 300 );

    dataModel_ = new QStandardItemModel( this );

    delegate_ = new gui::CommonDelegate( this );
    delegate_->AddDoubleSpinBoxOnColumn( eDays, 0, std::numeric_limits< double >::max() );

    tableView_ = new gui::RichWidget< QTableView >( "tableView", this );
    tableView_->setModel( dataModel_ );
    tableView_->setItemDelegate( delegate_ );
    tableView_->setSelectionBehavior( QAbstractItemView::SelectRows );
    tableView_->setSelectionMode( QAbstractItemView::SingleSelection );
    tableView_->setAlternatingRowColors( true );
    tableView_->verticalHeader()->setVisible( false );

    validateButton_ = new gui::RichPushButton( "ok", tr( "Ok" ), this );
    cancelButton_   = new gui::RichPushButton( "cancel", tr( "Cancel" ), this );

    QGridLayout* layout = new QGridLayout( this, 2, 2, 10 );
    layout->addMultiCellWidget( tableView_, 0, 0, 0, 1 );
    layout->addWidget( validateButton_, 1, 0 );
    layout->addWidget( cancelButton_, 1, 1 );

    connect( dataModel_, SIGNAL( itemChanged( QStandardItem* ) ), SLOT( OnValueChanged( QStandardItem* ) ) );
    connect( validateButton_, SIGNAL( clicked() ), SLOT( Validate() ) );
    connect( cancelButton_, SIGNAL( clicked() ), SLOT( Reject() ) );
    hide();
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticStockEditor destructor
// Created: MMC 2011-07-21
// -----------------------------------------------------------------------------
LogisticStockEditor::~LogisticStockEditor()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticStockEditor::NotifyUpdated
// Created: JSR 2011-10-03
// -----------------------------------------------------------------------------
void LogisticStockEditor::NotifyUpdated( const ModelLoaded& )
{
    dataModel_->clear();
    QStringList horizontalHeaders;
    dataModel_->setColumnCount( 2 );
    horizontalHeaders << tr( "Category" ) <<  tr( "Days" );
    dataModel_->setHorizontalHeaderLabels( horizontalHeaders );
    tableView_->horizontalHeader()->setResizeMode( eCategory, QHeaderView::ResizeToContents );
    tableView_->horizontalHeader()->setResizeMode( eDays, QHeaderView::Stretch );
    auto itLogClass = staticModel_.objectTypes_.tools::StringResolver< kernel::LogisticSupplyClass >::CreateIterator();
    unsigned int row = 0;
    QStandardItem* item = 0;
    while( itLogClass.HasMoreElements() )
    {
        const kernel::LogisticSupplyClass& logClass = itLogClass.NextElement();
        item = new QStandardItem();
        item->setEditable( false );
        item->setCheckable( true );
        item->setCheckState( Qt::Checked );
        item->setText( logClass.GetName().c_str() );
        dataModel_->setItem( row, eCategory, item );

        item = new QStandardItem();
        item->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable );
        item->setData( QVariant( 1 ), Qt::EditRole ); // default value : 1
        dataModel_->setItem( row, eDays, item );
        ++row;
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticStockEditor::NotifyUpdated
// Created: JSR 2011-10-03
// -----------------------------------------------------------------------------
void LogisticStockEditor::NotifyUpdated( const ModelUnLoaded& )
{
    dataModel_->clear();
}

// -----------------------------------------------------------------------------
// Name: LogisticStockEditor::OnValueChanged
// Created: JSR 2011-10-04
// -----------------------------------------------------------------------------
void LogisticStockEditor::OnValueChanged( QStandardItem* item )
{
    if( item && item->column() == eCategory )
        if( QStandardItem* day = dataModel_->item( item->row(), eDays ) )
            day->setEnabled( item->checkState() == Qt::Checked );
}

// -----------------------------------------------------------------------------
// Name: LogisticStockEditor::Validate
// Created: MMC 2011-07-21
// -----------------------------------------------------------------------------
void LogisticStockEditor::Validate()
{
    SupplyHierarchy( selected_ );
    Accept();
}

// -----------------------------------------------------------------------------
// Name: LogisticStockEditor::Accept
// Created: MMC 2011-07-21
// -----------------------------------------------------------------------------
void LogisticStockEditor::Accept()
{
    accept();
    selected_ = 0;
}

// -----------------------------------------------------------------------------
// Name: LogisticStockEditor::Reject
// Created: MMC 2011-07-21
// -----------------------------------------------------------------------------
void LogisticStockEditor::Reject()
{
    reject();
    selected_ = 0;
}

// -----------------------------------------------------------------------------
// Name: LogisticStockEditor::closeEvent
// Created: MMC 2011-07-21
// -----------------------------------------------------------------------------
void LogisticStockEditor::closeEvent( QCloseEvent* /*pEvent*/ )
{
    Reject();
}

// -----------------------------------------------------------------------------
// Name: LogisticStockEditor::ShowStocksDialog
// Created: MMC 2011-07-21
// -----------------------------------------------------------------------------
void LogisticStockEditor::ShowStocksDialog()
{
    setCaption( tools::translate( "StocksEditionDialog", "Stocks Edition" ) );
    showStocks_ = true;
    show();
}

// -----------------------------------------------------------------------------
// Name: LogisticStockEditor::ShowQuotasDialog
// Created: MMC 2011-07-21
// -----------------------------------------------------------------------------
void LogisticStockEditor::ShowQuotasDialog()
{
    setCaption( tools::translate( "StocksEditionDialog", "Quotas Edition" ) );
    showStocks_ = false;
    show();
}

// -----------------------------------------------------------------------------
// Name: LogisticStockEditor::NotifyContextMenu
// Created: MMC 2011-07-21
// -----------------------------------------------------------------------------
void LogisticStockEditor::NotifyContextMenu( const Automat_ABC& automat, ContextMenu& menu )
{
    if( automat.Get< gui::LogisticBase >().IsBase() )
        Update( automat, menu );
}

// -----------------------------------------------------------------------------
// Name: LogisticStockEditor::NotifyContextMenu
// Created: MMC 2011-07-21
// -----------------------------------------------------------------------------
void LogisticStockEditor::NotifyContextMenu( const Formation_ABC& formation, ContextMenu& menu )
{
    if( formation.Get< gui::LogisticBase >().IsBase() )
        Update( formation, menu );
}

// -----------------------------------------------------------------------------
// Name: LogisticStockEditor::Update
// Created: MMC 2011-07-21
// -----------------------------------------------------------------------------
void LogisticStockEditor::Update( const Entity_ABC& entity, ContextMenu& menu )
{
    if( entity.GetId() == 0 ) // no side team
        return;
    selected_ = const_cast< Entity_ABC* >( &entity );
    ContextMenu* pSubMenu = menu.SubMenu( "Helpers", tr( "Logistic" ), false, 7 );
    pSubMenu->insertItem( tools::translate( "LogisticStockEditor", "Edit Stocks" ), this, SLOT( ShowStocksDialog() ) );
    pSubMenu->insertItem( tools::translate( "LogisticStockEditor", "Edit Quotas" ), this, SLOT( ShowQuotasDialog() ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticStockEditor::EvaluateOneDaySupplyQty
// Created: MMC 2011-08-10
// -----------------------------------------------------------------------------
void LogisticStockEditor::SupplyHierarchy( SafePointer< Entity_ABC > entity )
{
    const gui::LogisticHierarchiesBase* pLogHierarchy = entity->Retrieve< gui::LogisticHierarchiesBase >();
    if( !pLogHierarchy )
        return;
    if( !logistic_helpers::IsLogisticBase( *entity ) )
        return;

    if( showStocks_ )
    {
        std::set< const Agent_ABC* > entStocks;
        FindStocks( *entity, *entity, entStocks );
        CleanStocks( entStocks );
        T_Requirements requirements;
        auto itLogClass = staticModel_.objectTypes_.tools::StringResolver< kernel::LogisticSupplyClass >::CreateIterator();
        for( int row = 0; itLogClass.HasMoreElements(); ++row )
        {
            const kernel::LogisticSupplyClass& supplyClass = itLogClass.NextElement();
            if( dataModel_->item( row )->checkState() == Qt::Checked )
                SupplyLogisticBaseStocks( pLogHierarchy->GetEntity(), supplyClass, requirements );
        }
        SupplyStocks( entStocks, requirements );
    }
    else
        GenerateLogChildrenQuotas( *pLogHierarchy );
}

// -----------------------------------------------------------------------------
// Name: LogisticStockEditor::FillSupplyRequirements
// Created: MMC 2011-08-31
// -----------------------------------------------------------------------------
void LogisticStockEditor::FillSupplyRequirements( const Entity_ABC& entity, const kernel::LogisticSupplyClass& logType, T_Requirements& requirements )
{
    const Agent_ABC* pAgent = dynamic_cast< const Agent_ABC* >( &entity );
    if( pAgent )
        ComputeRequirements( *pAgent, logType, requirements );
    const TacticalHierarchies* pTacticalHierarchies = entity.Retrieve< TacticalHierarchies >();
    if( pTacticalHierarchies )
    {
        auto children = pTacticalHierarchies->CreateSubordinateIterator();
        while( children.HasMoreElements() )
        {
            const Entity_ABC& childrenEntity = children.NextElement();
            FillSupplyRequirements( childrenEntity, logType, requirements );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticStockEditor::SupplyLogisticBaseStocks
// Created: MMC 2011-08-31
// -----------------------------------------------------------------------------
void LogisticStockEditor::SupplyLogisticBaseStocks( const Entity_ABC& blLogBase, const kernel::LogisticSupplyClass& logType, T_Requirements& requirements )
{
    auto logChildren = blLogBase.Get< gui::LogisticHierarchiesBase >().CreateSubordinateIterator();
    while( logChildren.HasMoreElements() )
    {
        const Entity_ABC& entity = logChildren.NextElement();
        if( logistic_helpers::IsLogisticBase( entity ) )
        {
            if( blLogBase.GetId() != entity.GetId() )
                SupplyLogisticBaseStocks( entity, logType, requirements );
        }
        else
            FillSupplyRequirements( entity, logType, requirements );
    }
    const Automat_ABC* pTC2 = dynamic_cast< const Automat_ABC* >( &blLogBase );
    if( pTC2 && pTC2->Get< gui::LogisticBase >().IsBase() )
        FillSupplyRequirements( blLogBase, logType, requirements );
}

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
// Name: LogisticStockEditor::CleanStocks
// Created: MMC 2011-08-31
// -----------------------------------------------------------------------------
void LogisticStockEditor::CleanStocks( std::set< const Agent_ABC* >& entStocks )
{
    for( auto itEnt = entStocks.begin(); itEnt != entStocks.end(); ++itEnt )
    {
        Stocks& stocks = const_cast< Stocks& >( (*itEnt)->Get< Stocks >() );
        std::vector< const DotationType* > toReset;
        auto itDotation = stocks.CreateIterator();
        while( itDotation.HasMoreElements() )
        {
            const Dotation& curDotation = itDotation.NextElement();
            auto itLogClass = staticModel_.objectTypes_.tools::StringResolver< kernel::LogisticSupplyClass >::CreateIterator();
            int row = 0;
            for( ; itLogClass.HasMoreElements(); ++row )
            {
                const kernel::LogisticSupplyClass& supplyClass = itLogClass.NextElement();
                if( supplyClass.GetId() == curDotation.type_.GetLogisticSupplyClass().GetId() )
                    break;
            }
            if( dataModel_->item( row )->checkState() == Qt::Checked )
                toReset.push_back( &curDotation.type_ );
        }
        for( auto it = toReset.begin(); it!= toReset.end(); ++it )
            stocks.SetDotation( **it, 0, false );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticStockEditor::ComputeRequirements
// Created: MMC 2011-08-10
// -----------------------------------------------------------------------------
void LogisticStockEditor::ComputeRequirements( const Agent_ABC& agent, const kernel::LogisticSupplyClass& logType, T_Requirements& requirements )
{
    AgentType& agentType = staticModel_.types_.tools::Resolver< AgentType >::Get( agent.GetType().GetId() );
    auto agentCompositionIterator = agentType.CreateIterator();
    while( agentCompositionIterator.HasMoreElements() )
    {
        const AgentComposition& agentComposition = agentCompositionIterator.NextElement();
        const EquipmentType& equipmentType = staticModel_.objectTypes_.Resolver2< EquipmentType >::Get( agentComposition.GetType().GetId() );
        auto resourcesIterator = equipmentType.CreateResourcesIterator();
        while( resourcesIterator.HasMoreElements() )
        {
            const DotationCapacityType& dotationCapacity = resourcesIterator.NextElement();
            const DotationType& category = staticModel_.objectTypes_.Resolver2< DotationType >::Get( dotationCapacity.GetName() );
            if( &category.GetLogisticSupplyClass() == &logType )
                requirements[ &category ] += agentComposition.GetCount() * dotationCapacity.GetNormalizedConsumption();
        }
    }
}

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

namespace
{
    unsigned int GetQuantity( const QStandardItemModel& dataModel, int row, double requirement )
    {
        const double days = dataModel.item( row, 1 )->data( Qt::EditRole ).asDouble();
        return static_cast< unsigned int >( days * requirement + 0.5 );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticStockEditor::SupplyStocks
// Created: MMC 2011-08-31
// -----------------------------------------------------------------------------
void LogisticStockEditor::SupplyStocks( std::set< const Agent_ABC* >& entStocks, const T_Requirements& requirements )
{
    for( auto itRequired = requirements.begin(); itRequired != requirements.end(); ++itRequired )
    {
        const DotationType& dotationType = *itRequired->first;

        auto itLogClass = staticModel_.objectTypes_.tools::StringResolver< kernel::LogisticSupplyClass >::CreateIterator();
        int row = 0;
        for( ; itLogClass.HasMoreElements(); ++row )
        {
            const kernel::LogisticSupplyClass& supplyClass = itLogClass.NextElement();
            if( supplyClass.GetId() == dotationType.GetLogisticSupplyClass().GetId() )
                break;
        }

        if( dataModel_->item( row )->checkState() == Qt::Checked )
        {
            const unsigned int quantity = GetQuantity( *dataModel_, row, itRequired->second );

            std::set< const Agent_ABC* > entDotationStocks;
            for( auto it = entStocks.begin(); it != entStocks.end(); ++it )
                if( IsStockValid( **it, dotationType ) )
                {
                    Stocks& stocks = const_cast< Stocks& >( ( *it )->Get< Stocks >() );
                    stocks.SetDotation( dotationType, 0, false );
                    entDotationStocks.insert( *it );
                }
            if( quantity == 0 )
                continue;
            std::set< const Agent_ABC* > copyEntDotationStocks = entDotationStocks;
            const double surplus = DoDotationDistribution( entDotationStocks, dotationType, quantity );
            int size = static_cast< int >( copyEntDotationStocks.size() );
            if( surplus >= 1 && size > 0 )
            {
                int remaining = static_cast< int >( surplus );
                int mean =  remaining / size;
                int rest = remaining - size * mean;
                for( auto itEnt = copyEntDotationStocks.begin(); itEnt != copyEntDotationStocks.end(); ++itEnt )
                {
                    const Agent_ABC& entStock = **itEnt;
                    Stocks& stocks = const_cast< Stocks& >( entStock.Get< Stocks >() );
                    stocks.SetDotation( dotationType, mean + rest, true );
                    remaining -= mean + rest;
                    rest = 0;
                }
                assert( remaining == 0);
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticStockEditor::IsStockValid
// Created: MMC 2011-08-30
// -----------------------------------------------------------------------------
bool LogisticStockEditor::IsStockValid( const Agent_ABC& stockUnit, const DotationType& dotation )
{
    AgentType& agentType = staticModel_.types_.tools::Resolver< AgentType >::Get( stockUnit.GetType().GetId() );
    return agentType.IsStockCategoryDefined( dotation.GetLogisticSupplyClass() );
}

// -----------------------------------------------------------------------------
// Name: LogisticStockEditor::GenerateLogChildrenQuotas
// Created: MMC 2012-03-23
// -----------------------------------------------------------------------------
void LogisticStockEditor::GenerateLogChildrenQuotas( const gui::LogisticHierarchiesBase& logHierarchy )
{
    auto itLogChildren = logHierarchy.CreateSubordinateIterator();
    while( itLogChildren.HasMoreElements() )
    {
        const Entity_ABC& logChildren = itLogChildren.NextElement();
        if( logistic_helpers::IsLogisticBase( logChildren ) )
        {
            const gui::LogisticHierarchiesBase* pLogChildrenHierarchy = logChildren.Retrieve< gui::LogisticHierarchiesBase >();
            if( pLogChildrenHierarchy )
            {
                T_Requirements requirements;
                auto itLogClass = staticModel_.objectTypes_.tools::StringResolver< kernel::LogisticSupplyClass >::CreateIterator();
                for( int row = 0; itLogClass.HasMoreElements(); ++row )
                {
                    const kernel::LogisticSupplyClass& supplyClass = itLogClass.NextElement();
                    if( dataModel_->item( row )->checkState() == Qt::Checked )
                        SupplyLogisticBaseStocks( pLogChildrenHierarchy->GetEntity(), supplyClass, requirements );
                }
                SetQuotas( *pLogChildrenHierarchy, requirements );
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticStockEditor::SetQuotas
// Created: MMC 2012-03-23
// -----------------------------------------------------------------------------
void LogisticStockEditor::SetQuotas( const gui::LogisticHierarchiesBase& logHierarchy, const T_Requirements& requirements )
{
    const LogisticBaseStates* pBaseStates = dynamic_cast< const LogisticBaseStates* >( &logHierarchy );
    if( !pBaseStates )
        return;

    LogisticBaseStates& baseStates = *const_cast< LogisticBaseStates* >( pBaseStates );
    for( auto itRequired = requirements.begin(); itRequired != requirements.end(); ++itRequired )
    {
        const DotationType& dotationType = *itRequired->first;
        auto itLogClass = staticModel_.objectTypes_.tools::StringResolver< kernel::LogisticSupplyClass >::CreateIterator();
        for( int row = 0; itLogClass.HasMoreElements(); ++row )
        {
            const kernel::LogisticSupplyClass& supplyClass = itLogClass.NextElement();
            if( dataModel_->item( row )->checkState() == Qt::Checked )
            {
                if( supplyClass.GetId() == dotationType.GetLogisticSupplyClass().GetId() )
                {
                    const unsigned int quantity = GetQuantity( *dataModel_, row, itRequired->second );
                    baseStates.SetDotation( dotationType, quantity );
                }
            }
        }
    }
}
