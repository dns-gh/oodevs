// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "launcher_dll_pch.h"
#include "ControlEndTickMessageHandler.h"

using namespace launcher;

// -----------------------------------------------------------------------------
// Name: ControlEndTickMessageHandler constructor
// Created: LGY 2011-09-07
// -----------------------------------------------------------------------------
ControlEndTickMessageHandler::ControlEndTickMessageHandler( LauncherPublisher& publisher, const std::string& exercise, const std::string& session )
    : ClientMessageHandlerBase( publisher, exercise, session )
    , timeFactor_( 0 )
    , checkpoint_( 0 )
    , tick_      ( 0 )
    , publisher_ ( publisher )
{
    // NOTHING
}
// -----------------------------------------------------------------------------
// Name: ControlInformationMessageHandler destructor
// Created: LGY 2011-09-07
// -----------------------------------------------------------------------------
ControlEndTickMessageHandler::~ControlEndTickMessageHandler()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ControlInformationMessageHandler::OnReceiveMessage
// Created: LGY 2011-09-07
// -----------------------------------------------------------------------------
bool ControlEndTickMessageHandler::OnReceiveMessage( const sword::SimToClient& message )
{
    if( message.message().has_control_information() )
        timeFactor_ = message.message().control_information().time_factor();
    if( message.message().has_control_change_time_factor_ack() )
        timeFactor_ = message.message().control_change_time_factor_ack().time_factor();
    if( message.message().has_control_checkpoint_save_end() )
        checkpoint_ = tick_;
    if( message.message().has_control_end_tick() )
    {
        const sword::ControlEndTick& control = message.message().control_end_tick();
        unsigned long pathfinds = control.long_pathfinds() + control.short_pathfinds();
        tick_ = control.current_tick();
        TickInformation response;
        response().set_current_tick( control.current_tick() );
        response().set_tick_duration( control.tick_duration() );
        response().set_time_factor( timeFactor_ );
        response().set_pathfind_request_number( pathfinds );
        if( checkpoint_ != 0 )
            response().set_last_checkpoint_build_duration( checkpoint_ );
        response.Send( publisher_ );
    }
    return false;
}
