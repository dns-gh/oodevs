// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifdef _MSC_VER
#   pragma warning( disable : 4244 )
#endif

#include "host_test.h"

#include "runtime/FileSystem.h"
#include "runtime/PropertyTree.h"
#include "host/Crypt.h"
#include "host/Sql.h"
#include "host/UserController.h"
#include "host/UuidFactory.h"
#include "web/HttpException.h"

#include <boost/date_time.hpp>
#include <boost/foreach.hpp>
#include <boost/uuid/nil_generator.hpp>
#include <cpplog/cpplog.hpp>

using namespace host;
using namespace property_tree;
using runtime::FileSystem;

namespace
{
struct SubFixture
{
    SubFixture()
        : log( std::cout )
        , fs ( log )
        , db ( BOOST_RESOLVE( "user_controller_test.db" ) )
    {
        fs.Remove( db );
    }

    cpplog::OstreamLogger log;
    const FileSystem fs;
    const Path db;
    const Crypt crypt;
    UuidFactory uuids;
};

struct Fixture
{
    Fixture()
        : sql  ( sub.db )
        , users( sub.log, sub.crypt, sub.uuids, sql )
    {
        // NOTHING
    }

    SubFixture sub;
    Sql sql;
    UserController users;

    Tree LogAdmin()
    {
        return users.Login( "admin@masagroup.net", "admin" );
    }
};

void CheckUser( const Tree& user, const std::string& username, const std::string& name, const std::string& type, bool temporary, const std::string& language )
{
    BOOST_CHECK_EQUAL( Get< std::string >( user, "username" ), username );
    BOOST_CHECK_EQUAL( Get< std::string >( user, "name" ), name );
    BOOST_CHECK_EQUAL( Get< std::string >( user, "type" ), type );
    BOOST_CHECK_EQUAL( Get< std::string >( user, "temporary" ), temporary ? "true" : "false" );
    BOOST_CHECK_EQUAL( Get< std::string >( user, "language" ), language );
}

void CheckAdmin( const Tree& user )
{
    CheckUser( user, "admin@masagroup.net", "Default", "administrator", false, "eng" );
}
}

BOOST_FIXTURE_TEST_CASE( user_controller_defaults, Fixture )
{
    BOOST_CHECK_EQUAL( users.CountUsers( boost::uuids::nil_uuid() ), size_t( 1 ) );
    std::vector< Tree > items = users.ListUsers( boost::uuids::nil_uuid(), 0, INT_MAX );
    BOOST_CHECK_EQUAL( items.size(), size_t( 1 ) );
    items.push_back( LogAdmin() );
    BOOST_FOREACH( const Tree& src, items )
        CheckAdmin( src );
}

BOOST_FIXTURE_TEST_CASE( invalid_user_cannot_log, Fixture )
{
    BOOST_CHECK_THROW( users.Login( "random_junk", "blah" ), web::HttpException );
    BOOST_CHECK_THROW( users.Login( "admin@masagroup.net", "AdMiN" ), web::HttpException );
}

BOOST_FIXTURE_TEST_CASE( user_can_log_multiple_times, Fixture )
{
    std::vector< Tree > items;
    items.push_back( LogAdmin() );
    items.push_back( LogAdmin() );
    BOOST_FOREACH( const Tree& src, items )
        CheckAdmin( src );

    const std::string sid_a = Get< std::string >( items[0], "sid" );
    const std::string sid_b = Get< std::string >( items[1], "sid" );
    BOOST_CHECK( !sid_a.empty() );
    BOOST_CHECK( !sid_b.empty() );
    BOOST_CHECK( sid_a != sid_b );

    items.clear();
    items.push_back( users.IsAuthenticated( sid_a ) );
    items.push_back( users.IsAuthenticated( sid_b ) );
    BOOST_FOREACH( const Tree& src, items )
        CheckAdmin( src );

    users.Logout( sid_b );
    BOOST_CHECK_THROW( users.IsAuthenticated( sid_b ), web::HttpException );
    CheckAdmin( users.IsAuthenticated( sid_a ) );
}

BOOST_FIXTURE_TEST_CASE( invalid_token_cannot_authentify, Fixture )
{
    BOOST_CHECK_THROW( users.IsAuthenticated( "ababeza)erjz" ), web::HttpException );
    const Tree src = LogAdmin();
    const std::string sid = Get< std::string >( src, "sid" );
    BOOST_CHECK_THROW( users.IsAuthenticated( sid + "_" ), web::HttpException );
}

BOOST_FIXTURE_TEST_CASE( user_tokens_have_expiration_date, Fixture )
{
    const Tree tree = LogAdmin();
    const std::string sid = Get< std::string >( tree, "sid" );
    CheckAdmin( tree );
    CheckAdmin( users.IsAuthenticated( sid ) );
    const boost::gregorian::date now = boost::gregorian::day_clock::local_day();
    const int limit = users.GetExpirationLimitDays();
    for( int i = -1; i <= limit + 1; i++ )
    {
        const std::string current = boost::gregorian::to_sql_string( now + boost::gregorian::days( i ) );
        if( i >= 0 && i < limit )
            CheckAdmin( users.IsAuthenticated( sid, current ) );
        else
            BOOST_CHECK_THROW( users.IsAuthenticated( sid, current ), web::HttpException );
    }
}

BOOST_FIXTURE_TEST_CASE( users_can_be_created_updated_deleted, Fixture )
{
    std::vector< Tree > items;
    items.push_back( users.CreateUser( boost::uuids::nil_uuid(), "zorg", "Zorg", "zorg_pwd", web::USER_TYPE_ADMINISTRATOR, false ) );
    BOOST_CHECK_EQUAL( users.CountUsers( boost::uuids::nil_uuid() ), size_t( 2 ) );
    items.push_back( users.GetUser( boost::uuids::nil_uuid(), Get< int >( items[0], "id" ) ) );
    BOOST_FOREACH( const Tree& src, items )
        CheckUser( src, "zorg", "Zorg", "administrator", false, "eng" );

    Tree src = users.Login( "zorg", "zorg_pwd" );
    BOOST_CHECK( !src.empty() );
    int id = Get< int >( src, "id" );
    std::string sid = Get< std::string >( src, "sid" );
    BOOST_CHECK_THROW( users.UpdateUser( boost::uuids::nil_uuid(), sid, id, "zorg", "John Doe", false, std::string( "new_pwd" ) ), web::HttpException );
    src = users.UpdateUser( boost::uuids::nil_uuid(), sid, id, "zorg", "John Doe", false, boost::none );
    CheckUser( src, "zorg", "John Doe", "administrator", false, "eng" );

    src = users.UpdateLogin( "zorg", "zorg_pwd", "new_pwd" );
    BOOST_CHECK( !src.empty() );
    BOOST_CHECK_THROW( users.IsAuthenticated( sid ), web::HttpException );
    src = users.Login( "zorg", "new_pwd" );
    BOOST_CHECK( !src.empty() );

    const std::string admin = Get< std::string >( LogAdmin(), "sid" );
    sid = Get< std::string >( src, "sid" );
    id = Get< int >( src, "id" );
    BOOST_CHECK_THROW( users.DeleteUser( boost::uuids::nil_uuid(), sid, id ), web::HttpException );
    src = users.DeleteUser( boost::uuids::nil_uuid(), admin, id );
    CheckUser( src, "zorg", "John Doe", "administrator", false, "eng" );
    BOOST_CHECK_EQUAL( users.CountUsers( boost::uuids::nil_uuid() ), size_t( 1 ) );
    BOOST_CHECK_THROW( users.IsAuthenticated( sid ), web::HttpException );
}