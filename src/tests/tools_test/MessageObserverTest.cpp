// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "tools_test_pch.h"
#include "tools/MessageController.h"
#include "tools/MessageObserver.h"

using namespace tools;

namespace
{
    struct FirstMessage {};
    struct SecondMessage {};
    struct Category
    {
      bool has_first_message() const
      {
          return true;
      }
      const FirstMessage& first_message() const
      {
          return firstMessage_;
      }
      bool has_second_message() const
      {
          return true;
      }
      const SecondMessage& second_message() const
      {
          return secondMessage_;
      }
    private:
        FirstMessage firstMessage_;
        SecondMessage secondMessage_;
    };

    class MockMessageObserver : public MessageObserver< FirstMessage >
                              , public MessageObserver< SecondMessage >
    {
    public:
        MOCK_METHOD_EXT( Notify, 2, void( const FirstMessage&, int ), NotifyFirst );
        MOCK_METHOD_EXT( Notify, 2, void( const SecondMessage&, int ), NotifySecond );
    };
}

BOOST_AUTO_TEST_CASE( observer_connects_to_controller_and_receives_messages )
{
    MessageController< Category > controller;
    MockMessageObserver observer;
    CONNECT( controller, observer, first_message );
    CONNECT( controller, observer, second_message );
    mock::sequence s;
    MOCK_EXPECT( observer, NotifyFirst ).once().in( s ).with( mock::any, 42 );
    MOCK_EXPECT( observer, NotifySecond ).once().in( s ).with( mock::any, 42 );
    Category category;
    controller.Dispatch( category, 42 );
}

BOOST_AUTO_TEST_CASE( context_is_negative_by_default )
{
    MessageController< Category > controller;
    MockMessageObserver observer;
    CONNECT( controller, observer, first_message );
    mock::sequence s;
    MOCK_EXPECT( observer, NotifyFirst ).once().in( s ).with( mock::any, -1 );
    Category category;
    controller.Dispatch( category );
}

BOOST_AUTO_TEST_CASE( observer_can_explicitly_disconnect_from_controller )
{
    MessageController< Category > controller;
    MockMessageObserver observer;
    CONNECT( controller, observer, first_message );
    CONNECT( controller, observer, second_message );
    DISCONNECT( controller, observer, first_message );
    MOCK_EXPECT( observer, NotifySecond ).once();
    Category category;
    controller.Dispatch( category, 42 );
}

BOOST_AUTO_TEST_CASE( observer_can_connect_to_multiple_controllers )
{
    MessageController< Category > controller1;
    MessageController< Category > controller2;
    MockMessageObserver observer;
    Category category;
    CONNECT( controller1, observer, first_message );
    CONNECT( controller2, observer, first_message );
    MOCK_EXPECT( observer, NotifyFirst ).once();
    controller1.Dispatch( category );
    MOCK_EXPECT( observer, NotifyFirst ).once();
    controller2.Dispatch( category, 42 );
}

BOOST_AUTO_TEST_CASE( observer_can_disconnect_from_one_specific_controller )
{
    MessageController< Category > controller1;
    MessageController< Category > controller2;
    MockMessageObserver observer;
    Category category;
    CONNECT( controller1, observer, first_message );
    CONNECT( controller2, observer, first_message );
    DISCONNECT( controller1, observer, first_message );
    controller1.Dispatch( category );
    MOCK_EXPECT( observer, NotifyFirst ).once();
    controller2.Dispatch( category, 42 );
}

namespace
{
    template< typename Category >
    MOCK_BASE_CLASS( MockMessageController, MessageController_ABC< Category > )
    {
        MOCK_METHOD_EXT_TPL( Register, 1, void( MessageHandler_ABC< Category >& ), Register )
        MOCK_METHOD_EXT_TPL( Unregister, 1, void( MessageHandler_ABC< Category >& ), Unregister )
        MOCK_METHOD_EXT_TPL( Dispatch, 2, void( const Category&, int ), Dispatch )
    };
    class Observer : private MessageObserver< FirstMessage >
    {
    public:
        explicit Observer( MessageController_ABC< Category >& controller )
        {
            CONNECT( controller, *this, first_message );
        }
    private:
        virtual void Notify( const FirstMessage& /*message*/, int /*context*/ )
        {
            // NOTHING
        }
    };
}

BOOST_AUTO_TEST_CASE( observer_automatically_disconnect_at_destruction )
{
    MockMessageController< Category > controller;
    mock::sequence s;
    MOCK_EXPECT( controller, Register ).once().in( s );
    MOCK_EXPECT( controller, Unregister ).once().in( s );
    {
        Observer observer( controller );
    }
}
