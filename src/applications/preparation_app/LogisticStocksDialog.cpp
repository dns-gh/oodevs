// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "LogisticStocksDialog.h"
#include "moc_LogisticStocksDialog.cpp"
#include "StocksAndNaturesEditor.h"
#include "clients_gui/RichPushButton.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/AgentType.h"
#include "preparation/Stocks.h"

// -----------------------------------------------------------------------------
// Name: LogisticStocksDialog constructor
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
LogisticStocksDialog::LogisticStocksDialog( QWidget* parent, kernel::Controllers& controllers, const StaticModel& staticModel )
    : QDialog( parent, "StocksEditionDialog" )
    , controllers_( controllers )
    , selected_( controllers )
{
    setModal( true );
    gui::SubObjectName subObject( "LogisticStocksDialog" );
    resize( 550, 500 );

    stockAndNaturesEditor_ = new StocksAndNaturesEditor( parent, staticModel, controllers );

    QGridLayout* mainLayout = new QGridLayout( this, 2, 5, 10 );
    mainLayout->addWidget( stockAndNaturesEditor_, 0, 0, 1, 5 );

    gui::RichPushButton* okButton = new gui::RichPushButton( "ok", tr( "Ok" ), this );
    gui::RichPushButton* cancelButton = new gui::RichPushButton( "cancel", tr( "Cancel" ), this );

    mainLayout->addWidget( okButton, 1, 3, 1, 1 );
    mainLayout->addWidget( cancelButton, 1, 4, 1, 1 );

    connect( okButton, SIGNAL( clicked() ), SLOT( Accept() ) );
    connect( cancelButton, SIGNAL( clicked() ), SLOT( Reject() ) );

    hide();

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticStocksDialog::destructor
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
LogisticStocksDialog::~LogisticStocksDialog()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticStocksDialog::NotifyUpdated
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void LogisticStocksDialog::NotifyUpdated( const kernel::ModelUnLoaded& )
{
    Reject();
}

// -----------------------------------------------------------------------------
// Name: LogisticStocksDialog::Accept
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void LogisticStocksDialog::Accept()
{
    if( selected_ )
        stockAndNaturesEditor_->SupplyStocks( *selected_.ConstCast() );
    accept();
    selected_ = 0;
}

// -----------------------------------------------------------------------------
// Name: LogisticStocksDialog::Reject
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void LogisticStocksDialog::Reject()
{
    reject();
    selected_ = 0;
}

// -----------------------------------------------------------------------------
// Name: LogisticStocksDialog::ShowDialog
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void LogisticStocksDialog::ShowDialog()
{
    auto pEntity = selected_.ConstCast();
    setCaption( tr( "Stocks" ) + " - " + pEntity->GetName() );
    stockAndNaturesEditor_->Initialize( *pEntity );
    show();
}

// -----------------------------------------------------------------------------
// Name: LogisticStocksDialog::NotifyContextMenu
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void LogisticStocksDialog::NotifyContextMenu( const kernel::Agent_ABC& agent, kernel::ContextMenu& menu )
{
    if( agent.GetType().IsLogisticSupply() && agent.Retrieve< Stocks >() )
    {
        selected_ = &agent;
        kernel::ContextMenu* pSubMenu = menu.SubMenu( "Helpers", tr( "Logistic" ), false, 7 );
        pSubMenu->insertItem( tr( "Edit logistics stocks" ), this, SLOT( ShowDialog() ) );
    }
}
