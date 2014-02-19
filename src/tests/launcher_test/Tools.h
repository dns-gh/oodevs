// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#pragma warning( push, 0 )
#include <QtGui/qapplication.h>
#pragma warning( pop )
#include "clients_kernel/Controllers.h"
#include "frontend/Config.h"
#include "frontend/Exercise_ABC.h"
#include "frontend/LauncherClient.h"
#include "frontend/CommandLineTools.h"
#include "launcher_dll/LauncherFacade.h"
#include "protocol/LauncherSenders.h"
#include "protocol/AuthenticationSenders.h"
#include "protocol/ClientSenders.h"
#include "MockResponseHandler.h"
#include "MockConnectionHandler.h"
#include <tools/Path.h>
#include <tools/ElementObserver_ABC.h>
#include <tools/ServerNetworker.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/assign.hpp>

#define LAUNCHER_CHECK_MESSAGE( MSG, EXPECTED ) BOOST_CHECK_EQUAL( MSG.ShortDebugString(), EXPECTED )

namespace launcher_test
{
    const std::string host = "127.0.0.1";

    template< typename P >
    void FillProfileDescription( P* p )
    {
        p->set_login( "login" );
        p->set_password( "password" );
        p->set_supervisor( true );
    }
    template< typename P >
    void FillProfile( P* p )
    {
        FillProfileDescription( p );
        p->mutable_read_only_formations()->add_elem()->set_id( 3 );
        p->mutable_read_only_formations()->add_elem()->set_id( 5 );
        p->mutable_read_write_formations()->add_elem()->set_id( 6 );
        p->mutable_read_write_formations()->add_elem()->set_id( 7 );
        p->mutable_read_only_automates()->add_elem()->set_id( 8 );
        p->mutable_read_only_automates()->add_elem()->set_id( 9 );
        p->mutable_read_write_automates()->add_elem()->set_id( 10 );
        p->mutable_read_write_automates()->add_elem()->set_id( 11 );
        p->mutable_read_only_parties()->add_elem()->set_id( 12 );
        p->mutable_read_only_parties()->add_elem()->set_id( 13 );
        p->mutable_read_write_parties()->add_elem()->set_id( 14 );
        p->mutable_read_write_parties()->add_elem()->set_id( 15 );
        p->mutable_read_only_crowds()->add_elem()->set_id( 16 );
        p->mutable_read_only_crowds()->add_elem()->set_id( 17 );
        p->mutable_read_write_crowds()->add_elem()->set_id( 18 );
        p->mutable_read_write_crowds()->add_elem()->set_id( 19 );
    }

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

    MOCK_BASE_CLASS( MockDispatcher, tools::ServerNetworker )
    {
        MockDispatcher( unsigned short port )
            : tools::ServerNetworker( boost::lexical_cast< std::string >( port ) )
            , authenticated_( false )
        {
            AllowConnections();
            RegisterMessage< MockDispatcher, sword::ClientToSim >( *this, &MockDispatcher::Receive );
            RegisterMessage< MockDispatcher, sword::ClientToAuthentication >( *this, &MockDispatcher::AutoAuthentification );
        }
        MOCK_METHOD( Receive, 2, void( const std::string&, const sword::ClientToSim& ), ReceiveSim );
        MOCK_METHOD( Receive, 2, void( const std::string&, const sword::ClientToAuthentication& ), ReceiveAuth );
        MOCK_METHOD( ConnectionSucceeded, 2 );
        MOCK_METHOD( ConnectionFailed, 2 );
        MOCK_METHOD( ConnectionError, 2 );
        MOCK_METHOD( ConnectionWarning, 2 );

        template <typename M>
        void Send( M& msg )
        {
            ServerNetworker::Send( host, msg );
        }

        bool AuthenticationPerformed() const
        {
            return authenticated_;
        }
        void AutoAuthentification( const std::string& endpoint, const sword::ClientToAuthentication& msg )
        {
            if( msg.message().has_authentication_request() )
            {
                authentication::AuthenticationResponse response;
                *response().mutable_server_version() = msg.message().authentication_request().version();
                response().set_error_code( sword::AuthenticationResponse::success );
                response.Send( *this, msg.has_context() ? msg.context() : 1 );
                authenticated_ = true;
            }
            else
                Receive( endpoint, msg ); // forward to mock
        }
        std::string host;
        bool authenticated_;
    };

    struct ApplicationFixture
    {
        ApplicationFixture()
            : arg1( "--launcher-port=" + boost::lexical_cast< std::string >( PORT ) )
            , arg2( "--dispatcher-port=" + boost::lexical_cast< std::string >( PORT + 2 ) )
            , args( boost::assign::list_of< char* >( "" )
                                                   ( "--root-dir=../../data" )
                                                   ( "--exercises-dir=tests/launcher_test" )
                                                   ( "--test" )
                                                   ( const_cast< char* >( arg1.c_str() ) )
                                                   ( const_cast< char* >( arg2.c_str() ) ) )
            , argc( static_cast< int >( args.size() ) )
            , app ( argc, &args[0] )
        {}
        std::string arg1, arg2;
        std::vector< char* > args;
        int argc;
        QApplication app;
    };

