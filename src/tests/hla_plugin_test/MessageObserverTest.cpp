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
    class ControlTickObserver : private MessageObserver< sword::SimToClient_Content, sword::ControlBeginTick >
                              , private MessageObserver< sword::SimToClient_Content, sword::ControlEndTick >
    {
    public:
        explicit ControlTickObserver( MessageController< sword::SimToClient_Content >& controller )
            : MESSAGE_OBSERVER( sword::SimToClient_Content, sword::ControlBeginTick, control_begin_tick, controller )
            , MESSAGE_OBSERVER( sword::SimToClient_Content, sword::ControlEndTick, control_end_tick, controller )
        {
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

BOOST_AUTO_TEST_CASE( event_controller_is_cool )
{
    MessageController< sword::SimToClient_Content > controller;
    ControlTickObserver receiver( controller );
    sword::SimToClient message;
    message.mutable_message()->mutable_control_begin_tick()->set_current_tick( 3 );
    message.mutable_message()->mutable_control_end_tick()->set_current_tick( 3 );
    controller.Notify( message.message() );
}
