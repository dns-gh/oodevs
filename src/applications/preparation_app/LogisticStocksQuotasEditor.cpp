// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "LogisticStocksQuotasEditor.h"
#include "moc_LogisticStocksQuotasEditor.cpp"
#include "LogisticQuotaEditor.h"
#include "LogisticStockEditor.h"
#include "MaxStockNaturesTable.h"
#include "QuotasResourcesTable.h"
#include "StockResourcesTable.h"
#include "clients_gui/LogisticBase.h"
#include "clients_gui/LogisticHelpers.h"
#include "clients_gui/RichPushButton.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/AgentComposition.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/ComponentType.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/LogisticSupplyClass.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/tools.h"
#include "preparation/Dotation.h"
#include "preparation/LogisticBaseStates.h"
#include "preparation/StaticModel.h"
#include "preparation/Stocks.h"

// -----------------------------------------------------------------------------
// Name: LogisticStocksQuotasEditor constructor
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
LogisticStocksQuotasEditor::LogisticStocksQuotasEditor( QWidget* parent, kernel::Controllers& controllers, const StaticModel& staticModel )
    : QDialog( parent, "StocksEditionDialog", 0, Qt::WStyle_Customize | Qt::WStyle_Title )
    , controllers_( controllers )
    , selected_( controllers )
    , stocksTabIndex_( 0 )
    , quotasTabIndex_( 0 )
    , supplyClasses_( staticModel.objectTypes_ )
    , agentTypes_( staticModel.types_ )
    , equipments_( staticModel.objectTypes_ )
{
    automaticStocksEditor_ = new LogisticStockEditor( parent, controllers, staticModel );
    automaticQuotaEditor_ = new LogisticQuotaEditor( parent, controllers, staticModel );

    setCaption( tools::translate( "StocksEditionDialog", "Stocks Edition" ) );
    gui::SubObjectName subObject( "LogisticStocksQuotasEditor" );
    setMinimumSize( 550, 300 );

    tabs_ = new CustomTabWidget( this );

    stocksTableView_ = new StockResourcesTable( "stocksTable", this, staticModel.objectTypes_ );
    maxStocksTableView_ = new MaxStockNaturesTable( "maxStocksTable", this, staticModel.objectTypes_ );

    QWidget* pStocks = new QWidget( this, "Stocks" );
    QGridLayout* stocksLayout = new QGridLayout( pStocks, 2, 1, 10 );
    stocksLayout->setRowStretch( 0, 2 );
    stocksLayout->setRowStretch( 1, 1 );
    stocksLayout->addWidget( stocksTableView_, 0, 0, 1, 1 );
    stocksLayout->addWidget( maxStocksTableView_, 1, 0, 1, 1 );
    pStocks->setLayout( stocksLayout );

    quotasTableView_ = new QuotasResourcesTable( "quotasTable", this, staticModel.objectTypes_ );

    subordinateCombo_ = new QComboBox( this );
    QWidget* pSubordinateQuotas = new QWidget( this, "SubordinateQuotas" );
    QGridLayout* subordinateLayout = new QGridLayout( pSubordinateQuotas, 1, 1, 10 );
    subordinateLayout->setColStretch( 0, 1 );
    subordinateLayout->setColStretch( 1, 3 );
    subordinateLayout->setAlignment( Qt::AlignRight );
    subordinateLayout->addWidget( new QLabel( tr( "Logistics subordinate:" ) ), 0, 0, 1, 1 );
    subordinateLayout->addWidget( subordinateCombo_, 0, 1, 1, 1 );

    pQuotas_ = new QWidget( this, "Quotas" );
    QGridLayout* quotasLayout = new QGridLayout( pQuotas_, 2, 1, 10 );
    quotasLayout->addWidget( pSubordinateQuotas, 0, 0, 1, 1 );
    quotasLayout->addWidget( quotasTableView_, 1, 0, 1, 1 );
    pQuotas_->setLayout( quotasLayout );

    stocksTabIndex_ = tabs_->addTab( pStocks, "Stocks" );
    quotasTabIndex_ = tabs_->addTab( pQuotas_, "Quotas" );

    QGridLayout* logisticLayout = new QGridLayout( this, 2, 5, 10 );
    logisticLayout->addWidget( tabs_, 0, 0, 1, 5 );

    automaticEditButton_ = new gui::RichPushButton( "ok", tr( "Automatic allocation..." ), this );
    gui::RichPushButton* validateButton = new gui::RichPushButton( "ok", tr( "Ok" ), this );
    gui::RichPushButton* cancelButton = new gui::RichPushButton( "cancel", tr( "Cancel" ), this );

    logisticLayout->addWidget( automaticEditButton_, 1, 0, 1, 2 );
    logisticLayout->addWidget( validateButton, 1, 3, 1, 1 );
    logisticLayout->addWidget( cancelButton, 1, 4, 1, 1 );

    connect( subordinateCombo_, SIGNAL( activated( int ) ), SLOT( OnQuotaNameChanged( int ) ) );
    connect( automaticStocksEditor_, SIGNAL( DotationsStocksComputed( const LogisticEditor::T_Requirements& ) ), SLOT( NotifyAutomaticStocks( const LogisticEditor::T_Requirements& ) ) );
    connect( automaticQuotaEditor_, SIGNAL( DotationsQuotasComputed( LogisticEditor::T_RequirementsMap& ) ), SLOT( NotifyAutomaticQuotas( LogisticEditor::T_RequirementsMap& ) ) );
    connect( stocksTableView_, SIGNAL( ResourceValueChanged() ), SLOT( NotifyStocksUserChange() ) );
    connect( quotasTableView_, SIGNAL( ResourceValueChanged() ), SLOT( NotifyQuotasUserChange() ) );
    connect( automaticEditButton_, SIGNAL( clicked() ), SLOT( ShowAutomaticDialog() ) );
    connect( validateButton, SIGNAL( clicked() ), SLOT( Accept() ) );
    connect( cancelButton, SIGNAL( clicked() ), SLOT( Reject() ) );

    hide();

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticStocksQuotasEditor::destructor
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
LogisticStocksQuotasEditor::~LogisticStocksQuotasEditor()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticStocksQuotasEditor::NotifyUpdated
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void LogisticStocksQuotasEditor::NotifyUpdated( const kernel::ModelUnLoaded& )
{
    Reject();
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
}

// -----------------------------------------------------------------------------
// Name: LogisticStocksQuotasEditor::CleanStocks
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void LogisticStocksQuotasEditor::CleanStocks( std::vector< const kernel::Agent_ABC* >& entStocks ) const
{
    for( auto it = entStocks.begin(); it != entStocks.end(); ++it )
    {
        Stocks& stocks = const_cast< Stocks& >( ( *it )->Get< Stocks >() );
        std::vector< const kernel::DotationType* > toReset;
        auto itDotation = stocks.CreateIterator();
        while( itDotation.HasMoreElements() )
        {
            const Dotation& curDotation = itDotation.NextElement();
            auto itLogClass = supplyClasses_.CreateIterator();
            for( int row = 0; itLogClass.HasMoreElements(); ++row )
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
// Name: LogisticStocksQuotasEditor::IsStockValid
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
bool LogisticStocksQuotasEditor::IsStockValid( const kernel::Agent_ABC& stockUnit, const kernel::DotationType& dotation ) const
{
    const kernel::AgentType& agentType = agentTypes_.Get( stockUnit.GetType().GetId() );
    return agentType.IsStockCategoryDefined( dotation.GetLogisticSupplyClass() );
}

// -----------------------------------------------------------------------------
// Name: LogisticStocksQuotasEditor::ComputeStockWeightVolumeLeft
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void LogisticStocksQuotasEditor::ComputeStockWeightVolumeLeft( const kernel::Agent_ABC& stockUnit, std::string nature, MaxStockNaturesTable::WeightVolume& result ) const
{
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
        const kernel::EquipmentType& equipmentType = equipments_.Get( equipment.GetId() );
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
// Name: LogisticStocksQuotasEditor::SupplyStocks
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void LogisticStocksQuotasEditor::SupplyStocks( const kernel::Entity_ABC& entityBase, const LogisticEditor::T_Requirements& requirements ) const
{
    std::vector< const kernel::Agent_ABC* > stocks;
    FindStocks( entityBase , entityBase, stocks );
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

// -----------------------------------------------------------------------------
// Name: LogisticStocksQuotasEditor::ApplyQuotas
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void LogisticStocksQuotasEditor::ApplyQuotas( const gui::LogisticHierarchiesBase& logHierarchy, const LogisticEditor::T_Requirements& generatedQuotas )
{
    const LogisticBaseStates* pBaseStates = dynamic_cast< const LogisticBaseStates* >( &logHierarchy );
    if( !pBaseStates )
        return;
    LogisticBaseStates& baseStates = *const_cast< LogisticBaseStates* >( pBaseStates );
    baseStates.ClearDotations();
    for( auto itRequired = generatedQuotas.begin(); itRequired != generatedQuotas.end(); ++itRequired )
    {
        if( itRequired->second <= 0 )
            continue;
        const kernel::DotationType& dotationType = *itRequired->first;
        auto itLogClass = supplyClasses_.CreateIterator();
        while( itLogClass.HasMoreElements() )
        {
            const auto& supplyClass = itLogClass.NextElement();
            if( supplyClass.GetId() == dotationType.GetLogisticSupplyClass().GetId() )
            {
                baseStates.SetDotation( dotationType, static_cast< unsigned int >( itRequired->second ) );
                break;
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticStocksQuotasEditor::ApplyQuotas
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void LogisticStocksQuotasEditor::ApplyQuotas()
{
    for( auto it = quotasByEntity_.begin(); it != quotasByEntity_.end(); ++it )
    {
        auto itEntity = quotasEntityFromId_.find( it->first );
        if( itEntity == quotasEntityFromId_.end() )
            continue;
        auto pEntity = itEntity->second;
        const gui::LogisticHierarchiesBase* pLogChildrenHierarchy = pEntity->Retrieve< gui::LogisticHierarchiesBase >();
        if( pLogChildrenHierarchy )
            ApplyQuotas( *pLogChildrenHierarchy, it->second );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticStocksQuotasEditor::Accept
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void LogisticStocksQuotasEditor::Accept()
{
    if( !selected_ )
        return;
    LogisticEditor::T_Requirements dotations;
    stocksTableView_->ComputeValueByDotation( dotations );
    if( auto pAgent = dynamic_cast< const kernel::Agent_ABC* >( selected_.ConstCast() ) )
    {
        std::vector< const kernel::Agent_ABC* > entStock;
        entStock.push_back( pAgent );
        CleanStocks( entStock );
        for( auto itRequired = dotations.begin(); itRequired != dotations.end(); ++itRequired )
        {
            const kernel::DotationType& dotationType = *itRequired->first;
            if( IsStockValid( *pAgent, dotationType ) )
                if( auto pStocks = const_cast< Stocks* >( pAgent->Retrieve< Stocks >() ) )
                    pStocks->SetDotation( dotationType, itRequired->second, true );
        }
    }
    else
    {
        SupplyStocks( *selected_.ConstCast(), dotations );
        ApplyQuotas();
    }
    accept();
    selected_ = 0;
}

// -----------------------------------------------------------------------------
// Name: LogisticStocksQuotasEditor::Reject
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void LogisticStocksQuotasEditor::Reject()
{
    reject();
    selected_ = 0;
}

// -----------------------------------------------------------------------------
// Name: LogisticStocksQuotasEditor::ComputeInitStocks
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void LogisticStocksQuotasEditor::ComputeInitStocks( const kernel::Entity_ABC& entity, LogisticEditor::T_Requirements& requirements ) const 
{
    std::vector< const kernel::Agent_ABC* > stocks;
    if( auto pAgent = dynamic_cast< const kernel::Agent_ABC* >( selected_.ConstCast() ) )
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

// -----------------------------------------------------------------------------
// Name: LogisticStocksQuotasEditor::ComputeInitQuotas
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void LogisticStocksQuotasEditor::ComputeInitQuotas( const kernel::Entity_ABC& entity, LogisticEditor::T_RequirementsMap& requirements ) const
{
    const gui::LogisticHierarchiesBase* pLogChildrenHierarchy = entity.Retrieve< gui::LogisticHierarchiesBase >();
    if( !pLogChildrenHierarchy )
        return;

    tools::Iterator< const kernel::Entity_ABC& > itLogChildren = pLogChildrenHierarchy->CreateSubordinateIterator();
    while( itLogChildren.HasMoreElements() )
    {
        const kernel::Entity_ABC& logChildren = itLogChildren.NextElement();
        if( logistic_helpers::IsLogisticBase( logChildren ) )
        {
            const gui::LogisticHierarchiesBase* pLogHierarchyBase = logChildren.Retrieve< gui::LogisticHierarchiesBase >();
            if( !pLogHierarchyBase )
                continue;
            const LogisticBaseStates* pBaseStates = dynamic_cast< const LogisticBaseStates* >( pLogHierarchyBase );
            if( !pBaseStates )
                continue;
            auto dotationIt = static_cast< const tools::Resolver< Dotation >* >( pBaseStates )->CreateIterator();
            while( dotationIt.HasMoreElements() )
            {
                const auto& curDotation = dotationIt.NextElement();
                requirements[ &logChildren ][ &curDotation.type_ ] += curDotation.quantity_;
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticStocksQuotasEditor::ShowDialog
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void LogisticStocksQuotasEditor::ShowDialog()
{
    quotasByEntity_.clear();
    quotasEntityFromId_.clear();
    subordinateCombo_->clear();
    stocksTableView_->OnClearItems();
    quotasTableView_->OnClearItems();

    bool bQuotas = false;
    auto pEntity = selected_.ConstCast();
    if( auto pAgent = dynamic_cast< const kernel::Agent_ABC* >( pEntity ) )
    {
        tabs_->HideTabBar();
        if( tabs_->count() > 1 )
            tabs_->removeTab( quotasTabIndex_ );
        tabs_->setCurrentIndex( stocksTabIndex_ );
        setCaption( tools::translate( "StocksEditionDialog", "Stocks" ) + QString::fromStdString( " - " ) + pAgent->GetName() );
        automaticEditButton_->hide();
    }
    else if( auto pAutomat = dynamic_cast< const kernel::Automat_ABC* >( pEntity ) )
    {
        bQuotas = true;
        setCaption( tools::translate( "StocksEditionDialog", "Stocks & Quotas" ) + QString::fromStdString( " - " ) + pAutomat->GetName() );
    }
    else if( auto pFormation = dynamic_cast< const kernel::Formation_ABC* >( pEntity ) )
    {
        bQuotas = true;
        setCaption( tools::translate( "StocksEditionDialog", "Stocks & Quotas" ) + QString::fromStdString( " - " ) + pFormation->GetBasicName() );
    }
    std::set< std::string > dummy;
    maxStocksTableView_->Update( *pEntity, std::map< std::string, MaxStockNaturesTable::WeightVolume >(), dummy );
    if( bQuotas )
    {
        tabs_->ShowTabBar();
        if( tabs_->count() < 2 )
            quotasTabIndex_ = tabs_->addTab( pQuotas_, "Quotas" );
        tabs_->setCurrentIndex( stocksTabIndex_ );
        automaticEditButton_->show();
        if( const gui::LogisticHierarchiesBase* pLogHierarchy = selected_->Retrieve< gui::LogisticHierarchiesBase >() )
        {
            tools::Iterator< const kernel::Entity_ABC& > itLogChildren = pLogHierarchy->CreateSubordinateIterator();
            while( itLogChildren.HasMoreElements() )
            {
                const kernel::Entity_ABC& logChildren = itLogChildren.NextElement();
                if( logistic_helpers::IsLogisticBase( logChildren ) )
                    if( const gui::LogisticHierarchiesBase* pLogChildrenHierarchy = logChildren.Retrieve< gui::LogisticHierarchiesBase >() )
                    {
                        subordinateCombo_->addItem( logChildren.GetName(), QVariant( static_cast< int >( logChildren.GetId() ) ) );
                        quotasByEntity_[ logChildren.GetId() ] = LogisticEditor::T_Requirements();
                        quotasEntityFromId_[ logChildren.GetId() ] = &logChildren;
                    }
            }
        }
    }

    LogisticEditor::T_Requirements initialStocks;
    ComputeInitStocks( *pEntity, initialStocks );
    NotifyAutomaticStocks( initialStocks );

    if( bQuotas )
    {
        LogisticEditor::T_RequirementsMap initialQuotas;
        ComputeInitQuotas( *pEntity, initialQuotas );
        NotifyAutomaticQuotas( initialQuotas );
    }

    show();
}

// -----------------------------------------------------------------------------
// Name: LogisticStocksQuotasEditor::NotifyContextMenu
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void LogisticStocksQuotasEditor::NotifyContextMenu( const kernel::Agent_ABC& agent, kernel::ContextMenu& menu )
{
    if( agent.GetId() != 0 && agent.GetType().IsLogisticSupply() && agent.Retrieve< Stocks >() )
    {
        selected_ = &agent;
        kernel::ContextMenu* pSubMenu = menu.SubMenu( "Helpers", tr( "Logistic" ), false, 7 );
        pSubMenu->insertItem( tools::translate( "LogisticStocksQuotasEditor", "Edit Logistics Stocks" ), this, SLOT( ShowDialog() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticStocksQuotasEditor::NotifyContextMenu
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void LogisticStocksQuotasEditor::NotifyContextMenu( const kernel::Automat_ABC& automat, kernel::ContextMenu& menu )
{
    if( automat.Get< gui::LogisticBase >().IsBase() )
        Update( automat, menu );
}

// -----------------------------------------------------------------------------
// Name: LogisticStocksQuotasEditor::NotifyContextMenu
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void LogisticStocksQuotasEditor::NotifyContextMenu( const kernel::Formation_ABC& formation, kernel::ContextMenu& menu )
{
    if( formation.Get< gui::LogisticBase >().IsBase() )
        Update( formation, menu );
}

// -----------------------------------------------------------------------------
// Name: LogisticStocksQuotasEditor::Update
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void LogisticStocksQuotasEditor::Update( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu )
{
    if( entity.GetId() == 0 ) // no side team
        return;
    selected_ = &entity;
    kernel::ContextMenu* pSubMenu = menu.SubMenu( "Helpers", tr( "Logistic" ), false, 7 );
    pSubMenu->insertItem( tools::translate( "LogisticStocksQuotasEditor", "Edit Stocks & Quotas" ), this, SLOT( ShowDialog() ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticStocksQuotasEditor::ShowAutomaticDialog
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void LogisticStocksQuotasEditor::ShowAutomaticDialog()
{
    if( tabs_->currentIndex() == stocksTabIndex_ )
        automaticStocksEditor_->Show( *selected_ );
    else
        automaticQuotaEditor_->Show( *selected_ );
}

// -----------------------------------------------------------------------------
// Name: LogisticStocksQuotasEditor::ComputeStocksByNature
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void LogisticStocksQuotasEditor::ComputeStocksByNature( std::map< std::string, MaxStockNaturesTable::WeightVolume >& result ) const
{
    LogisticEditor::T_Requirements dotations;
    stocksTableView_->ComputeValueByDotation( dotations );
    for( auto it = dotations.begin(); it != dotations.end(); ++it )
    {
        const auto& curDotation = *it->first;
        double value = it->second;
        result[ curDotation.GetNature() ].weight_ += value * curDotation.GetUnitWeight();
        result[ curDotation.GetNature() ].volume_ += value * curDotation.GetUnitVolume();
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticStocksQuotasEditor::NotifyAutomaticStocks
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void LogisticStocksQuotasEditor::NotifyAutomaticStocks( const LogisticEditor::T_Requirements& stocks )
{
    stocksTableView_->Disconnect();
    stocksTableView_->OnClearItems();
    for( auto it = stocks.begin(); it != stocks.end(); ++it )
        stocksTableView_->AddResource( *it->first, it->second );
    NotifyStocksUserChange();
    stocksTableView_->Connect();
}

// -----------------------------------------------------------------------------
// Name: LogisticStocksQuotasEditor::NotifyStocksUserChange
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void LogisticStocksQuotasEditor::NotifyStocksUserChange()
{
    std::map< std::string, MaxStockNaturesTable::WeightVolume > valuesByNature;
    ComputeStocksByNature( valuesByNature );
    std::set< std::string > allowedNatures;
    maxStocksTableView_->Update( *selected_.ConstCast(), valuesByNature, allowedNatures );
    stocksTableView_->SetAllowedNatures( allowedNatures );
}

// -----------------------------------------------------------------------------
// Name: LogisticStocksQuotasEditor::NotifyAutomaticQuotas
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void LogisticStocksQuotasEditor::NotifyAutomaticQuotas( LogisticEditor::T_RequirementsMap& quotas )
{
    quotasByEntity_.clear();
    for( auto it = quotas.begin(); it != quotas.end(); ++it )
        for( auto itD = it->second.begin(); itD != it->second.end(); ++itD )
            quotasByEntity_[ it->first->GetId() ][ itD->first ] = itD->second;
    OnQuotaNameChanged( subordinateCombo_->currentIndex() );
}

// -----------------------------------------------------------------------------
// Name: LogisticStocksQuotasEditor::NotifyAutomaticQuotas
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void LogisticStocksQuotasEditor::NotifyAutomaticQuotas( const LogisticEditor::T_Requirements& stocks )
{
    quotasTableView_->OnClearItems();
    for( auto it = stocks.begin(); it != stocks.end(); ++it )
        quotasTableView_->AddResource( *it->first, it->second );
}

// -----------------------------------------------------------------------------
// Name: LogisticStocksQuotasEditor::OnQuotaNameChanged
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void LogisticStocksQuotasEditor::OnQuotaNameChanged( int index )
{   
    quotasTableView_->Disconnect();
    disconnect( quotasTableView_, SIGNAL( ResourceValueChanged() ), this, SLOT( NotifyQuotasUserChange() ) );
    int entityId = subordinateCombo_->itemData( index, Qt::UserRole ).toInt();
    if( entityId != 0 )
        NotifyAutomaticQuotas( quotasByEntity_[ entityId ] );
    quotasTableView_->Connect();
    connect( quotasTableView_, SIGNAL( ResourceValueChanged() ), SLOT( NotifyQuotasUserChange() ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticStocksQuotasEditor::NotifyQuotasUserChange
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void LogisticStocksQuotasEditor::NotifyQuotasUserChange()
{
    int entityId = subordinateCombo_->itemData( subordinateCombo_->currentIndex(), Qt::UserRole ).toInt();
    if( entityId == 0)
        return;
    LogisticEditor::T_Requirements dotations;
    quotasTableView_->ComputeValueByDotation( dotations );
    quotasByEntity_[ entityId ] = dotations;
}
