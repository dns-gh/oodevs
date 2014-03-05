// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "LogisticStocksAndQuotasDialog.h"
#include "moc_LogisticStocksAndQuotasDialog.cpp"
#include "LogisticQuotaEditor.h"
#include "LogisticStockEditor.h"
#include "QuotasEditor.h"
#include "StocksAndNaturesEditor.h"
#include "clients_gui/LogisticBase.h"
#include "clients_gui/RichPushButton.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Tools.h"
#include "preparation/Stocks.h"

// -----------------------------------------------------------------------------
// Name: LogisticStocksAndQuotasDialog constructor
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
LogisticStocksAndQuotasDialog::LogisticStocksAndQuotasDialog( QWidget* parent, kernel::Controllers& controllers, const StaticModel& staticModel )
    : QDialog( parent, "StocksEditionDialog", 0, Qt::WStyle_Customize | Qt::WStyle_Title )
    , controllers_( controllers )
    , selected_( controllers )
    , stocksTabIndex_( 0 )
    , quotasTabIndex_( 0 )
{
    automaticStocksEditor_ = new LogisticStockEditor( parent, controllers, staticModel );
    automaticQuotaEditor_ = new LogisticQuotaEditor( parent, controllers, staticModel );

    setCaption( tools::translate( "StocksEditionDialog", "Stocks Edition" ) );
    gui::SubObjectName subObject( "LogisticStocksAndQuotasDialog" );
    setMinimumSize( 550, 300 );

    tabs_ = new CustomTabWidget( this );

    stockAndNaturesEditor_ = new StocksAndNaturesEditor( parent, staticModel );

    quotasEditor_ = new QuotasEditor( parent, staticModel );

    stocksTabIndex_ = tabs_->addTab( stockAndNaturesEditor_, "Stocks" );
    quotasTabIndex_ = tabs_->addTab( quotasEditor_, "Quotas" );

    QGridLayout* logisticLayout = new QGridLayout( this, 2, 5, 10 );
    logisticLayout->addWidget( tabs_, 0, 0, 1, 5 );

    automaticEditButton_ = new gui::RichPushButton( "ok", tr( "Automatic allocation..." ), this );
    gui::RichPushButton* validateButton = new gui::RichPushButton( "ok", tr( "Ok" ), this );
    gui::RichPushButton* cancelButton = new gui::RichPushButton( "cancel", tr( "Cancel" ), this );

    logisticLayout->addWidget( automaticEditButton_, 1, 0, 1, 2 );
    logisticLayout->addWidget( validateButton, 1, 3, 1, 1 );
    logisticLayout->addWidget( cancelButton, 1, 4, 1, 1 );

    connect( automaticStocksEditor_, SIGNAL( DotationsStocksComputed( const LogisticEditor::T_Requirements& ) ), stockAndNaturesEditor_, SLOT( NotifyAutomaticStocks( const LogisticEditor::T_Requirements& ) ) );
    connect( automaticQuotaEditor_, SIGNAL( DotationsQuotasComputed( const LogisticEditor::T_RequirementsMap& ) ), quotasEditor_, SLOT( NotifyAutomaticQuotas( const LogisticEditor::T_RequirementsMap& ) ) );
    connect( automaticEditButton_, SIGNAL( clicked() ), SLOT( ShowAutomaticDialog() ) );
    connect( validateButton, SIGNAL( clicked() ), SLOT( Accept() ) );
    connect( cancelButton, SIGNAL( clicked() ), SLOT( Reject() ) );

    hide();

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticStocksAndQuotasDialog::destructor
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
LogisticStocksAndQuotasDialog::~LogisticStocksAndQuotasDialog()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticStocksAndQuotasDialog::NotifyUpdated
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void LogisticStocksAndQuotasDialog::NotifyUpdated( const kernel::ModelUnLoaded& )
{
    Reject();
}

// -----------------------------------------------------------------------------
// Name: LogisticStocksAndQuotasDialog::Accept
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void LogisticStocksAndQuotasDialog::Accept()
{
    if( !selected_ )
        return;
    stockAndNaturesEditor_->SupplyStocks( *selected_.ConstCast() );

    if( dynamic_cast< const kernel::Agent_ABC* >( selected_.ConstCast() ) == 0 )
        quotasEditor_->ApplyQuotas();

    accept();
    selected_ = 0;
}

// -----------------------------------------------------------------------------
// Name: LogisticStocksAndQuotasDialog::Reject
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void LogisticStocksAndQuotasDialog::Reject()
{
    reject();
    selected_ = 0;
}

// -----------------------------------------------------------------------------
// Name: LogisticStocksAndQuotasDialog::ShowDialog
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void LogisticStocksAndQuotasDialog::ShowDialog()
{
    stockAndNaturesEditor_->ClearStocks(); // ??
    quotasEditor_->ClearQuotas();

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
        quotasEditor_->UpdateQuotas( *pEntity );

        tabs_->ShowTabBar();
        if( tabs_->count() < 2 )
            quotasTabIndex_ = tabs_->addTab( quotasEditor_, "Quotas" );
        tabs_->setCurrentIndex( stocksTabIndex_ );
        automaticEditButton_->show();
    }

    stockAndNaturesEditor_->UpdateInitStocks( *pEntity );

    if( bQuotas )
        quotasEditor_->UpdateInitQuotas( *pEntity );

    show();
}

// -----------------------------------------------------------------------------
// Name: LogisticStocksAndQuotasDialog::NotifyContextMenu
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void LogisticStocksAndQuotasDialog::NotifyContextMenu( const kernel::Agent_ABC& agent, kernel::ContextMenu& menu )
{
    if( agent.GetId() != 0 && agent.GetType().IsLogisticSupply() && agent.Retrieve< Stocks >() )
    {
        selected_ = &agent;
        kernel::ContextMenu* pSubMenu = menu.SubMenu( "Helpers", tr( "Logistic" ), false, 7 );
        pSubMenu->insertItem( tools::translate( "LogisticStocksAndQuotasDialog", "Edit Logistics Stocks" ), this, SLOT( ShowDialog() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticStocksAndQuotasDialog::NotifyContextMenu
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void LogisticStocksAndQuotasDialog::NotifyContextMenu( const kernel::Automat_ABC& automat, kernel::ContextMenu& menu )
{
    if( automat.Get< gui::LogisticBase >().IsBase() )
        Update( automat, menu );
}

// -----------------------------------------------------------------------------
// Name: LogisticStocksAndQuotasDialog::NotifyContextMenu
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void LogisticStocksAndQuotasDialog::NotifyContextMenu( const kernel::Formation_ABC& formation, kernel::ContextMenu& menu )
{
    if( formation.Get< gui::LogisticBase >().IsBase() )
        Update( formation, menu );
}

// -----------------------------------------------------------------------------
// Name: LogisticStocksAndQuotasDialog::Update
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void LogisticStocksAndQuotasDialog::Update( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu )
{
    if( entity.GetId() == 0 ) // no side team
        return;
    selected_ = &entity;
    kernel::ContextMenu* pSubMenu = menu.SubMenu( "Helpers", tr( "Logistic" ), false, 7 );
    pSubMenu->insertItem( tools::translate( "LogisticStocksAndQuotasDialog", "Edit Stocks & Quotas" ), this, SLOT( ShowDialog() ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticStocksAndQuotasDialog::ShowAutomaticDialog
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void LogisticStocksAndQuotasDialog::ShowAutomaticDialog()
{
    if( tabs_->currentIndex() == stocksTabIndex_ )
        automaticStocksEditor_->Show( *selected_ );
    else
        automaticQuotaEditor_->Show( *selected_ );
}
