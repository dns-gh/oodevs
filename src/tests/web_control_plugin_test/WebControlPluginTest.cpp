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

#include <boost/assign/list_of.hpp>
#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>

#ifdef _MSC_VER
#   pragma warning( push )
#   pragma warning( disable : 4100 4512 )
#endif
#include <boost/property_tree/json_parser.hpp>
#ifdef _MSC_VER
#pragma warning( pop )
#endif

typedef boost::property_tree::ptree Tree;

namespace
{
    MOCK_BASE_CLASS( MockPublisher, dispatcher::SimulationPublisher_ABC )
    {
        MOCK_METHOD( Send, 1, void( const sword::DispatcherToSim& ), DispatchToSim );
        MOCK_METHOD( Send, 1, void( const sword::ClientToSim& ),     ClientToSim );
    };

    const Tree FromJson( const std::string& data )
    {
        if( data.empty() )
            return Tree();
        Tree rpy;
        try
        {
            std::istringstream input( data );
            boost::property_tree::read_json( input, rpy );
        }
        catch( ... )
        {
            // NOTHING
        }
        return rpy;
    }

    typedef std::pair< std::string, std::string > T_Pair;;
    typedef std::vector< T_Pair > T_Pairs;

    void CheckJson( const std::string& data, const T_Pairs& expected )
    {
        const Tree tree = FromJson( data );
        BOOST_FOREACH( const T_Pairs::value_type& value, expected )
        {
            const boost::optional< std::string > opt = tree.get_optional< std::string >( value.first );
            const std::string next = opt == boost::none ? std::string() : *opt;
            BOOST_CHECK_EQUAL( value.second, next );
        }
    }

    void CheckJsonList( const std::string& data, const std::string& name, const std::vector< std::string >& list )
    {
        const Tree root = FromJson( data );
        BOOST_CHECK_EQUAL( root.count( name ), size_t( 1 ) );
        if( list.empty() )
            return;
        Tree::const_assoc_iterator sub = root.find( name );
        BOOST_REQUIRE( sub != root.not_found() );
        size_t count = 0;
        for( Tree::const_iterator it = sub->second.begin(); it != sub->second.end(); ++it, ++count )
            BOOST_CHECK( std::find( list.begin(), list.end(), it->second.data() ) != list.end() );
        BOOST_CHECK_EQUAL( list.size(), count );
    }

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

    void CheckNotifyJson( plugins::web_control::WebControl& control, const std::string& method, const std::string& url, int code, const T_Pairs& pairs )
    {
        const std::string reply = control.Notify( method, url );
        BOOST_CHECK( CheckHttpCode( code, reply ) );
        CheckJson( EraseHttpHeader( reply ), pairs );
    }

    void CheckNotify( plugins::web_control::WebControl& control, const std::string& method, const std::string& url, int code, const std::string& expected )
    {
        const std::string reply = control.Notify( method, url );
        BOOST_CHECK( CheckHttpCode( code, reply ) );
        BOOST_CHECK_EQUAL( expected, EraseHttpHeader( reply ) );
    }

    void CheckNotifyJsonList( plugins::web_control::WebControl& control, const std::string& method, const std::string& url, int code, const std::string& name, const std::vector< std::string >& list )
    {
        const std::string reply = control.Notify( method, url );
        BOOST_CHECK( CheckHttpCode( code, reply ) );
        CheckJsonList( EraseHttpHeader( reply ), name, list );
    }
}

BOOST_FIXTURE_TEST_CASE( plugin_notify_get_returns_json_state, Fixture )
{
    CheckNotifyJson( control_, "GET", "/get", 200, boost::assign::list_of< T_Pair >( std::make_pair( "state", "stopped" ) ) );
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
    CheckNotifyJson( control, "GET", "/get", 200, boost::assign::list_of< T_Pair >( std::make_pair( "state", state ) ) );
}
}

BOOST_FIXTURE_TEST_CASE( plugin_receive_message_update_state, Fixture )
{
    sword::SimToClient msg;
    msg.mutable_message()->mutable_control_information()->set_status( sword::running );
    control_.Receive( msg );

    CheckNotifyJson( control_, "GET", "/get", 200, boost::assign::list_of< T_Pair >( std::make_pair( "state", "running" ) ) );

    CheckControlAck( control_, &sword::SimToClient_Content::mutable_control_pause_ack,  "paused" );
    CheckControlAck( control_, &sword::SimToClient_Content::mutable_control_resume_ack, "running" );
    CheckControlAck( control_, &sword::SimToClient_Content::mutable_control_stop_ack,   "stopped" );
}

BOOST_FIXTURE_TEST_CASE( plugin_receive_date_time_updates, Fixture )
{
    sword::SimToClient msg;
    msg.mutable_message()->mutable_control_information()->mutable_initial_date_time()->set_data( "uno" );
    msg.mutable_message()->mutable_control_information()->mutable_date_time()->set_data( "duo" );
    control_.Receive( msg );
    CheckNotifyJson( control_, "GET", "/get", 200, boost::assign::list_of< T_Pair >
        ( std::make_pair( "start_time", "uno" ) )
        ( std::make_pair( "current_time", "duo" ) ) );
}

BOOST_FIXTURE_TEST_CASE( plugin_list_clients, Fixture )
{
    CheckNotifyJsonList( control_, "GET", "/get", 200, "clients", std::vector< std::string >() );
    control_.NotifyClientAuthenticated( "127.0.0.1" );
    CheckNotifyJsonList( control_, "GET", "/get", 200, "clients", boost::assign::list_of< std::string >
        ( "127.0.0.1" ) );
    control_.NotifyClientAuthenticated( "192.168.1.13" );
    CheckNotifyJsonList( control_, "GET", "/get", 200, "clients", boost::assign::list_of< std::string >
        ( "127.0.0.1" )( "192.168.1.13" ) );
    control_.NotifyClientLeft( "127.0.0.1" );
    CheckNotifyJsonList( control_, "GET", "/get", 200, "clients", boost::assign::list_of< std::string >
        ( "192.168.1.13" ) );
    control_.NotifyClientLeft( "192.168.1.13" );
    CheckNotifyJsonList( control_, "GET", "/get", 200, "clients", std::vector< std::string >() );
}