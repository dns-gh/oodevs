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
#include "clients_kernel/LogisticLevel.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/LogisticSupplyClass.h"
#include "clients_kernel/tools.h"
#include "preparation/Dotation.h"
#include "preparation/LogisticBaseStates.h"
#include "preparation/LogisticHierarchiesBase.h"
#include "preparation/StaticModel.h"
#include "preparation/Stocks.h"

// -----------------------------------------------------------------------------
// Name: LogisticStockEditor constructor
// Created: MMC 2011-07-21
// -----------------------------------------------------------------------------
LogisticStockEditor::LogisticStockEditor( QWidget* parent, kernel::Controllers& controllers, const StaticModel& staticModel )
    : QDialog( parent, "StocksEditionDialog", 0, Qt::WStyle_Customize | Qt::WStyle_Title )
    , controllers_( controllers )
    , selected_   ( controllers )
    , staticModel_( staticModel )
    , showStocks_( true )
{
    controllers_.Register( *this );
    setCaption( tools::translate( "StocksEditionDialog", "Stocks Edition" ) );
    setMinimumSize( 350, 300 );

    dataModel_ = new QStandardItemModel( this );

    delegate_ = new gui::CommonDelegate( this );
    delegate_->AddSpinBoxOnColumn( eDays, 0, std::numeric_limits< int >::max() );

    tableView_ = new QTableView( this );
    tableView_->setModel( dataModel_ );
    tableView_->setItemDelegate( delegate_ );
    tableView_->setSelectionBehavior( QAbstractItemView::SelectRows );
    tableView_->setSelectionMode( QAbstractItemView::SingleSelection );
    tableView_->setAlternatingRowColors( true );
    tableView_->verticalHeader()->setVisible( false );

    validateButton_ = new QPushButton( tr( "Ok" ), this );
    cancelButton_   = new QPushButton( tr( "Cancel" ), this );

    QGridLayout* layout = new QGridLayout( this, 2, 2, 10 );
    layout->addMultiCellWidget( tableView_, 0, 0, 0, 1 );
    layout->addWidget( validateButton_, 1, 0 );
    layout->addWidget( cancelButton_, 1, 1 );

    connect( dataModel_, SIGNAL( itemChanged( QStandardItem* ) ), SLOT( OnValueChanged( QStandardItem* ) ) );
    connect( validateButton_, SIGNAL( clicked() ), SLOT( Validate() ) );
    connect( cancelButton_, SIGNAL( clicked() ), SLOT( Reject() ) );
    hide();
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
void LogisticStockEditor::NotifyUpdated( const kernel::ModelLoaded& )
{
    dataModel_->clear();
    QStringList horizontalHeaders;
    dataModel_->setColumnCount( 2 );
    horizontalHeaders << tr( "Category" ) <<  tr( "Days" );
    dataModel_->setHorizontalHeaderLabels( horizontalHeaders );
    tableView_->horizontalHeader()->setResizeMode( eCategory, QHeaderView::ResizeToContents );
    tableView_->horizontalHeader()->setResizeMode( eDays, QHeaderView::Stretch );
    tools::Iterator< const kernel::LogisticSupplyClass& > itLogClass = staticModel_.objectTypes_.tools::StringResolver< kernel::LogisticSupplyClass >::CreateIterator();
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
        item->setData( *new QVariant( 1 ), Qt::EditRole ); // default value : 1
        dataModel_->setItem( row, eDays, item );
        ++row;
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticStockEditor::NotifyUpdated
// Created: JSR 2011-10-03
// -----------------------------------------------------------------------------
void LogisticStockEditor::NotifyUpdated( const kernel::ModelUnLoaded& )
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
void LogisticStockEditor::NotifyContextMenu( const kernel::Automat_ABC& automat, kernel::ContextMenu& menu )
{
    if( automat.GetLogisticLevel() == kernel::LogisticLevel::logistic_base_ )
        Update( automat, menu );
}

// -----------------------------------------------------------------------------
// Name: LogisticStockEditor::NotifyContextMenu
// Created: MMC 2011-07-21
// -----------------------------------------------------------------------------
void LogisticStockEditor::NotifyContextMenu( const kernel::Formation_ABC& formation, kernel::ContextMenu& menu )
{
    if( formation.GetLogisticLevel() == kernel::LogisticLevel::logistic_base_ )
        Update( formation, menu );
}

// -----------------------------------------------------------------------------
// Name: LogisticStockEditor::Update
// Created: MMC 2011-07-21
// -----------------------------------------------------------------------------
void LogisticStockEditor::Update( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu )
{
    selected_ = const_cast< kernel::Entity_ABC* >( &entity );
    kernel::ContextMenu* pSubMenu = menu.SubMenu( "Helpers", tr( "Logistic" ), false, 7 );
    pSubMenu->insertItem( tools::translate( "LogisticStockEditor", "Edit Stocks" ), this, SLOT( ShowStocksDialog() ) );
    pSubMenu->insertItem( tools::translate( "LogisticStockEditor", "Edit Quotas" ), this, SLOT( ShowQuotasDialog() ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticStockEditor::IsLogisticBase
// Created: MMC 2011-09-12
// -----------------------------------------------------------------------------
bool LogisticStockEditor::IsLogisticBase( const kernel::Entity_ABC& entity )
{
    const kernel::Automat_ABC* pAutomat = dynamic_cast< const kernel::Automat_ABC* >( &entity );
    if( pAutomat && pAutomat->GetLogisticLevel() == kernel::LogisticLevel::logistic_base_ )
        return true;

    const kernel::Formation_ABC* pFormation = dynamic_cast< const kernel::Formation_ABC* >( &entity);
    if( pFormation && pFormation->GetLogisticLevel() == kernel::LogisticLevel::logistic_base_ )
        return true;

    return false;
}

// -----------------------------------------------------------------------------
// Name: LogisticStockEditor::EvaluateOneDaySupplyQty
// Created: MMC 2011-08-10
// -----------------------------------------------------------------------------
void LogisticStockEditor::SupplyHierarchy( kernel::SafePointer< kernel::Entity_ABC > entity )
{
    const LogisticHierarchiesBase* pLogHierarchy = entity->Retrieve< LogisticHierarchiesBase >();
    if( !pLogHierarchy )
        return;
    if( !IsLogisticBase( *entity ) )
        return;

    if( showStocks_ )
    {
        std::set< const kernel::Agent_ABC* > entStocks;
        FindStocks( *entity, *entity, entStocks );
        CleanStocks( entStocks );
        T_Requirements requirements;
        tools::Iterator< const kernel::LogisticSupplyClass& > itLogClass = staticModel_.objectTypes_.tools::StringResolver< kernel::LogisticSupplyClass >::CreateIterator();
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
void LogisticStockEditor::FillSupplyRequirements( const kernel::Entity_ABC& entity, const kernel::LogisticSupplyClass& logType, T_Requirements& requirements )
{
    const kernel::Agent_ABC* pAgent = dynamic_cast< const kernel::Agent_ABC* >( &entity );
    if( pAgent )
        ComputeRequirements( *pAgent, logType, requirements );
    const kernel::TacticalHierarchies* pTacticalHierarchies = entity.Retrieve< kernel::TacticalHierarchies >();
    if( pTacticalHierarchies )
    {
        tools::Iterator< const kernel::Entity_ABC& > children = pTacticalHierarchies->CreateSubordinateIterator();
        while( children.HasMoreElements() )
        {
            const kernel::Entity_ABC& childrenEntity = children.NextElement();
            FillSupplyRequirements( childrenEntity, logType, requirements );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticStockEditor::SupplyLogisticBaseStocks
// Created: MMC 2011-08-31
// -----------------------------------------------------------------------------
void LogisticStockEditor::SupplyLogisticBaseStocks( const kernel::Entity_ABC& blLogBase, const kernel::LogisticSupplyClass& logType, T_Requirements& requirements )
{
    tools::Iterator< const kernel::Entity_ABC& > logChildren = blLogBase.Get< LogisticHierarchiesBase >().CreateSubordinateIterator();
    while( logChildren.HasMoreElements() )
    {
        const kernel::Entity_ABC& entity = logChildren.NextElement();
        if( IsLogisticBase( entity ) )
        {
            if( blLogBase.GetId() != entity.GetId() )
                SupplyLogisticBaseStocks( entity, logType, requirements );
        }
        else
            FillSupplyRequirements( entity, logType, requirements );
    }
    const kernel::Automat_ABC* pTC2 = dynamic_cast< const kernel::Automat_ABC* >( &blLogBase );
    if( pTC2 && pTC2->GetLogisticLevel() == kernel::LogisticLevel::logistic_base_ )
        FillSupplyRequirements( blLogBase, logType, requirements );
}

// -----------------------------------------------------------------------------
// Name: LogisticStockEditor::FindStocks
// Created: MMC 2011-08-31
// -----------------------------------------------------------------------------
void LogisticStockEditor::FindStocks( const kernel::Entity_ABC& rootEntity , const kernel::Entity_ABC& entity, std::set< const kernel::Agent_ABC* >& entStocks )
{
    const kernel::TacticalHierarchies* pTacticalHierarchies = entity.Retrieve< kernel::TacticalHierarchies >();
    if( !pTacticalHierarchies )
        return;

    if( entity.GetId() != rootEntity.GetId() && IsLogisticBase( entity ) )
        return;

    tools::Iterator< const kernel::Entity_ABC& > children = pTacticalHierarchies->CreateSubordinateIterator();
    while( children.HasMoreElements() )
    {
        const kernel::Entity_ABC& childrenEntity = children.NextElement();
        const kernel::Agent_ABC* pAgent = dynamic_cast< const kernel::Agent_ABC* >( &childrenEntity );
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
void LogisticStockEditor::CleanStocks( std::set< const kernel::Agent_ABC* >& entStocks )
{
    for( std::set< const kernel::Agent_ABC* >::iterator itEnt = entStocks.begin(); itEnt != entStocks.end(); ++itEnt )
    {
        Stocks& stocks = const_cast< Stocks& >( (*itEnt)->Get< Stocks >() );
        std::vector< const kernel::DotationType* > toReset;
        tools::Iterator< const Dotation& > itDotation = stocks.CreateIterator();
        while( itDotation.HasMoreElements() )
        {
            const Dotation& curDotation = itDotation.NextElement();
            tools::Iterator< const kernel::LogisticSupplyClass& > itLogClass = staticModel_.objectTypes_.tools::StringResolver< kernel::LogisticSupplyClass >::CreateIterator();
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
        for( std::vector< const kernel::DotationType* >::iterator it = toReset.begin(); it!= toReset.end(); ++it )
            stocks.SetDotation( **it, 0, false );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticStockEditor::ComputeRequirements
// Created: MMC 2011-08-10
// -----------------------------------------------------------------------------
void LogisticStockEditor::ComputeRequirements( const kernel::Agent_ABC& agent, const kernel::LogisticSupplyClass& logType, T_Requirements& requirements )
{
    kernel::AgentType& agentType = staticModel_.types_.tools::Resolver< kernel::AgentType >::Get( agent.GetType().GetId() );
    tools::Iterator< const kernel::AgentComposition& > agentCompositionIterator = agentType.CreateIterator();
    while( agentCompositionIterator.HasMoreElements() )
    {
        const kernel::AgentComposition& agentComposition = agentCompositionIterator.NextElement();
        const kernel::EquipmentType& equipmentType = staticModel_.objectTypes_.tools::Resolver< kernel::EquipmentType >::Get( agentComposition.GetType().GetId() );
        tools::Iterator< const kernel::DotationCapacityType& > resourcesIterator = equipmentType.CreateResourcesIterator();
        while( resourcesIterator.HasMoreElements() )
        {
            const kernel::DotationCapacityType& dotationCapacity = resourcesIterator.NextElement();
            const kernel::DotationType& category = staticModel_.objectTypes_.kernel::Resolver2< kernel::DotationType >::Get( dotationCapacity.GetName() );
            if( &category.GetLogisticSupplyClass() == &logType )
                requirements[ &category ] += agentComposition.GetCount() * dotationCapacity.GetNormalizedConsumption();
        }
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticStockEditor::ComputeAvailableCapacity
// Created: MMC 2012-02-17
// -----------------------------------------------------------------------------
void LogisticStockEditor::ComputeAvailableCapacity( const kernel::Agent_ABC& entStock, const kernel::DotationType& dotationType, double& weight, double& volume )
{
    weight = volume = 0.;
    double weightCapacity = 0., volumeCapacity = 0.;
    tools::Iterator< const kernel::AgentComposition& > itComposition = entStock.GetType().CreateIterator();
    while( itComposition.HasMoreElements() )
    {
        const kernel::AgentComposition& agentComposition = itComposition.NextElement();
        const kernel::ComponentType& equipment = agentComposition.GetType();
        const kernel::EquipmentType& equipmentType = staticModel_.objectTypes_.Resolver< kernel::EquipmentType >::Get( equipment.GetId() );
        if( const kernel::EquipmentType::CarryingSupplyFunction* carrying = equipmentType.GetLogSupplyFunctionCarrying() )
            if( carrying->stockNature_ == dotationType.GetNature() )
            {
                unsigned int nEquipments = agentComposition. GetCount();
                weightCapacity += nEquipments * carrying->stockWeightCapacity_;
                volumeCapacity += nEquipments * carrying->stockVolumeCapacity_;
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
double LogisticStockEditor::DoDotationDistribution( std::set< const kernel::Agent_ABC* >& entStocks, const kernel::DotationType& dotationType, double quantity )
{
    if( entStocks.empty() || quantity < entStocks.size() )
        return quantity;

    int quantityMean = static_cast< int >( quantity / entStocks.size() );

    std::set< const kernel::Agent_ABC* > toErase;
    for( std::set< const kernel::Agent_ABC* >::iterator itEnt = entStocks.begin(); itEnt != entStocks.end(); ++itEnt )
    {
        const kernel::Agent_ABC& entStock = **itEnt;
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
        for( std::set< const kernel::Agent_ABC* >::iterator itEnt = entStocks.begin(); itEnt != entStocks.end(); ++itEnt )
        {
            const kernel::Agent_ABC& entStock = **itEnt;
            Stocks& stocks = const_cast< Stocks& >( entStock.Get< Stocks >() );
            stocks.SetDotation( dotationType, quantityMean, true );
            quantity -= quantityMean;
        }
        return quantity;
    }

    for( std::set< const kernel::Agent_ABC* >::const_iterator itErase = toErase.begin(); itErase != toErase.end(); ++itErase )
        entStocks.erase( *itErase );

    return DoDotationDistribution( entStocks, dotationType, quantity );
};

// -----------------------------------------------------------------------------
// Name: LogisticStockEditor::SupplyStocks
// Created: MMC 2011-08-31
// -----------------------------------------------------------------------------
void LogisticStockEditor::SupplyStocks( std::set< const kernel::Agent_ABC* >& entStocks, const T_Requirements& requirements )
{
    for( CIT_Requirements itRequired = requirements.begin(); itRequired != requirements.end(); ++itRequired )
    {
        const kernel::DotationType& dotationType = *itRequired->first;

        tools::Iterator< const kernel::LogisticSupplyClass& > itLogClass = staticModel_.objectTypes_.tools::StringResolver< kernel::LogisticSupplyClass >::CreateIterator();
        int row = 0;
        for( ; itLogClass.HasMoreElements(); ++row )
        {
            const kernel::LogisticSupplyClass& supplyClass = itLogClass.NextElement();
            if( supplyClass.GetId() == dotationType.GetLogisticSupplyClass().GetId() )
                break;
        }

        if( dataModel_->item( row )->checkState() == Qt::Checked )
        {
            int days = dataModel_->item( row, 1 )->data( Qt::EditRole ).asInt();
            unsigned int quantity = static_cast< unsigned int >( days * itRequired->second + 0.5 );

            std::set< const kernel::Agent_ABC* > entDotationStocks;
            for( std::set< const kernel::Agent_ABC* >::const_iterator it = entStocks.begin(); it != entStocks.end(); ++it )
                if( IsStockValid( **it, dotationType ) )
                {
                    Stocks& stocks = const_cast< Stocks& >( ( *it )->Get< Stocks >() );
                    stocks.SetDotation( dotationType, 0, false );
                    entDotationStocks.insert( *it );
                }
            if( quantity == 0 )
                continue;
            std::set< const kernel::Agent_ABC* > copyEntDotationStocks = entDotationStocks;
            double surplus = DoDotationDistribution( entDotationStocks, dotationType, static_cast< double >( quantity ) );
            if( surplus >= 1 && copyEntDotationStocks.size() > 0 )
            {
                int remaining = static_cast< int >( surplus );
                int size = static_cast< int >( copyEntDotationStocks.size() );
                int mean =  remaining / size;
                int rest = remaining - size * mean;
                for( std::set< const kernel::Agent_ABC* >::iterator itEnt = copyEntDotationStocks.begin(); itEnt != copyEntDotationStocks.end(); ++itEnt )
                {
                    const kernel::Agent_ABC& entStock = **itEnt;
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
bool LogisticStockEditor::IsStockValid( const kernel::Agent_ABC& stockUnit, const kernel::DotationType& dotation )
{
    kernel::AgentType& agentType = staticModel_.types_.tools::Resolver< kernel::AgentType >::Get( stockUnit.GetType().GetId() );
    return agentType.IsStockCategoryDefined( dotation.GetLogisticSupplyClass() );
}

// -----------------------------------------------------------------------------
// Name: LogisticStockEditor::GenerateLogChildrenQuotas
// Created: MMC 2012-03-23
// -----------------------------------------------------------------------------
void LogisticStockEditor::GenerateLogChildrenQuotas( const LogisticHierarchiesBase& logHierarchy )
{
    tools::Iterator< const kernel::Entity_ABC& > itLogChildren = logHierarchy.CreateSubordinateIterator();
    while( itLogChildren.HasMoreElements() )
    {
        const kernel::Entity_ABC& logChildren = itLogChildren.NextElement();
        if( IsLogisticBase( logChildren ) )
        {
            const LogisticHierarchiesBase* pLogChildrenHierarchy = logChildren.Retrieve< LogisticHierarchiesBase >();
            if( pLogChildrenHierarchy )
            {
                T_Requirements requirements;
                tools::Iterator< const kernel::LogisticSupplyClass& > itLogClass = staticModel_.objectTypes_.tools::StringResolver< kernel::LogisticSupplyClass >::CreateIterator();
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
void LogisticStockEditor::SetQuotas( const LogisticHierarchiesBase& logHierarchy, const T_Requirements& requirements )
{
    const LogisticBaseStates* pBaseStates = dynamic_cast< const LogisticBaseStates* >( &logHierarchy );
    if( !pBaseStates )
        return;

    LogisticBaseStates& baseStates = *const_cast< LogisticBaseStates* >( pBaseStates );
    for( CIT_Requirements itRequired = requirements.begin(); itRequired != requirements.end(); ++itRequired )
    {
        const kernel::DotationType& dotationType = *itRequired->first;
        tools::Iterator< const kernel::LogisticSupplyClass& > itLogClass = staticModel_.objectTypes_.tools::StringResolver< kernel::LogisticSupplyClass >::CreateIterator();
        for( int row = 0; itLogClass.HasMoreElements(); ++row )
            if( dataModel_->item( row )->checkState() == Qt::Checked )
            {
                const kernel::LogisticSupplyClass& supplyClass = itLogClass.NextElement();
                if( supplyClass.GetId() == dotationType.GetLogisticSupplyClass().GetId() )
                {
                    int days = dataModel_->item( row, 1 )->data( Qt::EditRole ).asInt();
                    unsigned int quantity = static_cast< unsigned int >( days * itRequired->second + 0.5 );
                    baseStates.SetDotation( dotationType, quantity );
                }
            }
    }
}

