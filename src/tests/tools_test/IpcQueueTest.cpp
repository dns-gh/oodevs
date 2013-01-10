// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "tools_test_pch.h"

#include "tools/IpcQueue.h"
#include "tools/IpcHandler_ABC.h"

#include <boost/interprocess/detail/os_thread_functions.hpp>
#include <boost/interprocess/exceptions.hpp>
#include <boost/thread/thread.hpp>

using namespace tools::ipc;
namespace bip = boost::interprocess::ipcdetail;

namespace
{
    MOCK_BASE_CLASS( MockHandler, Handler_ABC )
    {
        MOCK_METHOD( Stop, 0 );
    };

    struct Fixture
    {
        Fixture()
            : queue_( handler_ )
        {
            // NOTHING
        }

        MockHandler handler_;
        Queue queue_;
    };
}

BOOST_FIXTURE_TEST_CASE( ipc_roundtrip_stop, Fixture )
{
    MOCK_EXPECT( handler_.Stop ).once();
    Queue::Send( IPC_COMMAND_STOP, bip::get_current_process_id() );
    boost::this_thread::sleep( boost::posix_time::milliseconds( 300 ) );
}

BOOST_FIXTURE_TEST_CASE( ipc_send_to_invalid_pid_throws, Fixture )
{
    BOOST_CHECK( !Queue::Send( IPC_COMMAND_STOP, bip::get_current_process_id() + 1 ) );
}

namespace
{
void MakeQueue()
{
    MockHandler handler;
    Queue queue( handler );
}
}

BOOST_FIXTURE_TEST_CASE( ipc_queue_instancied_twice_throws, Fixture )
{
    BOOST_CHECK_THROW( MakeQueue(), tools::Exception );
    BOOST_CHECK_THROW( MakeQueue(), tools::Exception );
}