// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef shield_test_Tools_h
#define shield_test_Tools_h

#include "shield/Client_ABC.h"
#include "shield/Server_ABC.h"
#include "shield/Converter.h"
#include "shield/proto/AarToClient.pb.h"
#include "shield/proto/AuthenticationToClient.pb.h"
#include "shield/proto/DispatcherToClient.pb.h"
#include "shield/proto/MessengerToClient.pb.h"
#include "shield/proto/ReplayToClient.pb.h"
#include "shield/proto/SimToClient.pb.h"
#include "shield/proto/LauncherToAdmin.pb.h"
#include "shield/proto/ClientToAar.pb.h"
#include "shield/proto/ClientToAuthentication.pb.h"
#include "shield/proto/ClientToMessenger.pb.h"
#include "shield/proto/ClientToReplay.pb.h"
#include "shield/proto/ClientToSim.pb.h"
#include "shield/proto/AdminToLauncher.pb.h"
#include <google/protobuf/descriptor.h>

namespace google
{
namespace protobuf
{
    inline mock::stream& operator<<( mock::stream& s, const Message& msg )
    {
        return s << msg.ShortDebugString();
    }
}
}

namespace shield
{
    MOCK_BASE_CLASS( MockClient, Client_ABC )
    {
        MOCK_METHOD_EXT( Send, 1, void( const MsgsAarToClient::MsgAarToClient& ), SendAarToClient )
        MOCK_METHOD_EXT( Send, 1, void( const MsgsAuthenticationToClient::MsgAuthenticationToClient& ), SendAuthenticationToClient )
        MOCK_METHOD_EXT( Send, 1, void( const MsgsDispatcherToClient::MsgDispatcherToClient& ), SendDispatcherToClient )
        MOCK_METHOD_EXT( Send, 1, void( const MsgsMessengerToClient::MsgMessengerToClient& ), SendMessengerToClient )
        MOCK_METHOD_EXT( Send, 1, void( const MsgsReplayToClient::MsgReplayToClient& ), SendReplayToClient )
        MOCK_METHOD_EXT( Send, 1, void( const MsgsSimToClient::MsgSimToClient& ), SendSimToClient )
        MOCK_METHOD_EXT( Send, 1, void( const MsgsLauncherToAdmin::MsgLauncherToAdmin& ), SendLauncherToAdmin )
    };
    MOCK_BASE_CLASS( MockServer, Server_ABC )
    {
        MOCK_METHOD_EXT( Send, 1, void( const sword::ClientToSim& ), SendClientToSim )
        MOCK_METHOD_EXT( Send, 1, void( const sword::ClientToAuthentication& ), SendClientToAuthentication )
        MOCK_METHOD_EXT( Send, 1, void( const sword::ClientToReplay& ), SendClientToReplay )
        MOCK_METHOD_EXT( Send, 1, void( const sword::ClientToAar& ), SendClientToAar )
        MOCK_METHOD_EXT( Send, 1, void( const sword::ClientToMessenger& ), SendClientToMessenger )
        MOCK_METHOD_EXT( Send, 1, void( const sword::AdminToLauncher& ), SendAdminToLauncher )
    };

    struct Fixture
    {
        Fixture()
            : converter( "client endpoint", server, client )
        {}
        MockServer server;
        MockClient client;
        Converter converter;
    };

    template< typename M >
    struct MessageFixture : Fixture
    {
        MessageFixture()
            : content( *msg.mutable_message() )
        {}
        M msg;
        typename M::Content& content;
    };
    template< typename M >
    struct ContextFixture : MessageFixture< M >
    {
        ContextFixture()
        {
            msg.set_context( 42 );
        }
    };

    struct constraint
    {
        template< typename Message >
        constraint( const Message& msg, const std::string& expected )
            : expected_( expected )
            , fields_  ( msg.descriptor()->field_count() )
        {
            BOOST_REQUIRE_MESSAGE( msg.IsInitialized(),
                "The message has not been fully initialized, probably some fields are missing : "
                    + msg.InitializationErrorString() );
        }

        template< typename Actual >
        friend bool operator==( const Actual& actual, const constraint& c )
        {
            if( ! actual.IsInitialized() )
                return false;
            BOOST_REQUIRE_EQUAL( c.fields_, actual.descriptor()->field_count() );
            const std::string s =  actual.ShortDebugString();
            //for( std::size_t pos = 0; pos != std::min( s.size(), c.expected_.size() ); ++pos )
            //    if( s[pos] != c.expected_[pos] )
            //    {
            //        BOOST_ERROR( "Mismatch starting at position " << pos << ": " << s[pos] << " != " << c.expected_[pos] );
            //        return false;
            //    }
            //BOOST_CHECK_MESSAGE( s.find( ": 0 " ) == std::string::npos, "one of the value fields is 0 : " + s );
            //BOOST_CHECK_MESSAGE( s.find( ": \"\" " ) == std::string::npos, "one of the string fields is empty !" + s );
            return s.size() == c.expected_.size();
        }

        friend std::ostream& operator<<( std::ostream& os, const constraint& c )
        {
            return os << c.expected_;
        }

        std::string expected_;
        int fields_;
    };

    template< typename C >
    void FillCoordLatLong( C* c )
    {
        c->set_latitude( 17.23 );
        c->set_longitude( 23.17 );
    }
    template< typename P >
    void FillProfileDescription( P* p )
    {
        p->set_login( "login" );
        p->set_password( "password" );
        p->set_supervisor( true );
        p->mutable_role()->set_id( 77 );
    }
    template< typename S >
    void FillShape( S* s )
    {
        s->set_category( "category" );
        s->set_color( "color" );
        s->set_pattern( "pattern" );
        FillCoordLatLong( s->mutable_points()->add_elem() );
        FillCoordLatLong( s->mutable_points()->add_elem() );
    }
    template< typename M >
    void FillMarker( M* m )
    {
        m->set_name( "name" );
        m->set_description( "description" );
        m->mutable_parent()->set_id( 23 );
        m->set_number( "number" );
    }
    template< typename P >
    void FillProperty( P* p )
    {
        p->set_name( "property" );
        p->mutable_value()->set_string_value( "1" );
        p->mutable_value()->set_integer_value( 2 );
        p->mutable_value()->set_unsigned_integer_value( 3u );
        p->mutable_value()->set_float_value( 4.f );
        p->mutable_value()->set_bool_value( true );
    }
    template< typename C >
    void FillClientObject( C* c )
    {
        c->set_name( "name" );
        FillProperty( c->add_properties() );
        FillProperty( c->add_properties() );
    }
    template< typename T >
    void FillTasker( T* t )
    {
        t->mutable_automat()->set_id( 7 );
        t->mutable_formation()->set_id( 8 );
        t->mutable_crowd()->set_id( 9 );
        t->mutable_unit()->set_id( 10 );
        t->mutable_party()->set_id( 11 );
    }
    template< typename A >
    void FillPartyAdhesion( A* a )
    {
        a->mutable_party()->set_id( 20 );
        a->set_value( 21.f );
    }
    template< typename E >
    void FillExercice( E* e )
    {
        e->set_name( "name" );
        e->set_running( true );
        e->set_port( 7u );
    }
    template< typename U >
    void FillUrbanUsage( U* u )
    {
        u->set_role( "office" );
        u->set_percentage( 11 );
    }
}

#endif // shield_test_Tools_h
