// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "protocol/Simulation.h"
#include "hla_plugin/MessageDispatcher.h"
#include "hla_plugin/MessageObserver.h"

using namespace plugins::hla;

namespace
{
    class MockMessageObserver : public MessageObserver< sword::ControlBeginTick >
                              , public MessageObserver< sword::ControlEndTick >
    {
    public:
        MOCK_METHOD_EXT( Notify, 1, void( const sword::ControlBeginTick& ), NotifyBegin );
        MOCK_METHOD_EXT( Notify, 1, void( const sword::ControlEndTick& ), NotifyEnd );
    };
}

BOOST_AUTO_TEST_CASE( message_handling_with_controller_and_observer )
{
    MessageDispatcher< sword::SimToClient_Content > dispatcher;
    MockMessageObserver observer;
    CONNECT( dispatcher, observer, control_begin_tick );
    CONNECT( dispatcher, observer, control_end_tick );
    mock::sequence s;
    MOCK_EXPECT( observer, NotifyBegin ).once().in( s );
    MOCK_EXPECT( observer, NotifyEnd ).once().in( s );
    sword::SimToClient_Content message;
    message.mutable_control_begin_tick()->set_current_tick( 3 );
    message.mutable_control_end_tick()->set_current_tick( 3 );
    dispatcher.Dispatch( message );
}