    class ExerciseListener : private boost::noncopyable
                           , public tools::Observer_ABC
                           , public tools::ElementObserver_ABC< frontend::Exercise_ABC >
    {
    public:
        explicit ExerciseListener( kernel::Controllers& controllers )
            : controllers_( controllers )
        {
            controllers_.Register( *this );
        }
        virtual ~ExerciseListener()
        {
            controllers_.Unregister( *this );
        }
        virtual void NotifyCreated( const frontend::Exercise_ABC& exercise )
        {
            exercises_.push_back( &exercise );
        }
        bool Check() const
        {
            return ! exercises_.empty();
        }
        kernel::Controllers& controllers_;
        std::vector< const frontend::Exercise_ABC* > exercises_;
    };

    struct Fixture : ApplicationFixture
    {
        Fixture()
            : client    ( controllers.controller_ )
            , timeout   ( 5000 )
            , dispatcher( PORT + 2 )
        {
            launcher.Initialize( argc, &args[0] );
            BOOST_REQUIRE_MESSAGE( launcher.GetLastError().empty(), launcher.GetLastError() );
            MOCK_EXPECT( handler.OnConnectionSucceeded ).once();
            client.Connect( host, PORT, handler );
            while( !client.Connected() && !timeout.Expired() )
                Update();
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

    struct ExerciseFixture : Fixture
    {
        ExerciseFixture()
            : Fixture()
            , listener( controllers )
            , session ( "default" )
            , exercise( 0 )
            , handler ( new MockResponseHandler() )
        {
            BOOST_REQUIRE( client.Connected() );
            client.QueryExerciseList();
            timeout.Start();

            while( !listener.Check() && !timeout.Expired() )
                Update();
            BOOST_REQUIRE( listener.Check() );

            exercise = listener.exercises_.front();
            BOOST_REQUIRE( exercise );

            filePath = BOOST_RESOLVE( exercise->GetName() / "sessions" / session / "session.xml" );
            savePath = BOOST_RESOLVE( exercise->GetName() / "sessions" / "session.xml.save" );
            filePath.Copy( savePath, tools::Path::OverwriteIfExists );

            MOCK_EXPECT( dispatcher.ConnectionSucceeded ).once().with( mock::any, mock::retrieve( dispatcher.host ) );
            exercise->StartDispatcher( session,
                boost::assign::map_list_of
                    ( "session/config/simulation/network/@port", boost::lexical_cast< std::string >( PORT + 1 ) )
                    ( "session/config/dispatcher/network/@client", "localhost:" + boost::lexical_cast< std::string >( PORT + 1 ) )
                    ( "session/config/dispatcher/network/@server", boost::lexical_cast< std::string >( PORT + 2 ) )
                    ( "session/config/gaming/network/@server", "localhost:" + boost::lexical_cast< std::string >( PORT + 2 ) )
                );

            timeout.Start();
            while( ( !exercise->IsRunning() || !dispatcher.AuthenticationPerformed() ) && !timeout.Expired() )
                Update();
            BOOST_REQUIRE( exercise->IsRunning() );
            client.RegisterLauncherHandler( handler );
            mock::verify();
        }
        ~ExerciseFixture()
        {
           exercise->Stop( session );
           savePath.Copy( filePath, tools::Path::OverwriteIfExists );
           savePath.Remove();
        }

        void VerifySendRequest( const std::string& expected )
        {
            sword::ClientToSim message;
            MOCK_EXPECT( dispatcher.ReceiveSim ).once().with( mock::any, mock::retrieve( message ) );
            timeout.Start();
            while( !message.IsInitialized()&& !timeout.Expired() )
                Update();
            LAUNCHER_CHECK_MESSAGE( message, expected );
            mock::verify();
        }
        void VerifySendAuthRequest( const std::string& expected )
        {
            sword::ClientToAuthentication message;
            MOCK_EXPECT( dispatcher.ReceiveAuth ).once().with( mock::any, mock::retrieve( message ) );
            timeout.Start();
            while( !message.IsInitialized()&& !timeout.Expired() )
                Update();
            LAUNCHER_CHECK_MESSAGE( message, expected.c_str() );
            mock::verify();
        }
        template< typename T >
        void Wait( const T& message )
        {
            timeout.Start();
            while( !message.IsInitialized() && !timeout.Expired() )
                Update();
        }
        tools::Path filePath;
        tools::Path savePath;
        ExerciseListener listener;
        const frontend::Exercise_ABC* exercise;
        const tools::Path session;
        boost::shared_ptr< MockResponseHandler > handler;
    };
}
