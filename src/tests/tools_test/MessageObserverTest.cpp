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
        MOCK_METHOD_EXT( Notify, 1, void( const FirstMessage& ), NotifyFirst );
        MOCK_METHOD_EXT( Notify, 1, void( const SecondMessage& ), NotifySecond );
    };
}

BOOST_AUTO_TEST_CASE( observer_connects_to_controller_and_receives_messages )
{
    MessageController< Category > dispatcher;
    MockMessageObserver observer;
    CONNECT( dispatcher, observer, first_message );
    CONNECT( dispatcher, observer, second_message );
    mock::sequence s;
    MOCK_EXPECT( observer, NotifyFirst ).once().in( s );
    MOCK_EXPECT( observer, NotifySecond ).once().in( s );
    Category category;
    dispatcher.Dispatch( category );
}

BOOST_AUTO_TEST_CASE( observer_can_explicitly_disconnect_from_controller )
{
    MessageController< Category > dispatcher;
    MockMessageObserver observer;
    CONNECT( dispatcher, observer, first_message );
    CONNECT( dispatcher, observer, second_message );
    DISCONNECT( dispatcher, observer, first_message );
    MOCK_EXPECT( observer, NotifySecond ).once();
    Category category;
    dispatcher.Dispatch( category );
}

namespace
{
    template< typename Category >
    MOCK_BASE_CLASS( MockMessageController, MessageController_ABC< Category > )
    {
        MOCK_METHOD_EXT_TPL( Register, 1, void( MessageHandler_ABC< Category >& ), Register )
        MOCK_METHOD_EXT_TPL( Unregister, 1, void( MessageHandler_ABC< Category >& ), Unregister )
        MOCK_METHOD_EXT_TPL( Dispatch, 1, void( const Category& ), Dispatch )
    };
    class Observer : private MessageObserver< FirstMessage >
    {
    public:
        explicit Observer( MessageController_ABC< Category >& controller )
        {
            CONNECT( controller, *this, first_message );
        }
    private:
        virtual void Notify( const FirstMessage& /*message*/ )
        {
            // NOTHING
        }
    };
}

BOOST_AUTO_TEST_CASE( observer_automatically_disconnect_at_destruction )
{
    MockMessageController< Category > dispatcher;
    mock::sequence s;
    MOCK_EXPECT( dispatcher, Register ).once().in( s );
    MOCK_EXPECT( dispatcher, Unregister ).once().in( s );
    {
        Observer observer( dispatcher );
    }
}
