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
        bool        threads;
        unsigned    first_tick;
        unsigned    last_tick;
        int         valid_frames;
        int         num_msg;
        int         num_keymsg;
    };

    const TestCase tests[] =
    {
        { "invalid_path",       false, ~0u,  0, 0,   0,   0 },
        { "just_5",             false,   1,  5, 5, 256, 461 },
        { "just_5_offset",      false,  91, 95, 5, 170, 773 },
        { "single_current",     false,   1,  5, 5, 256, 461 },
        { "truncated_info",     false,   1,  5, 5, 256, 461 },
        { "truncated_index",    false,   1,  5, 1, 130, 461 },
        { "truncated_update",   false,   1,  5, 5,   0, 461 },
        { "truncated_key",      false,   1,  5, 5, 256,   0 },
        { "truncated_keyindex", false,   1,  5, 5, 256,   0 },
        { "invalid_path",       true,  ~0u,  0, 0,   0,   0 },
        { "just_5",             true,    1,  5, 5, 130,   0 },
        { "just_5_offset",      true,   91, 95, 5,  55,   0 },
        { "single_current",     true,    1,  5, 5, 130,   0 },
        { "truncated_info",     true,    1,  5, 5, 130,   0 },
        { "truncated_index",    true,    1,  5, 5, 130,   0 },
        { "truncated_update",   true,    1,  5, 5,   0,   0 },
        { "truncated_key",      true,    1,  5, 5, 130,   0 },
        { "truncated_keyindex", true,    1,  5, 5, 130,   0 },
    };

    #define COUNT_OF( X ) (sizeof(X)/sizeof*(X))
}

BOOST_FIXTURE_TEST_CASE( message_loader_loads_records, Fixture )
{
    for( size_t i = 0; i < COUNT_OF( tests ); ++i )
    {
        const TestCase& test = tests[i];
        MockMessageHandler msg, keymsg;
        MessageLoader loader( BOOST_RESOLVE( test.dir ), test.threads, 0 );
        BOOST_CHECK_EQUAL( loader.GetFirstTick(),  test.first_tick );
        BOOST_CHECK_EQUAL( loader.GetTickNumber(), test.last_tick );
        LoadAllFrames( loader, test.valid_frames,
                       msg,    test.num_msg,
                       keymsg, test.num_keymsg );
    }
}
