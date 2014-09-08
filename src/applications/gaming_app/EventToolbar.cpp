// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "EventToolbar.h"
#include "moc_EventToolbar.cpp"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Profile_ABC.h"
#include "reports/Report.h"
#include "gaming/statusicons.h"
#include "gaming/Simulation.h"
#include "clients_gui/resources.h"

// -----------------------------------------------------------------------------
// Name: EventToolbar constructor
// Created: SBO 2006-06-20
// -----------------------------------------------------------------------------
EventToolbar::EventToolbar( QMainWindow* pParent, kernel::Controllers& controllers, const kernel::Profile_ABC& profile )
    : gui::RichToolBar( controllers, pParent, "event toolbar" )
    , controllers_( controllers )
    , profile_( profile )
{
    setObjectName( "eventToolBar" );
    setWindowTitle( tr( "Notifications" ) );
    messageButton_ = new QToolButton( MAKE_ICON( msg ), "0", "", this, SLOT( MessageClicked() ), this );

    addWidget( messageButton_ );

    messageButton_->setUsesTextLabel( true );
    messageButton_->setTextPosition( QToolButton::BesideIcon );
    controllers_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: EventToolbar destructor
// Created: SBO 2006-06-20
// -----------------------------------------------------------------------------
EventToolbar::~EventToolbar()
{
    setVisible( false );
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: EventToolbar::MessageClicked
// Created: SBO 2006-06-26
// -----------------------------------------------------------------------------
void EventToolbar::MessageClicked()
{
    if( messageAgents_.empty() )
        return;
    const kernel::Entity_ABC* entity = messageAgents_.front();
    entity->Select( controllers_.actions_ );
    entity->Activate( controllers_.actions_ );
    messageAgents_.pop_front();
    UpdateMessageButton();
}

// -----------------------------------------------------------------------------
// Name: EventToolbar::NotifyCreated
// Created: SBO 2006-06-26
// -----------------------------------------------------------------------------
void EventToolbar::NotifyCreated( const Report& report )
{
    const kernel::Entity_ABC& entity = report.GetOwner();
    if( profile_.IsVisible( entity ) )
    {
        auto it = std::find( messageAgents_.begin(), messageAgents_.end(), &entity );
        if( it == messageAgents_.end() )
        {
            messageAgents_.push_back( &entity );
            UpdateMessageButton();
        }
    }
}

// -----------------------------------------------------------------------------
// Name: EventToolbar::NotifyUpdated
// Created: AGE 2006-10-26
// -----------------------------------------------------------------------------
void EventToolbar::NotifyUpdated( const kernel::Profile_ABC& )
{
    T_Agents filtered;
    for( auto it = messageAgents_.begin(); it != messageAgents_.end(); ++it )
        if( profile_.IsVisible( **it ) )
            filtered.push_back( *it );
    std::swap( filtered, messageAgents_ );
    UpdateMessageButton();
}

// -----------------------------------------------------------------------------
// Name: EventToolbar::NotifyUpdated
// Created: SBO 2006-11-07
// -----------------------------------------------------------------------------
void EventToolbar::NotifyUpdated( const Simulation& simulation )
{
    if( !simulation.IsConnected() )
        messageAgents_.clear();
}

// -----------------------------------------------------------------------------
// Name: EventToolbar::NotifyDeleted
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
void EventToolbar::NotifyDeleted( const kernel::Entity_ABC& entity )
{
    auto it = std::find( messageAgents_.begin(), messageAgents_.end(), &entity );
    if( it != messageAgents_.end() )
        messageAgents_.erase( it );
}

// -----------------------------------------------------------------------------
// Name: EventToolbar::UpdateMessageButton
// Created: AGE 2006-10-26
// -----------------------------------------------------------------------------
void EventToolbar::UpdateMessageButton()
{
    messageButton_->setTextLabel( locale().toString( messageAgents_.size() ) );
    QToolTip::add( messageButton_, tr( "%L1 pending messages" ).arg( messageAgents_.size() ) );
}
