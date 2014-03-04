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
#include "StocksAndNaturesEditor.h"
#include "QuotasResourcesTable.h"
#include "clients_gui/LogisticBase.h"
#include "clients_gui/LogisticHelpers.h"
#include "clients_gui/RichPushButton.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/LogisticSupplyClass.h"
#include "clients_kernel/ObjectTypes.h"
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
{
    automaticStocksEditor_ = new LogisticStockEditor( parent, controllers, staticModel );
    automaticQuotaEditor_ = new LogisticQuotaEditor( parent, controllers, staticModel );

    setCaption( tools::translate( "StocksEditionDialog", "Stocks Edition" ) );
    gui::SubObjectName subObject( "LogisticStocksQuotasEditor" );
    setMinimumSize( 550, 300 );

    tabs_ = new CustomTabWidget( this );

    stockAndNaturesEditor_ = new StocksAndNaturesEditor( parent, staticModel );

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

    stocksTabIndex_ = tabs_->addTab( stockAndNaturesEditor_, "Stocks" );
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
    connect( automaticStocksEditor_, SIGNAL( DotationsStocksComputed( const LogisticEditor::T_Requirements& ) ), stockAndNaturesEditor_, SLOT( NotifyAutomaticStocks( const LogisticEditor::T_Requirements& ) ) );
    connect( automaticQuotaEditor_, SIGNAL( DotationsQuotasComputed( LogisticEditor::T_RequirementsMap& ) ), SLOT( NotifyAutomaticQuotas( LogisticEditor::T_RequirementsMap& ) ) );
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
    stockAndNaturesEditor_->SupplyStocks( *selected_.ConstCast() );

    if( dynamic_cast< const kernel::Agent_ABC* >( selected_.ConstCast() ) == 0 )
        ApplyQuotas();

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
    stockAndNaturesEditor_->ClearStocks(); // ??
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
    stockAndNaturesEditor_->UpdateMaxStocks( *pEntity );
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

    stockAndNaturesEditor_->UpdateInitStocks( *pEntity );

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
