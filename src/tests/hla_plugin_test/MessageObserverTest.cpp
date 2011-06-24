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
#include "hla_plugin/MessageController.h"
#include "hla_plugin/MessageObserver.h"

using namespace plugins::hla;

namespace
{
    class ControlTickObserver : private MessageObserver< sword::ControlBeginTick >
                              , private MessageObserver< sword::ControlEndTick >
    {
    public:
        explicit ControlTickObserver( MessageController< sword::SimToClient_Content >& controller )
        {
            CONNECT( controller, sword::ControlBeginTick, control_begin_tick );
            CONNECT( controller, sword::ControlEndTick  , control_end_tick );
            MOCK_EXPECT( beginTick, _ ).once().in( s );
            MOCK_EXPECT( endTick, _ ).once().in( s );
        }
        virtual ~ControlTickObserver()
        {}
    private:
        virtual void Notify( const sword::ControlBeginTick& /*event*/ )
        {
            beginTick();
        }
        virtual void Notify( const sword::ControlEndTick& /*event*/ )
        {
            endTick();
        }
    private:
        mock::sequence s;
        MOCK_FUNCTOR( void() ) beginTick;
        MOCK_FUNCTOR( void() ) endTick;
    };
}

BOOST_AUTO_TEST_CASE( message_handling_with_controller_and_observer )
{
    MessageController< sword::SimToClient_Content > controller;
    ControlTickObserver receiver( controller );
    sword::SimToClient message;
    message.mutable_message()->mutable_control_begin_tick()->set_current_tick( 3 );
    message.mutable_message()->mutable_control_end_tick()->set_current_tick( 3 );
    controller.Notify( message.message() );
}
