// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "EventToolbar.h"
#include "moc_EventToolbar.cpp"
#include "Controllers.h"
#include "ActionController.h"
#include "Report_ABC.h"
#include "Agent.h"
#include "Population.h"

// -----------------------------------------------------------------------------
// Name: EventToolbar constructor
// Created: SBO 2006-06-20
// -----------------------------------------------------------------------------
EventToolbar::EventToolbar( QMainWindow* pParent, Controllers& controllers )
    : QToolBar( pParent, "event toolbar" )
    , controllers_( controllers )
{
    setLabel( tr( "Messagerie" ) );
    gasButton_ = new QToolButton( MAKE_ICON( gas ), tr( "Pannes d'essence" ), "", this, SLOT( GasClicked() ), this );
    conflictButton_ = new QToolButton( MAKE_ICON( ammo ), tr( "Conflits" ), "", this, SLOT( ConflictClicked() ), this );
    messageButton_ = new QToolButton( MAKE_ICON( msg ), tr( "Messages" ), "", this, SLOT( MessageClicked() ), this );
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
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: EventToolbar::GasClicked
// Created: SBO 2006-06-26
// -----------------------------------------------------------------------------
void EventToolbar::GasClicked()
{
    
}

// -----------------------------------------------------------------------------
// Name: EventToolbar::ConflictClicked
// Created: SBO 2006-06-26
// -----------------------------------------------------------------------------
void EventToolbar::ConflictClicked()
{

}

// -----------------------------------------------------------------------------
// Name: EventToolbar::MessageClicked
// Created: SBO 2006-06-26
// -----------------------------------------------------------------------------
void EventToolbar::MessageClicked()
{
    if( messageAgents_.empty() )
        return;
    const Agent* agent = dynamic_cast< const Agent* >( messageAgents_.front() );
    if( agent )
    {
        controllers_.actions_.Select( *agent );
        controllers_.actions_.Activate( *agent );
    }
    else
    {
        const Population* popu = static_cast< const Population* >( messageAgents_.front() );
        controllers_.actions_.Select( *popu );
        controllers_.actions_.Activate( *popu );
    }
    messageAgents_.pop_front();
    messageButton_->setTextLabel( QString::number( messageAgents_.size() ) );
}

// -----------------------------------------------------------------------------
// Name: EventToolbar::NotifyCreated
// Created: SBO 2006-06-26
// -----------------------------------------------------------------------------
void EventToolbar::NotifyCreated( const Report_ABC& report )
{
    CIT_Agents it = std::find( messageAgents_.begin(), messageAgents_.end(), &report.GetAgent() );
    if( it != messageAgents_.end() )
        return;
    messageAgents_.push_back( &report.GetAgent() );
    messageButton_->setTextLabel( QString::number( messageAgents_.size() ) );
}
