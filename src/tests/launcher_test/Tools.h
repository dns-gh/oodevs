// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#pragma warning( push, 0 )
#include <qapplication.h>
#pragma warning( pop )

namespace launcher_test
{
    const std::string   defaultHost = "127.0.0.1";
    const unsigned int  timeOut     = 50000;

    struct Timeout : private boost::noncopyable
    {
        explicit Timeout( unsigned int duration )
            : duration_( duration )
        {
            Start();
        }
        void Start()
        {
            start_ = boost::posix_time::microsec_clock::universal_time();
        }
        bool Expired() const
        {
            return ( boost::posix_time::microsec_clock::universal_time() - start_ ).total_milliseconds() > duration_;
        }
        const unsigned int duration_;
        boost::posix_time::ptime start_;
    };

    MOCK_BASE_CLASS( MockConnectionHandler, frontend::ConnectionHandler_ABC )
    {
        MOCK_METHOD( OnConnectionSucceeded, 0 );
        MOCK_METHOD( OnConnectionFailed, 1 );
        MOCK_METHOD( OnConnectionLost, 1 );
        MOCK_METHOD( OnError, 1 );
    };
    MOCK_BASE_CLASS( MockDispatcher, tools::ServerNetworker )
    {
        MockDispatcher(unsigned short port)
            : tools::ServerNetworker( port )
        {
            AllowConnections();
            RegisterMessage<MockDispatcher,sword::ClientToSim>( *this, &MockDispatcher::Receive );
            RegisterMessage<MockDispatcher,sword::ClientToAuthentication>( *this, &MockDispatcher::Receive );
        }
        MOCK_METHOD_EXT( Receive, 2, void( const std::string&, const sword::ClientToSim& ), ReceiveSim );
        MOCK_METHOD_EXT( Receive, 2, void( const std::string&, const sword::ClientToAuthentication& ), ReceiveAuth );
        MOCK_METHOD_EXT( ConnectionSucceeded, 1, void( const std::string& ), ConnectionSucceeded );
        MOCK_METHOD_EXT( ConnectionFailed, 2, void( const std::string&, const std::string& ), ConnectionFailed );
        MOCK_METHOD_EXT( ConnectionError, 2, void( const std::string&, const std::string& ), ConnectionError );

        template <typename M>
        void Send(M& msg)
        {
            ServerNetworker::Send(host, msg);
        }

        bool AuthenticationPerformed() const
        {
            return host.length() != 0 && authMessage.IsInitialized();
        }
        std::string host;
        sword::ClientToAuthentication authMessage;
    };

    struct ApplicationFixture
    {
        ApplicationFixture()
            : varg( MakeArg() )
            , args( boost::assign::list_of< char* >( "" )( "--root-dir=../../data" )("--test")( &varg[0] ) )
            , argc( args.size() )
            , app( argc, &args[0] )
        {}
        std::vector< char > MakeArg()
        {
            const std::string arg( "--launcher-port=" + boost::lexical_cast< std::string >( PORT ) );
            std::vector< char > result( arg.begin(), arg.end() );
            result.push_back( 0 );
            return result;
        }
        std::string arg;
        std::vector< char > varg;
        std::vector< char* > args;
        int argc;
        QApplication app;
    };
    struct Fixture : ApplicationFixture
    {
        Fixture()
            : client  ( controllers.controller_ )
            , timeout ( timeOut )
            , dispatcher( frontend::DispatcherPort( 1 ) )
        {
            launcher.Initialize( argc, &args[0] );
            BOOST_REQUIRE_MESSAGE( launcher.GetLastError().empty(), launcher.GetLastError() );
            MOCK_EXPECT( handler, OnConnectionSucceeded ).once();
            client.Connect( defaultHost, PORT, handler );
            while( !client.Connected() && !timeout.Expired() )
            {
                Update();
            }
        }
        void Update()
        {
            client.Update();
            launcher.Update();
            dispatcher.Update();
        }

        LauncherFacade launcher;
        kernel::Controllers controllers;
        frontend::LauncherClient client;
        MockConnectionHandler handler;
        Timeout timeout;
        MockDispatcher dispatcher;
    };
}

#define LAUNCHER_CHECK_MESSAGE(MSG, EXPECTED) BOOST_CHECK_EQUAL(MSG.ShortDebugString(), EXPECTED)