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
                         , listDotationLogisticTypes_ ( new QStringList() )
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
    layout_->addWidget( category_,          0, 1 );
    layout_->addWidget( quantityLabel_,     1, 0 );
    layout_->addWidget( factor_,            1, 1 );
    layout_->addWidget( validateButton_,    2, 0 );
    layout_->addWidget( cancelButton_,      2, 1 );
    this->setFixedSize( 280, 280 );

    for ( int i = 0; i<eNbrStockCategory; ++i )
        *listDotationLogisticTypes_ << QString( ENT_Tr::ConvertFromStockCategory( static_cast< E_StockCategory >( i ), ENT_Tr::eToTr ).c_str() );

    category_->setSelectionMode( QAbstractItemView::MultiSelection );
    category_->setEditTriggers( QAbstractItemView::NoEditTriggers );
    category_->setModel( new QStringListModel( *listDotationLogisticTypes_ ) );
   
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
// Name: LogisticStockEditor::EvaluateOneDaySupplyQty
// Created: MMC 2011-08-10
// -----------------------------------------------------------------------------
void LogisticStockEditor::SupplyHierarchy( kernel::SafePointer< kernel::Entity_ABC > entity )
{
    const LogisticHierarchiesBase* pLogHierarchy = entity->Retrieve< LogisticHierarchiesBase >();
    if ( pLogHierarchy )
        SupplyLogisticBaseStocks( *pLogHierarchy );
}

