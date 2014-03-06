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
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"

// -----------------------------------------------------------------------------
// Name: LogisticStocksAndQuotasDialog constructor
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
LogisticStocksAndQuotasDialog::LogisticStocksAndQuotasDialog( QWidget* parent, kernel::Controllers& controllers, const StaticModel& staticModel )
    : QDialog( parent, "StocksEditionDialog", 0, Qt::WStyle_Customize | Qt::WStyle_Title )
    , controllers_( controllers )
    , selected_( controllers )
{
    setModal( true );
    gui::SubObjectName subObject( "LogisticStocksAndQuotasDialog" );
    setMinimumSize( 550, 300 );

    stockAndNaturesEditor_ = new StocksAndNaturesEditor( parent, staticModel );
    quotasEditor_ = new QuotasEditor( parent, staticModel );
    tabs_ = new QTabWidget( this );
    tabs_->addTab( stockAndNaturesEditor_, tr( "Stocks" ) );
    tabs_->addTab( quotasEditor_, tr( "Quotas" ) );

    QGridLayout* mainLayout = new QGridLayout( this, 2, 5, 10 );
    mainLayout->addWidget( tabs_, 0, 0, 1, 5 );

    gui::RichPushButton* automaticEditButton = new gui::RichPushButton( "ok", tr( "Automatic allocation..." ), this );
    gui::RichPushButton* okButton = new gui::RichPushButton( "ok", tr( "Ok" ), this );
    gui::RichPushButton* cancelButton = new gui::RichPushButton( "cancel", tr( "Cancel" ), this );

    mainLayout->addWidget( automaticEditButton, 1, 0, 1, 2 );
    mainLayout->addWidget( okButton, 1, 3, 1, 1 );
    mainLayout->addWidget( cancelButton, 1, 4, 1, 1 );

    automaticStocksEditor_ = new LogisticStockEditor( parent, controllers, staticModel );
    automaticQuotaEditor_ = new LogisticQuotaEditor( parent, controllers, staticModel );

    connect( automaticStocksEditor_, SIGNAL( DotationsStocksComputed( const std::map< const kernel::DotationType*, unsigned int >& ) ), stockAndNaturesEditor_, SLOT( NotifyAutomaticStocks( const std::map< const kernel::DotationType*, unsigned int >& ) ) );
    connect( automaticQuotaEditor_, SIGNAL( DotationsQuotasComputed( const std::map< const kernel::Entity_ABC*, std::map< const kernel::DotationType*, unsigned int > >& ) )
           , quotasEditor_, SLOT( NotifyAutomaticQuotas( const std::map< const kernel::Entity_ABC*, std::map< const kernel::DotationType*, unsigned int > >& ) ) );
    connect( automaticEditButton, SIGNAL( clicked() ), SLOT( ShowAutomaticDialog() ) );
    connect( okButton, SIGNAL( clicked() ), SLOT( Accept() ) );
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
    if( selected_ )
    {
        stockAndNaturesEditor_->SupplyStocks( *selected_.ConstCast() );
        quotasEditor_->ApplyQuotas();
    }
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
    kernel::Entity_ABC* pEntity = selected_.ConstCast();
    const QString entityName = pEntity->GetTypeName() == kernel::Formation_ABC::typeName_ ?
        static_cast< kernel::Formation_ABC* >( pEntity )->GetBasicName() : pEntity->GetName();
    setCaption( tr( "Stocks and Quotas" ) + " - " + entityName );
    stockAndNaturesEditor_->Initialize( *pEntity );
    quotasEditor_->Initialize( *pEntity );
    show();
}

// -----------------------------------------------------------------------------
// Name: LogisticStocksAndQuotasDialog::NotifyContextMenu
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void LogisticStocksAndQuotasDialog::NotifyContextMenu( const kernel::Automat_ABC& automat, kernel::ContextMenu& menu )
{
    CreateContextMenu( automat, menu );
}

// -----------------------------------------------------------------------------
// Name: LogisticStocksAndQuotasDialog::NotifyContextMenu
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void LogisticStocksAndQuotasDialog::NotifyContextMenu( const kernel::Formation_ABC& formation, kernel::ContextMenu& menu )
{
    CreateContextMenu( formation, menu );
}

// -----------------------------------------------------------------------------
// Name: LogisticStocksAndQuotasDialog::CreateContextMenu
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void LogisticStocksAndQuotasDialog::CreateContextMenu( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu )
{
    if( !entity.Get< gui::LogisticBase >().IsBase() )
        return;
    selected_ = &entity;
    kernel::ContextMenu* pSubMenu = menu.SubMenu( "Helpers", tr( "Logistic" ), false, 7 );
    pSubMenu->insertItem( tr( "Edit stocks and quotas" ), this, SLOT( ShowDialog() ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticStocksAndQuotasDialog::ShowAutomaticDialog
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void LogisticStocksAndQuotasDialog::ShowAutomaticDialog()
{
    if( tabs_->currentWidget() == stockAndNaturesEditor_ )
        automaticStocksEditor_->Show( *selected_ );
    else
        automaticQuotaEditor_->Show( *selected_ );
}
