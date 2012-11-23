// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "dispatcher_test_pch.h"
#include "dispatcher/MessageLoader.h"
#include "dispatcher/MessageHandler_ABC.h"
#include "MT_Tools/MT_LogManager.h"

using namespace dispatcher;
namespace bfs = boost::filesystem;

namespace
{

    MOCK_BASE_CLASS( MockMessageHandler, MessageHandler_ABC )
    {
        MOCK_METHOD_EXT( Receive, 1, void ( const sword::SimToClient& ), OnReceiveMessageSimToClient );
    };

    struct Fixture
    {
        Fixture()
        {
            // NOTHING
        }
        ~Fixture()
        {
            MT_LogManager::Cleanup();
        }
    };

    void LoadAllFrames( MessageLoader& loader,      int num_valid,
                        MockMessageHandler& msg,    int num_msg,
                        MockMessageHandler& keymsg, int num_keymsg )
    {
        MOCK_EXPECT( msg.OnReceiveMessageSimToClient ).exactly( num_msg );
        MOCK_EXPECT( keymsg.OnReceiveMessageSimToClient ).exactly( num_keymsg );
        for( unsigned i = loader.GetFirstTick(); i <= loader.GetTickNumber(); ++i )
        {
            bool valid = loader.LoadFrame( i, msg );
            if( valid )
                --num_valid;
            loader.LoadKeyFrame( i, keymsg );
        }
        BOOST_CHECK_EQUAL( num_valid, 0 );
    }

    struct TestCase
    {
        std::string dir;
        unsigned    first_tick;
        unsigned    last_tick;
        int         valid_frames;
        int         num_msg;
        int         num_keymsg;
    };

    const TestCase default_tests[] =
    {
        { "invalid_path",       ~0u,  0, 0,   0,   0 },
        { "just_5",               1,  5, 5, 256, 461 },
        { "just_5_offset",       91, 95, 5, 170, 773 },
        { "single_current",       1,  5, 5, 256, 461 },
        { "truncated_info",       1,  5, 5, 256, 461 },
        { "truncated_index",      1,  5, 1, 130, 461 },
        { "truncated_update",     1,  5, 5,   0, 461 },
        { "truncated_key",        1,  5, 5, 256,   0 },
        { "truncated_keyindex",   1,  5, 5, 256,   0 },
    };

    const TestCase thread_tests[] =
    {
        { "invalid_path",       ~0u,  0, 0,   0,    0 },
        { "just_5",               1,  5, 5, 256, 1844 },
        { "just_5_offset",       91, 95, 5, 170, 3092 },
        { "single_current",       1,  5, 5, 256, 1844 },
        { "truncated_info",       1,  5, 5, 256, 1844 },
        { "truncated_index",      1,  5, 5, 130, 1844 },
        { "truncated_update",     1,  5, 5,   0, 1844 },
        { "truncated_key",        1,  5, 5, 256,    0 },
        { "truncated_keyindex",   1,  5, 5, 256,    0 },
    };

    #define COUNT_OF( X ) (sizeof(X)/sizeof*(X))

    void TestRecords( const TestCase* tests, size_t size, bool thread )
    {
        for( size_t i = 0; i < size; ++i )
        {
            const TestCase& test = tests[i];
            MockMessageHandler msg, keymsg;
            MessageLoader loader( BOOST_RESOLVE( test.dir ), thread, 0 );
            BOOST_CHECK_EQUAL( loader.GetFirstTick(),  test.first_tick );
            BOOST_CHECK_EQUAL( loader.GetTickNumber(), test.last_tick );
            LoadAllFrames( loader, test.valid_frames,
                           msg,    test.num_msg,
                           keymsg, test.num_keymsg );
        }
    }
}

BOOST_FIXTURE_TEST_CASE( message_loader_loads_records, Fixture )
{
    TestRecords( default_tests, COUNT_OF( default_tests ), false );
}

BOOST_FIXTURE_TEST_CASE( message_loader_loads_records_with_threads, Fixture )
{
    TestRecords( thread_tests, COUNT_OF( thread_tests ), true);
}
