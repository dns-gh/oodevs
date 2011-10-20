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

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: EventToolbar constructor
// Created: SBO 2006-06-20
// -----------------------------------------------------------------------------
EventToolbar::EventToolbar( QMainWindow* pParent, Controllers& controllers, const kernel::Profile_ABC& profile )
    : QToolBar( "event toolbar", pParent )
    , controllers_( controllers )
    , profile_( profile )
{
    setObjectName( "eventToolBar" );
    setWindowTitle( tr( "Messages" ) );
    gasButton_ = new QToolButton( MAKE_ICON( gas ), tr( "Out of gas" ), "", this, SLOT( GasClicked() ), this );
    conflictButton_ = new QToolButton( MAKE_ICON( ammo ), tr( "Conflicts" ), "", this, SLOT( ConflictClicked() ), this );
    messageButton_ = new QToolButton( MAKE_ICON( msg ), "0", "", this, SLOT( MessageClicked() ), this );

    addWidget( gasButton_ );
    addWidget( conflictButton_ );
    addWidget( messageButton_ );

    messageButton_->setUsesTextLabel( true );
    messageButton_->setTextPosition( QToolButton::BesideIcon );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: EventToolbar destructor
// Created: SBO 2006-06-20
// -----------------------------------------------------------------------------
EventToolbar::~EventToolbar()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: EventToolbar::GasClicked
// Created: SBO 2006-06-26
// -----------------------------------------------------------------------------
void EventToolbar::GasClicked()
{
    // $$$$ AGE 2006-10-26: TODO !
}

// -----------------------------------------------------------------------------
// Name: EventToolbar::ConflictClicked
// Created: SBO 2006-06-26
// -----------------------------------------------------------------------------
void EventToolbar::ConflictClicked()
{
    // $$$$ AGE 2006-10-26: TODO !
}

// -----------------------------------------------------------------------------
// Name: EventToolbar::MessageClicked
// Created: SBO 2006-06-26
// -----------------------------------------------------------------------------
void EventToolbar::MessageClicked()
{
    if( messageAgents_.empty() )
        return;
    const Entity_ABC* entity = messageAgents_.front();
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
    const Entity_ABC& entity = report.GetOwner();
    if( profile_.IsVisible( entity ) )
    {
        const CIT_Agents it = std::find( messageAgents_.begin(), messageAgents_.end(), &entity );
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
void EventToolbar::NotifyUpdated( const Profile_ABC& )
{
    T_Agents filtered;
    for( CIT_Agents it = messageAgents_.begin(); it != messageAgents_.end(); ++it )
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
    IT_Agents it = std::find( messageAgents_.begin(), messageAgents_.end(), &entity );
    if( it != messageAgents_.end() )
        messageAgents_.erase( it );
}

// -----------------------------------------------------------------------------
// Name: EventToolbar::UpdateMessageButton
// Created: AGE 2006-10-26
// -----------------------------------------------------------------------------
void EventToolbar::UpdateMessageButton()
{
    messageButton_->setTextLabel( QString::number( messageAgents_.size() ) );
    QToolTip::add( messageButton_, tr( "%1 pending messages" ).arg( messageAgents_.size() ) );
}
