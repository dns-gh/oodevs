// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "web_control_plugin_test_pch.h"
#include "web_control_plugin/WebControl.h"
#include "protocol/SimulationSenders.h"
#include "protocol/ClientSenders.h"
#include "dispatcher/SimulationPublisher_ABC.h"

namespace
{
    MOCK_BASE_CLASS( MockPublisher, dispatcher::SimulationPublisher_ABC )
    {
        MOCK_METHOD_EXT( Send, 1, void( const sword::DispatcherToSim& ), DispatchToSim );
        MOCK_METHOD_EXT( Send, 1, void( const sword::ClientToSim& ),     ClientToSim );
    };

    struct Fixture
    {
        Fixture()
            : control_( publisher_ )
        {
            // NOTHING
        }
        ~Fixture()
        {
            // NOTHING
        }

        MockPublisher publisher_;
        plugins::web_control::WebControl control_;
    };

    template< bool (sword::ClientToSim_Content::*Func)() const >
    bool CheckClientToSim( const sword::ClientToSim& msg )
    {
        return ( msg.message().*Func )();
    }

    const boost::xpressive::sregex httpCodeRegex = boost::xpressive::sregex::compile( "^HTTP\\/1\\.1\\s+(\\d+)\\s+.+\r\n" );
    bool CheckHttpCode( int code, const std::string& data )
    {
        boost::xpressive::smatch what;
        bool match = boost::xpressive::regex_search( data, what, httpCodeRegex );
        return match && !what[1].compare( boost::lexical_cast< std::string >( code ) );
    }

    const boost::xpressive::sregex httpHeaderRegex = boost::xpressive::sregex::compile( "^.+\r\n\r\n" );
    std::string EraseHttpHeader( const std::string& data )
    {
        return boost::xpressive::regex_replace( data, httpHeaderRegex, "" );
    }

    void CheckNotify( plugins::web_control::WebControl& control, const std::string& method, const std::string& url, int code, const std::string& expected )
    {
        const std::string reply = control.Notify( method, url );
        BOOST_CHECK( CheckHttpCode( code, reply ) );
        BOOST_CHECK_EQUAL( expected, EraseHttpHeader( reply ));
    }
}

BOOST_FIXTURE_TEST_CASE( plugin_notify_get_returns_json_state, Fixture )
{
    CheckNotify( control_, "GET", "/get", 200, "{\"state\":\"stopped\"}" );
}

BOOST_FIXTURE_TEST_CASE( plugin_notify_pause_send_control_pause, Fixture )
{
    MOCK_EXPECT( publisher_.ClientToSim ).once().with( CheckClientToSim< &sword::ClientToSim_Content::has_control_pause > );
    control_.Notify( "GET", "/pause" );
}

BOOST_FIXTURE_TEST_CASE( plugin_notify_play_send_control_resume, Fixture )
{
    MOCK_EXPECT( publisher_.ClientToSim ).once().with( CheckClientToSim< &sword::ClientToSim_Content::has_control_resume > );
    control_.Notify( "GET", "/play" );
}

BOOST_FIXTURE_TEST_CASE( plugin_notify_stop_send_control_stop, Fixture )
{
    MOCK_EXPECT( publisher_.ClientToSim ).once().with( CheckClientToSim< &sword::ClientToSim_Content::has_control_stop > );
    control_.Notify( "GET", "/stop" );
}

BOOST_FIXTURE_TEST_CASE( plugin_invalid_methods_return_bad_request, Fixture )
{
    const std::string expected = "Invalid method type";
    CheckNotify( control_, "POST",   "/stop", 400, expected );
    CheckNotify( control_, "PUT",    "/stop", 400, expected );
    CheckNotify( control_, "DELETE", "/stop", 400, expected );
    CheckNotify( control_, "RANDOM", "/stop", 400, expected );
}

BOOST_FIXTURE_TEST_CASE( plugin_invalid_urls_returns_not_found, Fixture )
{
    const std::string expected = "Unknown URI";
    CheckNotify( control_, "GET", "/", 404, expected );
    CheckNotify( control_, "GET", "/get2", 404, expected );
    CheckNotify( control_, "GET", "/a/b/c/d/", 404, expected );
    CheckNotify( control_, "GET", "/get/", 404, expected );
    CheckNotify( control_, "GET", "/get?token=F4B2D", 404, expected );
}

namespace
{
    template< typename T >
    void CheckControlAck( plugins::web_control::WebControl& control, const T& getPointer, const std::string& state )
    {
        sword::SimToClient msg;
        (msg.mutable_message()->*getPointer)()->set_error_code( sword::ControlAck_ErrorCode_no_error );
        control.Receive( msg );
        CheckNotify( control, "GET", "/get", 200, "{\"state\":\"" + state + "\"}" );
    }
}

BOOST_FIXTURE_TEST_CASE( plugin_receive_message_update_state, Fixture )
{
    sword::SimToClient msg;
    msg.mutable_message()->mutable_control_information()->set_status( sword::running );
    control_.Receive( msg );

    CheckNotify( control_, "GET", "/get", 200, "{\"state\":\"running\"}" );

    CheckControlAck( control_, &sword::SimToClient_Content::mutable_control_pause_ack,  "paused" );
    CheckControlAck( control_, &sword::SimToClient_Content::mutable_control_resume_ack, "running" );
    CheckControlAck( control_, &sword::SimToClient_Content::mutable_control_stop_ack,   "stopped" );
}