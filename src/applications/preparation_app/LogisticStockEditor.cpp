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
#include "clients_kernel/tools.h"
#include "preparation/Dotation.h"
#include "preparation/LogisticHierarchiesBase.h"
#include "preparation/StaticModel.h"
#include "preparation/Stocks.h"
#include "ENT/ENT_Tr_Gen.h"

// -----------------------------------------------------------------------------
// Name: LogisticStockEditor constructor
// Created: MMC 2011-07-21
// -----------------------------------------------------------------------------
LogisticStockEditor::LogisticStockEditor( QWidget* parent, kernel::Controllers& controllers, const StaticModel& staticModel )
    : QDialog( parent, "SupplyStocksDialog", 0, Qt::WStyle_Customize | Qt::WStyle_Title )
    , controllers_( controllers )
    , selected_   ( controllers )
    , staticModel_( staticModel )
{
    controllers_.Register( *this );
    setCaption( tools::translate( "SupplyStocksDialog", "Supply stocks" ) );

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
    
    unsigned int row = 0;
    QStandardItem* item = 0;
    for( int i = 0; i < eNbrStockCategory; ++i )
    {
        item = new QStandardItem();
        item->setEditable( false );
        item->setCheckable( true );
        item->setCheckState( Qt::Checked );
        item->setText( QString( ENT_Tr::ConvertFromStockCategory( static_cast< E_StockCategory >( i ), ENT_Tr::eToTr ).c_str() ) );
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
    kernel::ContextMenu* pSubMenu = menu.SubMenu( "Logistic", tr( "Logistic" ) );
    pSubMenu->insertItem( tools::translate( "LogisticStockEditor", "Supply stocks" ), this, SLOT( show() ) );
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

    if( IsLogisticBase( *entity ) )
    {
        std::set< const kernel::Agent_ABC* > entStocks;
        FindStocks( *entity, *entity, entStocks );
        T_Requirements requirements;
        for( int i = 0; i < eNbrStockCategory; ++i )
            if( dataModel_->item( i )->checkState() == Qt::Checked )
                SupplyLogisticBaseStocks( pLogHierarchy->GetEntity(), (E_StockCategory)i, requirements );
        
        static const unsigned distribPass = 10;
        static const double distribFactor = 1. / static_cast< double >( distribPass );
        for( unsigned int i = 0; i < distribPass; ++i )
            SupplyStocks( entStocks, requirements, distribFactor, i > 0 );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticStockEditor::FillSupplyRequirements
// Created: MMC 2011-08-31
// -----------------------------------------------------------------------------
void LogisticStockEditor::FillSupplyRequirements( const kernel::Entity_ABC& entity, const E_StockCategory logType, T_Requirements& requirements )
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
void LogisticStockEditor::SupplyLogisticBaseStocks( const kernel::Entity_ABC& blLogBase, const E_StockCategory logType, T_Requirements& requirements )
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
                {
                    stocks->Clear();
                    entStocks.insert( pAgent );
                }
            }
        }
        else
            FindStocks( rootEntity, childrenEntity, entStocks );
    }
}

namespace
{
    E_StockCategory GetDotationLogisticType( const kernel::DotationType& dotationType )
    {
        return tools::StockCategoryFromDotationFamily( static_cast< E_DotationFamily >( dotationType.GetFamily() ), dotationType.IsDType() );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticStockEditor::ComputeRequirements
// Created: MMC 2011-08-10
// -----------------------------------------------------------------------------
void LogisticStockEditor::ComputeRequirements( const kernel::Agent_ABC& agent, const E_StockCategory logType, T_Requirements& requirements )
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
            if( logType == GetDotationLogisticType( category ) )
            {
                const double normConso = dotationCapacity.GetNormalizedConsumption();
                requirements[ &category ] += normConso * agentComposition.GetCount();
            }
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
    weight = std::max( weightCapacity - stocks.ComputeWeight(), 0. );
    volume = std::max( volumeCapacity - stocks.ComputeVolume(), 0. );
}

// -----------------------------------------------------------------------------
// Name: LogisticStockEditor::DoDotationDistribution
// Created: MMC 2012-02-17
// -----------------------------------------------------------------------------
unsigned int LogisticStockEditor::DoDotationDistribution( std::set< const kernel::Agent_ABC* >& entStocks, const kernel::DotationType& dotationType, unsigned int quantity, bool additive /* = false */ )
{
    unsigned int surplus = 0;
    double quantityRequest = static_cast< double >( quantity );
    for( std::set< const kernel::Agent_ABC* >::iterator itEnt = entStocks.begin(); itEnt != entStocks.end(); ++itEnt )
    {
        const kernel::Agent_ABC& entStock = **itEnt;                
        if( IsStockValid( entStock, dotationType ) )
        {
            double weightCapacity, volumeCapacity;
            ComputeAvailableCapacity( entStock, dotationType, weightCapacity, volumeCapacity );
            double dotationWeight = quantityRequest * dotationType.GetUnitWeight();
            double dotationVolume = quantityRequest * dotationType.GetUnitVolume();                        
            double quantitySupply = quantityRequest;
            if( dotationWeight > weightCapacity && dotationType.GetUnitWeight() > 0. )
                quantitySupply = weightCapacity / dotationType.GetUnitWeight();
            if( dotationVolume > volumeCapacity && dotationType.GetUnitVolume() > 0. )
                quantitySupply = std::min( volumeCapacity / dotationType.GetUnitVolume(), quantitySupply );
            unsigned int supply = static_cast< unsigned int >( std::max( quantitySupply, 0. ) );
            if( supply < quantity )
                surplus += quantity - supply;

            Stocks& stocks = const_cast< Stocks& >( entStock.Get< Stocks >() );
            if( additive )
                stocks.AddDotationValue( dotationType, supply );
            else
                stocks.SetDotation( dotationType, supply );
        }
    }
    return surplus;
};

// -----------------------------------------------------------------------------
// Name: LogisticStockEditor::SupplyStocks
// Created: MMC 2011-08-31
// -----------------------------------------------------------------------------
void LogisticStockEditor::SupplyStocks( std::set< const kernel::Agent_ABC* >& entStocks, const T_Requirements& requirements, double distribFactor, bool additive /* = false */ )
{
    std::map< const E_StockCategory, int > days;
    for( int i = 0; i < eNbrStockCategory; ++i )
        if( dataModel_->item( i )->checkState() == Qt::Checked )
            days[ static_cast< E_StockCategory >( i ) ] = dataModel_->item( i, 1 )->data( Qt::EditRole ).asInt();

    for( CIT_Requirements itRequired = requirements.begin(); itRequired != requirements.end(); ++itRequired )
    {
        const kernel::DotationType& dotationType = *itRequired->first;
        std::map< const E_StockCategory, int >::const_iterator itDays = days.find( GetDotationLogisticType( dotationType ) );
        if( itDays != days.end() )
        {
            unsigned int quantity = static_cast< unsigned int>( ceil( static_cast< double >( itDays->second ) * itRequired->second * distribFactor ) );
            unsigned int surplus = DoDotationDistribution( entStocks, dotationType, quantity, additive );
            if( surplus > 0 )
                DoDotationDistribution( entStocks, dotationType, surplus, true );       
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
    return agentType.IsStockCategoryDefined( GetDotationLogisticType( dotation ) );
}
