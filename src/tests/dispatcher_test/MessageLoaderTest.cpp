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
#include <tools/Helpers.h>

using namespace dispatcher;

namespace
{

    MOCK_BASE_CLASS( MockMessageHandler, MessageHandler_ABC )
    {
        MOCK_METHOD( Receive, 1, void ( const sword::SimToClient& ), OnReceiveMessageSimToClient );
    };

    struct Fixture
    {
        Fixture()
        {
            // NOTHING
        }
        ~Fixture()
        {
            MT_LogManager::Instance().Reset();
        }
    };

    void LoadAllFrames( MessageLoader& loader,      int num_valid,
                        MockMessageHandler& msg,    int num_msg,
                        MockMessageHandler& keymsg, int num_keymsg )
    {
        MOCK_EXPECT( msg.OnReceiveMessageSimToClient ).exactly( num_msg );
        MOCK_EXPECT( keymsg.OnReceiveMessageSimToClient ).exactly( num_keymsg );
        std::set< unsigned > keyframes;
        for( unsigned i = loader.GetFirstTick(); i <= loader.GetTickNumber(); ++i )
            keyframes.insert( loader.FindKeyFrame( i ) );
        for( unsigned i = loader.GetFirstTick(); i <= loader.GetTickNumber(); ++i )
        {
            if( keyframes.count( i ) )
                loader.LoadKeyFrame( i, keymsg );
            if( loader.LoadFrame( i, msg ) )
                --num_valid;
        }
        BOOST_CHECK_EQUAL( num_valid, 0 );
    }

    struct TestCase
    {
        tools::Path dir;
        unsigned    first_tick;
        unsigned    last_tick;
        int         valid_frames;
        int         num_msg;
        int         num_keymsg;
    };

    const TestCase default_tests[] =
    {
        { "invalid_path",  UINT_MAX,  0, 0,   0,   0 },
        { "just_5",               1,  5, 5, 763, 461 },
        { "just_5_offset",       91, 95, 5, 224, 773 },
        { "single_current",       1,  5, 5, 763, 461 },
        { "truncated_info",       1,  5, 5, 763, 461 },
        { "truncated_index",      1,  5, 2, 637, 461 },
        { "truncated_update",     1,  5, 5,   0, 461 },
        { "truncated_key",        1,  5, 5, 763,   0 },
        { "truncated_keyindex",   1,  5, 5, 763,   0 },
    };

    const TestCase thread_tests[] =
    {
        { "invalid_path",  UINT_MAX,  0, 0,   0,   0 },
        { "just_5",               1,  5, 5, 763, 461 },
        { "just_5_offset",       91, 95, 5, 224, 773 },
        { "single_current",       1,  5, 5, 763, 461 },
        { "truncated_info",       1,  5, 5, 763, 461 },
        { "truncated_index",      1,  5, 5, 637, 461 },
        { "truncated_update",     1,  5, 5,   0, 461 },
        { "truncated_key",        1,  5, 5, 763,   0 },
        { "truncated_keyindex",   1,  5, 5, 763,   0 },
    };

    void TestRecords( const TestCase* tests, size_t size, bool thread )
    {
        for( size_t i = 0; i < size; ++i )
        {
            const TestCase& test = tests[i];
            BOOST_REQUIRE( !test.valid_frames || BOOST_RESOLVE( test.dir ).Exists() );
            MockMessageHandler msg, keymsg;
            MessageLoader loader( BOOST_RESOLVE( test.dir ), thread, 10, nullptr );
            BOOST_CHECK_EQUAL( loader.GetFirstTick(),  test.first_tick );
            BOOST_CHECK_EQUAL( loader.GetTickNumber(), test.last_tick );
            LoadAllFrames( loader, test.valid_frames,
                           msg,    test.num_msg,
                           keymsg, test.num_keymsg );
        }
    }
}

//BOOST_FIXTURE_TEST_CASE( message_loader_loads_records, Fixture )
//{
//    TestRecords( default_tests, COUNT_OF( default_tests ), false );
//}
//
//BOOST_FIXTURE_TEST_CASE( message_loader_loads_records_with_threads, Fixture )
//{
//    TestRecords( thread_tests, COUNT_OF( thread_tests ), true);
//}
