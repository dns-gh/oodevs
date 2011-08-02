// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ConnectionMenu.h"
#include "moc_ConnectionMenu.cpp"
#include "ConnectDialog.h"
#include "DisconnectDialog.h"
#include "clients_kernel/Controllers.h"
#include "clients_gui/resources.h"
#include "gaming/Simulation.h"
#include "gaming/Tools.h"

// -----------------------------------------------------------------------------
// Name: ConnectionMenu constructor
// Created: SBO 2008-09-18
// -----------------------------------------------------------------------------
ConnectionMenu::ConnectionMenu( Q3PopupMenu* parent, kernel::Controllers& controllers, Network& network, kernel::Logger_ABC& logger )
    : QObject( parent )
    , menu_( parent )
    , controllers_( controllers )
    , hosts_( new Q3PopupMenu( menu_ ) )
    , connectDialog_( new ConnectDialog( menu_->parentWidget(), network, logger ) )
    , disconnectDialog_( new DisconnectDialog( menu_->parentWidget(), network ) )
{
    connectDialog_->FillPopupMenu( hosts_ );
    connectDialog_->hide();
    disconnectDialog_->hide();

    connectItem_    = menu_->insertItem( MAKE_ICON( connected ), tools::translate( "ConnectionMenu", "&Connect..." ), this, SLOT( Connect() ) );
    recentItem_     = menu_->insertItem( tools::translate( "ConnectionMenu", "Recents" ), hosts_ );
    menu_->insertSeparator();
    disconnectItem_ = menu_->insertItem( MAKE_ICON( notconnected ), tools::translate( "ConnectionMenu", "&Disconnect..." ), this, SLOT( Disconnect() ) );
    menu_->setItemEnabled( disconnectItem_, false );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ConnectionMenu destructor
// Created: SBO 2008-09-18
// -----------------------------------------------------------------------------
ConnectionMenu::~ConnectionMenu()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ConnectionMenu::Connect
// Created: SBO 2008-09-18
// -----------------------------------------------------------------------------
void ConnectionMenu::Connect()
{
    connectDialog_->exec();
}

// -----------------------------------------------------------------------------
// Name: ConnectionMenu::Disconnect
// Created: SBO 2008-09-18
// -----------------------------------------------------------------------------
void ConnectionMenu::Disconnect()
{
    disconnectDialog_->show();
}

// -----------------------------------------------------------------------------
// Name: ConnectionMenu::NotifyUpdated
// Created: SBO 2008-09-18
// -----------------------------------------------------------------------------
void ConnectionMenu::NotifyUpdated( const Simulation& simulation )
{
    menu_->setItemEnabled( connectItem_   , !simulation.IsConnected() );
    menu_->setItemEnabled( recentItem_    , !simulation.IsConnected() );
    menu_->setItemEnabled( disconnectItem_,  simulation.IsConnected() );
}
