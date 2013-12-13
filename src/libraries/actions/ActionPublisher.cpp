// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "actions_pch.h"
#include "ActionPublisher.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/Time_ABC.h"
#include "clients_kernel/TimelineHandler_ABC.h"
#include "clients_kernel/TimelineHelpers.h"
#include "protocol/Protocol.h"
#include <timeline/api.h>

#include <boost/lexical_cast.hpp>
#pragma warning( push )
#pragma warning( disable : 4724 )
#include <boost/uuid/random_generator.hpp>
#pragma warning( pop )
#include <boost/uuid/uuid_io.hpp>

using namespace actions;

// -----------------------------------------------------------------------------
// Name: ActionPublisher constructor
// Created: SBO 2010-03-17
// -----------------------------------------------------------------------------
ActionPublisher::ActionPublisher( Publisher_ABC& publisher,
                                  kernel::Controllers& controllers,
                                  const kernel::Time_ABC& simulation )
    : controllers_( controllers )
    , publisher_( publisher )
    , simulation_( simulation )
    , design_( false )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ActionPublisher destructor
// Created: SBO 2010-03-17
// -----------------------------------------------------------------------------
ActionPublisher::~ActionPublisher()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ActionPublisher::SetTimelineHandler
// Created: ABR 2013-12-13
// -----------------------------------------------------------------------------
void ActionPublisher::SetTimelineHandler( const boost::shared_ptr< kernel::TimelineHandler_ABC >& handler )
{
    handler_ = handler;
}

namespace
{
    template< typename T >
    void SendToTimeline( kernel::TimelineHandler_ABC& handler, const std::string& time, const sword::ClientToSim& msg, const T& actionMsg )
    {
        timeline::Event event;
        msg.SerializePartialToString( &event.action.payload );
        event.uuid = boost::lexical_cast< std::string >( boost::uuids::random_generator()() );
        event.name = actionMsg.name();
        event.begin = time;
        event.action.target = CREATE_EVENT_TARGET( EVENT_ORDER_PROTOCOL, EVENT_SIMULATION_SERVICE );
        event.action.apply = true;
        event.done = true;
        handler.CreateEvent( event );
    };
}

// -----------------------------------------------------------------------------
// Name: ActionPublisher::Send
// Created: SBO 2010-03-17
// -----------------------------------------------------------------------------
void ActionPublisher::Send( const sword::ClientToSim& msg )
{
    if( !design_ )
        publisher_.Send( msg );
    if( handler_ )
        return;
    const std::string currentTime = simulation_.GetDateTime().toString( EVENT_DATE_FORMAT ).toStdString();
    if( msg.message().has_magic_action() )
        SendToTimeline( *handler_, currentTime, msg, msg.message().magic_action() );
    else if( msg.message().has_unit_magic_action() )
        SendToTimeline( *handler_, currentTime, msg, msg.message().unit_magic_action() );
    else if( msg.message().has_object_magic_action() )
        SendToTimeline( *handler_, currentTime, msg, msg.message().object_magic_action() );
    else if( msg.message().has_knowledge_magic_action() )
        SendToTimeline( *handler_, currentTime, msg, msg.message().knowledge_magic_action() );
    else if( msg.message().has_set_automat_mode() )
        SendToTimeline( *handler_, currentTime, msg, msg.message().set_automat_mode() );
}

// -----------------------------------------------------------------------------
// Name: ActionPublisher::Register
// Created: LGY 2013-12-10
// -----------------------------------------------------------------------------
void ActionPublisher::Register( T_Handler /*handler*/ )
{
    throw MASA_EXCEPTION( "not to be called." );;
}

// -----------------------------------------------------------------------------
// Name: ActionPublisher::Send
// Created: SBO 2010-03-17
// -----------------------------------------------------------------------------
void ActionPublisher::Send( const sword::ClientToAuthentication& /*message*/ )
{
    throw MASA_EXCEPTION( "not to be called." );
}

// -----------------------------------------------------------------------------
// Name: ActionPublisher::Send
// Created: SBO 2010-03-17
// -----------------------------------------------------------------------------
void ActionPublisher::Send( const sword::ClientToReplay& /*message*/ )
{
    throw MASA_EXCEPTION( "not to be called." );
}

// -----------------------------------------------------------------------------
// Name: ActionPublisher::Send
// Created: SBO 2010-03-17
// -----------------------------------------------------------------------------
void ActionPublisher::Send( const sword::ClientToAar& /*message*/ )
{
    throw MASA_EXCEPTION( "not to be called." );
}

// -----------------------------------------------------------------------------
// Name: ActionPublisher::Send
// Created: SBO 2010-03-17
// -----------------------------------------------------------------------------
void ActionPublisher::Send( const sword::ClientToMessenger& /*message*/ )
{
    throw MASA_EXCEPTION( "not to be called." );
}

// -----------------------------------------------------------------------------
// Name: ActionPublisher::NotifyModeChanged
// Created: LGY 2012-06-22
// -----------------------------------------------------------------------------
void ActionPublisher::NotifyModeChanged( E_Modes newMode )
{
    design_ = newMode == eModes_Planning;
}

