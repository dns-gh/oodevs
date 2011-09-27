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
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/ExtensionVisitor_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/LogisticLevel.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/LogisticSupplyClass.h"
#include "clients_kernel/tools.h"
#include "ENT/ENT_Tr_Gen.h"
#include "preparation/Dotation.h"
#include "preparation/LogisticHierarchiesBase.h"
#include "preparation/StaticModel.h"
#include "preparation/Stocks.h"
#include "MT_Tools/MT_Logger.h"


// -----------------------------------------------------------------------------
// Created: MMC 2011-08-09
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: LogisticStockEditor constructor
// Created: MMC 2011-07-21
// -----------------------------------------------------------------------------
LogisticStockEditor::LogisticStockEditor( QWidget* parent, kernel::Controllers& controllers, const StaticModel& staticModel )
                         : QDialog( parent, "SupplyStocksDialog", 0, Qt::WStyle_Customize | Qt::WStyle_Title )
                         , controllers_  ( controllers )
                         , selected_     ( controllers )
                         , staticModel_( staticModel )
{
    controllers_.Register( *this );
    setCaption( tools::translate( "SupplyStocksDialog", "Supply stocks" ) );

    categotyLabel_  = new QLabel( tr( "Categories" ), this );
    category_       = new QListView( this );
    quantityLabel_  = new QLabel( tr( "Days of supply" ), this );
    factor_         = new QSpinBox( 1, 100, 1, this );
    validateButton_ = new QPushButton( tr( "Ok" ), this );
    cancelButton_   = new QPushButton( tr( "Cancel" ), this );
    layout_         = new QGridLayout( this, 3, 2, 10 );

    layout_->addWidget( categotyLabel_,     0, 0 );
    layout_->addWidget( category_,          1, 0, 1, 2 );
    layout_->addWidget( quantityLabel_,     2, 0 );
    layout_->addWidget( factor_,            2, 1 );
    layout_->addWidget( validateButton_,    3, 0 );
    layout_->addWidget( cancelButton_,      3, 1 );
    this->setFixedSize( 260, 230 );

    category_->setSelectionMode( QAbstractItemView::MultiSelection );
    category_->setEditTriggers( QAbstractItemView::NoEditTriggers );

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
// Name: LogisticStockEditor::Show
// Created: MMC 2011-07-21
// -----------------------------------------------------------------------------
void LogisticStockEditor::Show()
{
    if( !category_->model() )
    {
        QStringList listLogisticSupplyClass;
        tools::Iterator< const kernel::LogisticSupplyClass& > itLogClass = staticModel_.objectTypes_.tools::StringResolver< kernel::LogisticSupplyClass >::CreateIterator();
        for( unsigned i = 0; itLogClass.HasMoreElements(); ++i )
        {
            const kernel::LogisticSupplyClass& logClass = itLogClass.NextElement();
            listLogisticSupplyClass << QString( logClass.GetName().c_str() );
        }
        category_->setModel( new QStringListModel( listLogisticSupplyClass ) );
    }
    category_->activateWindow();
    factor_->setValue( 1 );
    show();
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
    if ( automat.GetLogisticLevel() == kernel::LogisticLevel::logistic_base_ )
        Update( automat, menu );
}

void LogisticStockEditor::NotifyContextMenu( const kernel::Formation_ABC& formation, kernel::ContextMenu& menu )
{
    if ( formation.GetLogisticLevel() == kernel::LogisticLevel::logistic_base_ )
        Update( formation, menu );
}

// -----------------------------------------------------------------------------
// Name: LogisticStockEditor::Update
// Created: MMC 2011-07-21
// -----------------------------------------------------------------------------
void LogisticStockEditor::Update( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu )
{
    selected_ = const_cast< kernel::Entity_ABC* >( &entity );
    menu.InsertItem( "Command", tools::translate( "LogisticStockEditor", "Supply stocks" ), this, SLOT( Show() ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticStockEditor::IsLogisticBase
// Created: MMC 2011-09-12
// -----------------------------------------------------------------------------
bool LogisticStockEditor::IsLogisticBase( const kernel::Entity_ABC& entity )
{
    const kernel::Automat_ABC* pAutomat = dynamic_cast< const kernel::Automat_ABC* >( &entity );
    if ( pAutomat && pAutomat->GetLogisticLevel() == kernel::LogisticLevel::logistic_base_ )
        return true;

    const kernel::Formation_ABC* pFormation = dynamic_cast< const kernel::Formation_ABC* >( &entity);
    if ( pFormation && pFormation->GetLogisticLevel() == kernel::LogisticLevel::logistic_base_ )
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
    if ( !pLogHierarchy )
        return;

    QItemSelectionModel* pModel = category_->selectionModel();
    QModelIndexList indexList = pModel->selectedIndexes();

    if ( IsLogisticBase( *entity ) )
    {
        std::set< const kernel::Agent_ABC* > entStocks;
        FindStocks( *entity, *entity, entStocks );
        std::map< const kernel::DotationType*, double > requirements;

        tools::Iterator< const kernel::LogisticSupplyClass& > itLogClass = staticModel_.objectTypes_.tools::StringResolver< kernel::LogisticSupplyClass >::CreateIterator();
        for( unsigned i = 0; itLogClass.HasMoreElements(); ++i )
            SupplyLogisticBaseStocks( pLogHierarchy->GetEntity(), itLogClass.NextElement(), requirements );
        SupplyStocks( entStocks, requirements );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticStockEditor::FillSupplyRequirements
// Created: MMC 2011-08-31
// -----------------------------------------------------------------------------
void LogisticStockEditor::FillSupplyRequirements( const kernel::Entity_ABC& entity, const kernel::LogisticSupplyClass& logType, std::map< const kernel::DotationType*, double >& requirements )
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
void LogisticStockEditor::SupplyLogisticBaseStocks( const kernel::Entity_ABC& blLogBase, const kernel::LogisticSupplyClass& logType, std::map< const kernel::DotationType*, double >& requirements )
{
    tools::Iterator< const kernel::Entity_ABC& > logChildren = blLogBase.Get< LogisticHierarchiesBase >().CreateSubordinateIterator();
    while( logChildren.HasMoreElements() )
    {
        const kernel::Entity_ABC& entity = logChildren.NextElement();
        if( IsLogisticBase( entity ) )
            SupplyLogisticBaseStocks( entity, logType, requirements );
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
    if ( !pTacticalHierarchies )
        return;

    if ( entity.GetId() != rootEntity.GetId() && IsLogisticBase( entity ) )
        return;

    tools::Iterator< const kernel::Entity_ABC& > children = pTacticalHierarchies->CreateSubordinateIterator();
    while( children.HasMoreElements() )
    {
        const kernel::Entity_ABC& childrenEntity = children.NextElement();
        const kernel::Agent_ABC* pAgent = dynamic_cast< const kernel::Agent_ABC* >( &childrenEntity );
        if ( pAgent )
        {
            if ( pAgent->GetType().IsLogisticSupply() )
            {
                Stocks* stocks = const_cast< Stocks* >( pAgent->Retrieve< Stocks >() );
                if( stocks )
                {
                    stocks->DeleteAll();
                    entStocks.insert( pAgent );
                }
            }
        }
        else
            FindStocks( rootEntity, childrenEntity, entStocks );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticStockEditor::ComputeRequirements
// Created: MMC 2011-08-10
// -----------------------------------------------------------------------------
void LogisticStockEditor::ComputeRequirements( const kernel::Agent_ABC& agent, const kernel::LogisticSupplyClass& logType, std::map< const kernel::DotationType*, double >& requirements )
{
    kernel::AgentType& agentType = staticModel_.types_.tools::Resolver< kernel::AgentType >::Get( agent.GetType().GetId() );
    tools::Iterator< const kernel::AgentComposition& > agentCompositionIterator = agentType.CreateIterator();
    for( unsigned nPos = 0; agentCompositionIterator.HasMoreElements(); ++nPos )
    {
        const kernel::AgentComposition& agentComposition = agentCompositionIterator.NextElement();
        const kernel::EquipmentType& equipmentType = staticModel_.objectTypes_.tools::Resolver< kernel::EquipmentType >::Get( agentComposition.GetType().GetId() );
        tools::Iterator< const kernel::DotationCapacityType& > resourcesIterator = equipmentType.CreateResourcesIterator();
        while( resourcesIterator.HasMoreElements() )
        {
            const kernel::DotationCapacityType& dotationCapacity = resourcesIterator.NextElement();
            const kernel::DotationType& category = staticModel_.objectTypes_.kernel::Resolver2< kernel::DotationType >::Get( dotationCapacity.GetName() );
            if( &category.GetLogisticSupplyClass() == &logType )
            {
                const double normConso = dotationCapacity.GetNormalizedConsumption();
                requirements[ &category ] += normConso * agentComposition.GetCount();
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticStockEditor::'
// Created: MMC 2011-08-31
// -----------------------------------------------------------------------------
void LogisticStockEditor::SupplyStocks( std::set< const kernel::Agent_ABC* >& entStocks, const std::map< const kernel::DotationType*, double >& requirements )
{
    for( std::map< const kernel::DotationType*, double >::const_iterator itRequired = requirements.begin(); itRequired != requirements.end(); ++itRequired )
    {
        const kernel::DotationType& dotationType = *itRequired->first;
        const double quantity = factor_->value() * itRequired->second / CountAvailableStockBases( entStocks, dotationType );
        for ( std::set< const kernel::Agent_ABC* >::iterator itEnt = entStocks.begin(); itEnt != entStocks.end(); ++itEnt )
        {
            if( IsStockValid( **itEnt, dotationType ) )
            {
                Stocks& stocks = const_cast< Stocks& >( (*itEnt)->Get< Stocks >() );
                stocks.AddDotation( new Dotation( dotationType, (unsigned int)quantity ) );
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
// Name: LogisticStockEditor::CountAvailableStockBases
// Created: MMC 2011-08-30
// -----------------------------------------------------------------------------
unsigned int LogisticStockEditor::CountAvailableStockBases( const std::set< const kernel::Agent_ABC* >& entStocks, const kernel::DotationType& requirement )
{
    unsigned int count = 0;
    for ( std::set< const kernel::Agent_ABC* >::const_iterator itEnt = entStocks.begin(); itEnt != entStocks.end(); ++itEnt )
        if ( IsStockValid( *(*itEnt), requirement ) )
            ++count;
    return count;
}