// -----------------------------------------------------------------------------
// Name: LogisticStockEditor::SupplyLogisticBaseStocks
// Created: MMC 2011-08-10
// -----------------------------------------------------------------------------
void LogisticStockEditor::SupplyLogisticBaseStocks( const LogisticHierarchiesBase& logHierarchy )
{
    QItemSelectionModel* pModel = category_->selectionModel();
    QModelIndexList indexList = pModel->selectedIndexes();

    const kernel::Hierarchies& hierarchy = static_cast< const kernel::Hierarchies &>( logHierarchy );
    const kernel::Automat_ABC* pAutomat = dynamic_cast< const kernel::Automat_ABC* >( &hierarchy.GetEntity() );
    if ( pAutomat )
    {
        if ( pAutomat->GetLogisticLevel() == kernel::LogisticLevel::logistic_base_ )
            for ( int i=0; i < indexList.count(); ++i )
            {
                E_StockCategory logType = static_cast< E_StockCategory >( indexList[i].row() );
                LogisticStockEditor::SupplyTc2LogisticBaseStock( logHierarchy.GetEntity(), logType );
            }
        return;
    }

    const kernel::Formation_ABC* pFormation = dynamic_cast< const kernel::Formation_ABC* >( &hierarchy.GetEntity() );
    if ( pFormation )
    {
        if ( pFormation->GetLogisticLevel() == kernel::LogisticLevel::logistic_base_ )
            for ( int i=0; i < indexList.count(); ++i )
            {
                E_StockCategory logType = static_cast< E_StockCategory >( indexList[i].row() );
                SupplyBlLogisticBaseStock( logHierarchy.GetEntity(), logType );
            }
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticStockEditor::SupplyBlLogisticBaseStock
// Created: MMC 2011-08-31
// -----------------------------------------------------------------------------
void LogisticStockEditor::SupplyBlLogisticBaseStock( const kernel::Entity_ABC& blLogBase, E_StockCategory logType )
{
    std::set< const kernel::Agent_ABC* > entStocks;
    FindAgentStocksInFormationLogisticBase( blLogBase, blLogBase, entStocks );

    std::set< const kernel::Entity_ABC* > logBases;
    FindTc2LogisticBases( blLogBase.Get< LogisticHierarchiesBase >(), logBases );

    std::map< const kernel::DotationType*, double > requirements;
    for ( std::set< const kernel::Entity_ABC* >::iterator it = logBases.begin(); it != logBases.end(); ++it )
    {
        const kernel::Entity_ABC* pLogBase= *it; assert( pLogBase );
        std::set< const kernel::Agent_ABC* > entToSupply;
        FindTc2AgentsToSupply( *pLogBase, entToSupply );

        for ( std::set< const kernel::Agent_ABC* >::iterator itEnt = entToSupply.begin(); itEnt != entToSupply.end(); ++itEnt )
            ComputeRequirements( *(*itEnt), logType, requirements );
    }

    SupplyStocks( entStocks, requirements );
}

// -----------------------------------------------------------------------------
// Name: LogisticStockEditor::SupplyTc2LogisticBaseStock
// Created: MMC 2011-08-31
// -----------------------------------------------------------------------------
void LogisticStockEditor::SupplyTc2LogisticBaseStock( const kernel::Entity_ABC& logBase, E_StockCategory logType )
{
    std::set< const kernel::Agent_ABC* > entStocks;
    FindAgentStocksInTc2LogisticBase( logBase, entStocks );

    std::set< const kernel::Agent_ABC* > entToSupply;
    FindTc2AgentsToSupply( logBase, entToSupply );

    std::map< const kernel::DotationType*, double > requirements;
    for ( std::set< const kernel::Agent_ABC* >::iterator itEnt = entToSupply.begin(); itEnt != entToSupply.end(); ++itEnt )
        ComputeRequirements( *(*itEnt), logType, requirements );

    SupplyStocks( entStocks, requirements );
}

// -----------------------------------------------------------------------------
// Name: LogisticStockEditor::SupplyStocks
// Created: MMC 2011-08-31
// -----------------------------------------------------------------------------
void LogisticStockEditor::SupplyStocks( std::set< const kernel::Agent_ABC* >& entStocks, const std::map< const kernel::DotationType*, double >& requirements )
{
    std::map< const kernel::DotationType*, unsigned int > availableStockBasesCount;
    for ( std::map< const kernel::DotationType*, double >::const_iterator itRequired = requirements.begin(); itRequired != requirements.end(); ++itRequired )
        availableStockBasesCount[ itRequired->first ] = CountAvalaibleStockBases( entStocks, *itRequired->first );

    for ( std::map< const kernel::DotationType*, double >::const_iterator itRequired = requirements.begin(); itRequired != requirements.end(); ++itRequired )
    {
        std::map< const kernel::DotationType*, unsigned int >::iterator itAvailable = availableStockBasesCount.find( itRequired->first );
        if ( itAvailable != availableStockBasesCount.end() && itAvailable->second > 0 )
        {
            const kernel::DotationType& dotation = *itRequired->first;
            double suppply = itRequired->second * double( factor_->value() ) / double( itAvailable->second );
            for ( std::set< const kernel::Agent_ABC* >::iterator itEnt = entStocks.begin(); itEnt != entStocks.end(); ++itEnt )
            {
                Stocks& stocks = const_cast< Stocks& >( (*itEnt)->Get< Stocks >() );
                stocks.AddDotation( new Dotation( dotation, static_cast< unsigned int >( ceil( suppply ) ) ) );
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticStockEditor::FindTc2LogisticBases
// Created: MMC 2011-08-10
// -----------------------------------------------------------------------------
void LogisticStockEditor::FindTc2LogisticBases( const LogisticHierarchiesBase& logHierarchy, std::set< const kernel::Entity_ABC* >& logBases )
{
    const kernel::Hierarchies& hierarchy = static_cast< const kernel::Hierarchies &>( logHierarchy );
    const kernel::Automat_ABC* pAutomat = dynamic_cast< const kernel::Automat_ABC* >( &hierarchy.GetEntity() );
    if ( pAutomat && pAutomat->GetLogisticLevel() == kernel::LogisticLevel::logistic_base_ )
    {
        std::set< const kernel::Agent_ABC* > entStocks;
        FindAgentStocksInTc2LogisticBase( *pAutomat, entStocks );
        if ( !entStocks.empty() )
            logBases.insert( &logHierarchy.GetEntity() );
    }

    tools::Iterator< const kernel::Entity_ABC& > logChildren = logHierarchy.CreateSubordinateIterator();
    while( logChildren.HasMoreElements() )
    {
        const kernel::Entity_ABC& entity = logChildren.NextElement();
        const LogisticHierarchiesBase* pChildLogisticHierarchies = entity.Retrieve< LogisticHierarchiesBase >();
        if ( pChildLogisticHierarchies && pChildLogisticHierarchies != &logHierarchy )
            FindTc2LogisticBases( *pChildLogisticHierarchies, logBases );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticStockEditor::FindAgentStocksInFormationLogisticBase
// Created: MMC 2011-08-31
// -----------------------------------------------------------------------------
void LogisticStockEditor::FindAgentStocksInFormationLogisticBase( const kernel::Entity_ABC& rootEntity , const kernel::Entity_ABC& entity, std::set< const kernel::Agent_ABC* >& entStocks )
{
    const kernel::TacticalHierarchies* pTacticalHierarchies = entity.Retrieve< kernel::TacticalHierarchies >();
    if ( !pTacticalHierarchies )
        return;

    const kernel::Automat_ABC* pAutomat = dynamic_cast< const kernel::Automat_ABC* >( &entity );
    if ( pAutomat && pAutomat->GetLogisticLevel() == kernel::LogisticLevel::logistic_base_ )
        return;
    
    if ( entity.GetId() != rootEntity.GetId() )
    {
        const kernel::Formation_ABC* pFormation = dynamic_cast< const kernel::Formation_ABC* >( &entity );
        if ( pFormation && pFormation->GetLogisticLevel() == kernel::LogisticLevel::logistic_base_ )
            return;
    }

    tools::Iterator< const kernel::Entity_ABC& > children = pTacticalHierarchies->CreateSubordinateIterator();
    while( children.HasMoreElements() )
    {
        const kernel::Entity_ABC& childrenEntity = children.NextElement();
        const kernel::Agent_ABC* pAgent = dynamic_cast< const kernel::Agent_ABC* >( &childrenEntity );
        if ( pAgent )
        {
            if ( pAgent->GetType().IsLogisticSupply() && pAgent->Retrieve< Stocks >() )
                entStocks.insert( pAgent );
        }
        else 
            FindAgentStocksInFormationLogisticBase( rootEntity, childrenEntity, entStocks );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticStockEditor::FindAgentStocksInTc2LogBase
// Created: MMC 2011-08-10
// -----------------------------------------------------------------------------
void LogisticStockEditor::FindAgentStocksInTc2LogisticBase( const kernel::Entity_ABC& logBase, std::set< const kernel::Agent_ABC* >& entStocks )
{
    const kernel::TacticalHierarchies* pTacticalHierarchies = logBase.Retrieve< kernel::TacticalHierarchies >();
    if ( !pTacticalHierarchies )
        return;

    tools::Iterator< const kernel::Entity_ABC& > children = pTacticalHierarchies->CreateSubordinateIterator();
    while( children.HasMoreElements() )
    {
        const kernel::Agent_ABC* pAgent = dynamic_cast< const kernel::Agent_ABC* >( &children.NextElement() );
        if ( pAgent )
            if ( pAgent->GetType().IsLogisticSupply() && pAgent->Retrieve< Stocks >() )
                entStocks.insert( pAgent );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticStockEditor::FindTc2AgentsToSupply
// Created: MMC 2011-08-10
// -----------------------------------------------------------------------------
void LogisticStockEditor::FindTc2AgentsToSupply( const kernel::Entity_ABC& logBase, std::set< const kernel::Agent_ABC* >& entToSupply )
{
    const LogisticHierarchiesBase* pLogHierarchies = logBase.Retrieve< LogisticHierarchiesBase >();
    if ( !pLogHierarchies )
        return;

    const kernel::Agent_ABC* pLogBase = dynamic_cast< const kernel::Agent_ABC* >( &logBase );
    if ( pLogBase )
        entToSupply.insert( pLogBase );

    tools::Iterator< const kernel::Entity_ABC& > logChildren = pLogHierarchies->CreateSubordinateIterator();
    while( logChildren.HasMoreElements() )
    {
        const kernel::Automat_ABC* pAutomat = dynamic_cast< const kernel::Automat_ABC* >( &logChildren.NextElement() );
        if ( pAutomat )
        {
            const kernel::TacticalHierarchies* pTacticalHierarchies = pAutomat->Retrieve< kernel::TacticalHierarchies >();
            if ( pTacticalHierarchies )
            {
                tools::Iterator< const kernel::Entity_ABC& > automatChildren = pTacticalHierarchies->CreateSubordinateIterator();
                while( automatChildren.HasMoreElements() )
                {
                    const kernel::Agent_ABC* pAgent = dynamic_cast< const kernel::Agent_ABC* >( &automatChildren.NextElement() );
                    if ( pAgent )
                        entToSupply.insert( pAgent );
                }
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticStockEditor::DotationLogisticFromEquipment
// Created: MMC 2011-08-10
// -----------------------------------------------------------------------------
E_StockCategory LogisticStockEditor::GetDotationLogisticType( const kernel::DotationType& dotationType )
{
    E_DotationFamily family = static_cast< E_DotationFamily >( dotationType.GetFamily() );
    if ( dotationType.IsDType() )
    {     
        if ( family == eDotationFamily_Munition )
            return eStockCategory_UniteFireDirect;
        else
            throw std::runtime_error( "invalid category : d-type resource '" + dotationType.GetName() + "'" );
    }

    switch( family )
    {
    case eDotationFamily_Munition :
    case eDotationFamily_Explosif :
    case eDotationFamily_Mine :
    case eDotationFamily_Barbele :
        return eStockCategory_UniteFireNotDirect;
    case eDotationFamily_Carburant :
        return eStockCategory_UniteFuel;
    case eDotationFamily_Piece :
        return eStockCategory_Piece;
    case eDotationFamily_Ration :
    case eDotationFamily_AgentExtincteur :
    case eDotationFamily_Energy :
        return eStockCategory_UniteSupply;
    default :
        throw std::runtime_error( "Unknown category : resource '" + dotationType.GetName() + "' " );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticStockEditor::ComputeRequirements
// Created: MMC 2011-08-10
// -----------------------------------------------------------------------------
void LogisticStockEditor::ComputeRequirements( const kernel::Agent_ABC& agent, E_StockCategory logType, std::map< const kernel::DotationType*, double >& requirements )
{
    kernel::AgentType& agentType = staticModel_.types_.tools::Resolver< kernel::AgentType >::Get( agent.GetType().GetId() );
    tools::Iterator< const kernel::AgentComposition& > agentCompositionIterator = agentType.CreateIterator();
    for( unsigned nPos = 0; agentCompositionIterator.HasMoreElements(); ++nPos )
    {
        const kernel::AgentComposition& agentComposition = agentCompositionIterator.NextElement();
        const std::string& agentName = agentComposition.GetType().GetName();
        tools::Iterator< const kernel::EquipmentType& > equipmentTypeIterator = staticModel_.objectTypes_.tools::Resolver< kernel::EquipmentType >::CreateIterator();
        while( equipmentTypeIterator.HasMoreElements() )
        {
            const kernel::EquipmentType& equipmentType = equipmentTypeIterator.NextElement();
            if( equipmentType.GetName() == agentName )
            {
                tools::Iterator< const kernel::DotationCapacityType& > resourcesIterator = equipmentType.CreateResourcesIterator();
                while( resourcesIterator.HasMoreElements() )
                {
                    const kernel::DotationCapacityType& dotationCapa = resourcesIterator.NextElement();
                    const kernel::DotationType& category = staticModel_.objectTypes_.kernel::Resolver2< kernel::DotationType >::Get( dotationCapa.GetName() );
                    if ( GetDotationLogisticType( category ) ==  logType )
                    {
                        double normConso = dotationCapa.GetNormalizedConsumption();
                        std::map< const kernel::DotationType*, double >::iterator itRequired = requirements.find( &category );
                        if ( itRequired == requirements.end() )
                            requirements[ &category ] = normConso;
                        else
                            itRequired->second += normConso;
                    }
                }
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
    return agentType.IsStockCategoryDefined( ENT_Tr::ConvertFromStockCategory( GetDotationLogisticType( dotation ), ENT_Tr::eToSim ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticStockEditor::CountAvalaibleStockBases
// Created: MMC 2011-08-30
// -----------------------------------------------------------------------------
unsigned int LogisticStockEditor::CountAvalaibleStockBases( const std::set< const kernel::Agent_ABC* >& entStocks, const kernel::DotationType& requirement )
{
    unsigned int count = 0;
    for ( std::set< const kernel::Agent_ABC* >::const_iterator itEnt = entStocks.begin(); itEnt != entStocks.end(); ++itEnt )
        if ( IsStockValid( *(*itEnt), requirement ) )
            ++count;
    return count;
}
