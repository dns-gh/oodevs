// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "UserController.h"

#include "cpplog/cpplog.hpp"
#include "PropertyTree.h"
#include "runtime/Utf8.h"
#include "Sql_ABC.h"
#include "UuidFactory_ABC.h"

extern "C" {
#include <bcrypt/bcrypt.h>
}
#include <boost/algorithm/string/erase.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/uuid/uuid_io.hpp>

using namespace host;
using runtime::Utf8Convert;

namespace
{
std::string HashPassword( const std::string& password )
{
    char salt[BCRYPT_HASHSIZE], hash[BCRYPT_HASHSIZE];
    bcrypt_gensalt( 4, salt );
    bcrypt_hashpw( password.c_str(), salt, hash );
    return std::string( hash, sizeof hash );
}

bool ValidatePassword( const std::string& password, const std::string hash )
{
    char output[BCRYPT_HASHSIZE];
    if( hash.size() != sizeof output )
        return false;
    const int err = bcrypt_hashpw( password.c_str(), hash.c_str(), output );
    if( err )
        return false;
    return !memcmp( hash.c_str(), output, sizeof output );
}
}

// -----------------------------------------------------------------------------
// Name: UserController::UserController
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
UserController::UserController( cpplog::BaseLogger& log,
                                UuidFactory_ABC& uuids,
                                Sql_ABC& db )
    : log_  ( log )
    , uuids_( uuids )
    , db_   ( db )
{
    SetupDatabase();
}

// -----------------------------------------------------------------------------
// Name: UserController::UserController
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
UserController::~UserController()
{
    // NOTHING
}

namespace
{
void MakeTables( Sql_ABC& db )
{
    Sql_ABC::T_Transaction tr = db.Begin();
    db.Prepare( *tr, "CREATE TABLE IF NOT EXISTS revisions ("
        "  id       INTEGER PRIMARY KEY"
        ", revision INTEGER"
        ")" )->Next();
    db.Prepare( *tr, "CREATE TABLE IF NOT EXISTS users ("
        "  id       INTEGER PRIMARY KEY"
        ", username TEXT"
        ", name     TEXT"
        ", hash     TEXT"
        ", reset    BOOLEAN"
        ", language TEXT"
        ")" )->Next();
    db.Prepare( *tr, "CREATE TABLE IF NOT EXISTS tokens ("
        "  id       INTEGER PRIMARY KEY"
        ", user     INTEGER"
        ", token    TEXT"
        ", created  DATE"
        ")" )->Next();
    db.Commit( *tr );
}

void MakeDefaultDatabase( Sql_ABC& db )
{
    Sql_ABC::T_Transaction tr = db.Begin();
    Sql_ABC::T_Statement st = db.Prepare( *tr, "INSERT INTO revisions ( revision ) VALUES ( ? )" );
    st->Bind( 1 );
    st->Next();
    st = db.Prepare( *tr, "INSERT INTO users"
          "( username, name, hash, reset, language )"
          "VALUES ( ?, ?, ?, ?, ? )" );
    st->Bind( "admin" );
    st->Bind( "Admin" );
    st->Bind( HashPassword( "admin" ) );
    st->Bind( 1 );
    st->Bind( "eng" );
    st->Next();
    db.Commit( *tr );
}

int GetRevision( Sql_ABC& db )
{
    Sql_ABC::T_Transaction tr = db.Begin( false );
    Sql_ABC::T_Statement st = db.Prepare( *tr, "SELECT revision FROM revisions" );
    int rev = 0;
    while( st->Next() )
        st->Read( rev );
    return rev;
}

void MigrateDatabase( Sql_ABC& db )
{
    int rev = GetRevision( db );
    switch( rev )
    {
        case 0:
            MakeDefaultDatabase( db );
            break;
        case 1:
            break;
        // future revisions here
    }
}
}

// -----------------------------------------------------------------------------
// Name: UserController::MigrateDatabase
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
void UserController::SetupDatabase()
{
    MakeTables( db_ );
    MigrateDatabase( db_ );
}

namespace
{
template< typename T >
T Read( const Sql_ABC::T_Statement& st )
{
    T value;
    st->Read( value );
    return value;
}

Tree MakeToken( const std::string& token )
{
    Tree rpy;
    rpy.put( "token", token );
    return rpy;
}

// -----------------------------------------------------------------------------
// Name: CreateToken
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
Tree CreateToken( const UuidFactory_ABC& uuids, Sql_ABC& db, Sql_ABC::T_Transaction tr, int user )
{
    std::string token = boost::lexical_cast< std::string >( uuids.Create() );
    boost::algorithm::erase_all( token, "-" );
    Sql_ABC::T_Statement st = db.Prepare( *tr, "INSERT INTO tokens"
            "( user, token, created )"
            "VALUES ( ?, ?, DATE( 'now' ) )" );
    st->Bind( user );
    st->Bind( token );
    st->Next();
    db.Commit( *tr );
    return MakeToken( token );
}
}

// -----------------------------------------------------------------------------
// Name: UserController::Login
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
Tree UserController::Login( const std::string& username, const std::string& password )
{
    try
    {
        Sql_ABC::T_Transaction tr = db_.Begin();
        Sql_ABC::T_Statement st = db_.Prepare( *tr,
                "SELECT users.id, users.hash, tokens.token FROM users "
                "LEFT JOIN tokens ON users.id = tokens.user "
                "WHERE users.username = ?" );
        st->Bind( username );
        bool valid = st->Next();
        if( !valid )
            return Tree();
        const int id = Read< int >( st );
        const std::string hash = Read< std::string >( st );
        const std::string token = Read< std::string >( st );
        if( !ValidatePassword( password, hash ) )
            return Tree();
        if( !token.empty() )
            MakeToken( token );
        return CreateToken( uuids_, db_, tr, id );
    }
    catch( const std::exception& err )
    {
        LOG_ERROR( log_ ) << err.what();
        LOG_ERROR( log_ ) << "[sql] Unable to login";
    }
    return Tree();
}

bool UserController::IsAuthenticated( const std::string& token )
{
    try
    {
        Sql_ABC::T_Transaction tr = db_.Begin( false );
        Sql_ABC::T_Statement st = db_.Prepare( *tr,
                "SELECT COUNT( tokens.id ) FROM tokens "
                "JOIN users ON users.id = tokens.user "
                "WHERE tokens.token = ?" );
        st->Bind( token );
        st->Next();
        return Read< int >( st ) > 0;
    }
    catch( const std::exception& err )
    {
        LOG_ERROR( log_ ) << err.what();
        LOG_ERROR( log_ ) << "[sql] Unable to authenticate token";
    }
    return false;
}

void UserController::Logout( const std::string& token )
{
    try
    {
        Sql_ABC::T_Transaction tr = db_.Begin();
        Sql_ABC::T_Statement st = db_.Prepare( *tr, "DELETE FROM tokens WHERE token = ?" );
        st->Bind( token );
        st->Next();
        db_.Commit( *tr );
    }
    catch( const std::exception& err )
    {
        LOG_ERROR( log_ ) << err.what();
        LOG_ERROR( log_ ) << "[sql] Unable to logout";
    }
}
