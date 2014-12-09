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
#include "clients_gui/resources.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Profile_ABC.h"
#include "gaming/AgentsModel.h"
#include "gaming/ReportsModel.h"
#include "gaming/statusicons.h"
#include "protocol/ServerPublisher_ABC.h"

// -----------------------------------------------------------------------------
// Name: EventToolbar constructor
// Created: SBO 2006-06-20
// -----------------------------------------------------------------------------
EventToolbar::EventToolbar( QMainWindow* pParent, kernel::Controllers& controllers, ReportsModel& model,
                            const AgentsModel& agents, Publisher_ABC& publisher )
    : gui::RichToolBar( controllers, pParent, "event toolbar" )
    , controllers_( controllers )
    , model_      ( model )
    , agents_     ( agents )
{
    setObjectName( "eventToolBar" );
    setWindowTitle( tr( "Notifications" ) );
    messageButton_ = new QToolButton( MAKE_ICON( msg ), "0", "", this, SLOT( MessageClicked() ), this );

    addWidget( messageButton_ );

    messageButton_->setUsesTextLabel( true );
    messageButton_->setTextPosition( QToolButton::BesideIcon );
    controllers_.Update( *this );

    publisher.Register( Publisher_ABC::T_SimHandler( [&]( const sword::SimToClient& message, bool /*ack*/ )
    {
        if( !message.message().has_report() )
            return;
        UpdateMessageButton();
    } ) );
    publisher.Register( Publisher_ABC::T_ReplayHandler( [&]( const sword::ReplayToClient& message )
    {
        if( !message.message().has_list_reports_ack() )
            return;
        UpdateMessageButton();
    } ) );
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
    if( !model_.UnreadReports() )
        return;
    const auto id = model_.NextUnreadReports();
    const auto* entity = agents_.FindAllAgent( id );
    if( !entity )
        return;
    entity->Select( controllers_.actions_ );
    entity->Activate( controllers_.actions_ );
    model_.ReadReports();
    UpdateMessageButton();
}

// -----------------------------------------------------------------------------
// Name: EventToolbar::NotifyUpdated
// Created: AGE 2006-10-26
// -----------------------------------------------------------------------------
void EventToolbar::NotifyUpdated( const kernel::Profile_ABC& )
{
    model_.UpdateUnreadReports();
    UpdateMessageButton();
}

void EventToolbar::NotifySelected( const kernel::Entity_ABC* /*element*/ )
{
    UpdateMessageButton();
}

// -----------------------------------------------------------------------------
// Name: EventToolbar::UpdateMessageButton
// Created: AGE 2006-10-26
// -----------------------------------------------------------------------------
void EventToolbar::UpdateMessageButton()
{
    const auto size = model_.UnreadReports();
    messageButton_->setTextLabel( locale().toString( size) );
    QToolTip::add( messageButton_, tr( "%L1 pending messages" ).arg( size ) );
}
