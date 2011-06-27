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

BOOST_AUTO_TEST_CASE( message_handling_with_controller_and_observer )
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
