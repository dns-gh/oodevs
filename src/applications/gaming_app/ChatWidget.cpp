// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ChatWidget.h"
#include "moc_ChatWidget.cpp"
#include "ChatRoom.h"
#include "ChatProfiles.h"
#include "gaming/UserProfile.h"
#include "gaming/CommandPublisher.h"
#include "gaming/Command.h"
#include "gaming/CommandHandler.h"
#include "clients_kernel/Controllers.h"

// -----------------------------------------------------------------------------
// Name: ChatWidget constructor
// Created: SBO 2008-06-11
// -----------------------------------------------------------------------------
ChatWidget::ChatWidget( QWidget* parent, kernel::Controllers& controllers, Publisher_ABC& publisher, CommandHandler& handler )
    : QHBox( parent )
    , controllers_( controllers )
    , handler_( handler )
    , publisher_( new CommandPublisher( controllers, publisher ) )
{
    tabs_ = new QTabWidget( this );
    {
        QWidget* tab = new ChatRoom( tabs_, *publisher_, handler_ );
        tabs_->addTab( tab, tr( "General" ) );
    }
    profiles_ = new ChatProfiles( this, controllers );
    setStretchFactor( tabs_, 4 );
    setStretchFactor( profiles_, 1 );
    connect( profiles_, SIGNAL( Selected( const UserProfile& ) ), SLOT( OnProfileSelected( const UserProfile& ) ) );
    handler_.Register( "text", *this );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ChatWidget destructor
// Created: SBO 2008-06-11
// -----------------------------------------------------------------------------
ChatWidget::~ChatWidget()
{
    controllers_.Unregister( *this );
    handler_.Unregister( "text", *this );
}

// -----------------------------------------------------------------------------
// Name: ChatWidget::NotifyDeleted
// Created: SBO 2008-06-11
// -----------------------------------------------------------------------------
void ChatWidget::NotifyDeleted( const UserProfile& profile )
{
    T_Rooms::const_iterator it = rooms_.find( profile.GetLogin() );
    if( it != rooms_.end() )
    {
        tabs_->removePage( it->second );
        delete it->second; // $$$$ SBO 2008-06-11: maybe disable only
    }
}

// -----------------------------------------------------------------------------
// Name: ChatWidget::OnProfileSelected
// Created: SBO 2008-06-11
// -----------------------------------------------------------------------------
void ChatWidget::OnProfileSelected( const UserProfile& profile )
{
    Select( profile.GetLogin() );
}

// -----------------------------------------------------------------------------
// Name: ChatWidget::Select
// Created: AGE 2008-08-14
// -----------------------------------------------------------------------------
void ChatWidget::Select( const QString& name )
{
    ChatRoom*& tab = rooms_[name];
    if( !tab )
    {
        tab = new ChatRoom( tabs_, *publisher_, handler_, name );
        tabs_->addTab( tab, name );
    }
    tabs_->showPage( tab );
}

// -----------------------------------------------------------------------------
// Name: ChatWidget::Receive
// Created: AGE 2008-08-14
// -----------------------------------------------------------------------------
void ChatWidget::Receive( const Command& command )
{
    const QString sender = command.Source().c_str();
    if( !sender.isEmpty() && !command.Target().empty() && sender != publisher_->SelfProfile() )
        Select( sender );  // $$$$ AGE 2008-08-14: reetrance dans CommandHandler.Receive
}
