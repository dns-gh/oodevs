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
enum UserType
{
    USER_TYPE_ADMINISTRATOR,
    USER_TYPE_MANAGER,
    USER_TYPE_USER,
    USER_TYPE_COUNT,
};

// -----------------------------------------------------------------------------
// Name: Convert
// Created: BAX 2012-07-05
// -----------------------------------------------------------------------------
const char* Convert( UserType type )
{
    switch( type )
    {
        case USER_TYPE_ADMINISTRATOR:   return "administrator";
        case USER_TYPE_MANAGER:         return "manager";
    }
    return "user";
}

// -----------------------------------------------------------------------------
// Name: Convert
// Created: BAX 2012-07-05
// -----------------------------------------------------------------------------
UserType Convert( const std::string& type )
{
    if( type == "administrator" )   return USER_TYPE_ADMINISTRATOR;
    if( type == "manager" )         return USER_TYPE_MANAGER;
    return USER_TYPE_USER;
}

// -----------------------------------------------------------------------------
// Name: HashPassword
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
std::string HashPassword( const std::string& password )
{
    char salt[BCRYPT_HASHSIZE], hash[BCRYPT_HASHSIZE];
    bcrypt_gensalt( 4, salt );
    bcrypt_hashpw( password.c_str(), salt, hash );
    return std::string( hash, sizeof hash );
}

// -----------------------------------------------------------------------------
// Name: ValidatePassword
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
bool ValidatePassword( const std::string& password, const std::string& hash )
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
// -----------------------------------------------------------------------------
// Name: Execute
// Created: BAX 2012-07-05
// -----------------------------------------------------------------------------
size_t Execute( Statement_ABC& st )
{
    size_t count = 0;
    while( st.Next() )
        count++;
    return count;
}

// -----------------------------------------------------------------------------
// Name: MakeTables
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
void MakeTables( Sql_ABC& db )
{
    Sql_ABC::T_Transaction tr = db.Begin();
    Execute( *db.Prepare( *tr, "CREATE TABLE IF NOT EXISTS revisions ("
        "  id       INTEGER PRIMARY KEY"
        ", revision INTEGER"
        ", created  DATE"
        ")" ) );
    Execute( *db.Prepare( *tr, "CREATE TABLE IF NOT EXISTS users ("
        "  id           INTEGER PRIMARY KEY"
        ", username     TEXT"
        ", hash         TEXT"
        ", name         TEXT"
        ", type         TEXT"
        ", temporary    BOOLEAN"
        ", language     TEXT"
        ", created      DATE"
        ")" ) );
    Execute( *db.Prepare( *tr, "CREATE TABLE IF NOT EXISTS tokens ("
        "  id       INTEGER PRIMARY KEY"
        ", id_users INTEGER"
        ", source   TEXT"
        ", token    TEXT"
        ", created  DATE"
        ")" ) );
    db.Commit( *tr );
}

// -----------------------------------------------------------------------------
// Name: MakeDefaultDatabase
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
void MakeDefaultDatabase( Sql_ABC& db )
{
    Sql_ABC::T_Transaction tr = db.Begin();
    Sql_ABC::T_Statement st = db.Prepare( *tr,
            "INSERT INTO revisions"
            "          ( revision, created )"
            "VALUES    ( ?, DATE('NOW') )" );
    st->Bind( 1 );
    Execute( *st );
    st = db.Prepare( *tr,
        "INSERT INTO users"
        "          ( username, hash, name, type, temporary, language, created )"
        "VALUES    ( ?, ?, ?, ?, ?, ?, DATE('now') )" );
    st->Bind( "admin@masagroup.net" );
    st->Bind( HashPassword( "admin" ) );
    st->Bind( "Default" );
    st->Bind( Convert( USER_TYPE_ADMINISTRATOR ) );
    st->Bind( false );
    st->Bind( "eng" );
    Execute( *st );
    db.Commit( *tr );
}

// -----------------------------------------------------------------------------
// Name: GetRevision
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
int GetRevision( Sql_ABC& db )
{
    Sql_ABC::T_Transaction tr = db.Begin( false );
    Sql_ABC::T_Statement st = db.Prepare( *tr, "SELECT revision FROM revisions" );
    int rev = 0;
    while( st->Next() )
        rev = st->ReadInt();
    return rev;
}

// -----------------------------------------------------------------------------
// Name: MigrateDatabase
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
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
// Name: UserController::SetupDatabase
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
void UserController::SetupDatabase()
{
    MakeTables( db_ );
    MigrateDatabase( db_ );
}

namespace
{
// -----------------------------------------------------------------------------
// Name: MakeUser
// Created: BAX 2012-07-04
// -----------------------------------------------------------------------------
std::string MakeUser( const std::string& sid, const std::string& username, const std::string& name,
                      const std::string& type, bool temporary, const std::string& lang )
{
    Tree rpy;
    rpy.put( "sid", sid );
    rpy.put( "username", username );
    rpy.put( "name", name );
    rpy.put( "type", type );
    rpy.put( "temporary", temporary );
    rpy.put( "language", lang );
    return ToJson( rpy );
}

// -----------------------------------------------------------------------------
// Name: CreateToken
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
std::string CreateToken( const UuidFactory_ABC& uuids, Sql_ABC& db, Transaction& tr, int user, const std::string& source )
{
    std::string token = boost::lexical_cast< std::string >( uuids.Create() );
    boost::algorithm::erase_all( token, "-" );
    Sql_ABC::T_Statement st = db.Prepare( tr,
        "INSERT INTO tokens"
        "          ( id_users, source, token, created ) "
        "VALUES    ( ?, ?, ?, DATE('now') )" );
    st->Bind( user );
    st->Bind( source );
    st->Bind( token );
    Execute( *st );
    return token;
}

// -----------------------------------------------------------------------------
// Name: DeleteTokenWithUserId
// Created: BAX 2012-07-05
// -----------------------------------------------------------------------------
void DeleteTokenWithUserId( Sql_ABC& db, Transaction& tr, int id )
{
    Sql_ABC::T_Statement st = db.Prepare( tr, "DELETE FROM tokens WHERE id_users = ?" );
    st->Bind( id );
    Execute( *st );
}

// -----------------------------------------------------------------------------
// Name: DeleteTokenWithToken
// Created: BAX 2012-07-04
// -----------------------------------------------------------------------------
void DeleteTokenWithToken( Sql_ABC& db, Transaction& tr, const std::string& token )
{
    Sql_ABC::T_Statement st = db.Prepare( tr, "SELECT id_users FROM tokens WHERE token = ?" );
    st->Bind( token );
    while( st->Next() )
        DeleteTokenWithUserId( db, tr, st->ReadInt() );
}

bool FetchUser( Sql_ABC& db, Transaction& tr, const std::string& username, const std::string& source,
                int& id, std::string& hash, std::string& name, std::string& type, bool& temporary,
                std::string& lang, std::string& token )
{
    token.clear();
    Sql_ABC::T_Statement st = db.Prepare( tr,
        "SELECT     users.id, users.hash, users.name, users.type,"
        "           users.temporary, users.language, tokens.token "
        "FROM       users "
        "LEFT JOIN  tokens "
        "ON         users.id = tokens.id_users "
        "AND        tokens.source  = ? "
        "WHERE      users.username = ? "
        );
    st->Bind( source );
    st->Bind( username );
    bool valid = st->Next();
    if( !valid )
        return false;
    id = st->ReadInt();
    hash = st->ReadText();
    name = st->ReadText();
    type = st->ReadText();
    temporary = st->ReadBool();
    lang = st->ReadText();
    if( st->IsColumnDefined() )
        token = st->ReadText();
    return true;
}
}

// -----------------------------------------------------------------------------
// Name: UserController::Login
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
std::string UserController::Login( const std::string& username, const std::string& password, const std::string& source )
{
    try
    {
        Sql_ABC::T_Transaction tr = db_.Begin();
        int id; std::string hash, name, type, lang, token; bool temporary;
        bool valid = FetchUser( db_, *tr, username, source, id, hash, name, type, temporary, lang, token );
        if( !valid )
            return std::string();
        if( !ValidatePassword( password, hash ) )
            return std::string();
        if( !token.empty() )
            DeleteTokenWithToken( db_, *tr, token );
        const std::string sid = CreateToken( uuids_, db_, *tr, id, source );
        db_.Commit( *tr );
        return MakeUser( sid, username, name, type, temporary, lang );
    }
    catch( const std::exception& err )
    {
        LOG_ERROR( log_ ) << err.what();
        LOG_ERROR( log_ ) << "[sql] Unable to login";
    }
    return std::string();
}

// -----------------------------------------------------------------------------
// Name: UserController::IsAuthenticated
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
std::string UserController::IsAuthenticated( const std::string& token, const std::string& source )
{
    try
    {
        Sql_ABC::T_Transaction tr = db_.Begin( false );
        Sql_ABC::T_Statement st = db_.Prepare( *tr,
            "SELECT users.username, users.name, users.type, users.temporary, users.language "
            "FROM   tokens "
            "JOIN   users "
            "ON     users.id = tokens.id_users "
            "WHERE  tokens.source = ? "
            "AND    tokens.token  = ? "
            "LIMIT  1 "
            );
        st->Bind( source );
        st->Bind( token );
        bool valid = st->Next();
        if( !valid )
            return std::string();
        const std::string username = st->ReadText();
        const std::string name = st->ReadText();
        const std::string type = st->ReadText();
        const bool temporary = st->ReadBool();
        const std::string lang = st->ReadText();
        return MakeUser( token, username, name, type, temporary, lang );
    }
    catch( const std::exception& err )
    {
        LOG_ERROR( log_ ) << err.what();
        LOG_ERROR( log_ ) << "[sql] Unable to authenticate token";
    }
    return std::string();
}

// -----------------------------------------------------------------------------
// Name: UserController::Logout
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
void UserController::Logout( const std::string& token )
{
    try
    {
        Sql_ABC::T_Transaction tr = db_.Begin();
        DeleteTokenWithToken( db_, *tr, token );
        db_.Commit( *tr );
    }
    catch( const std::exception& err )
    {
        LOG_ERROR( log_ ) << err.what();
        LOG_ERROR( log_ ) << "[sql] Unable to logout";
    }
}

// -----------------------------------------------------------------------------
// Name: UserController::UpdateLogin
// Created: BAX 2012-07-05
// -----------------------------------------------------------------------------
std::string UserController::UpdateLogin( const std::string& username, const std::string& current, const std::string& password, const std::string& source )
{
    try
    {
        Sql_ABC::T_Transaction tr = db_.Begin();
        int id; std::string hash, name, type, lang, token; bool temporary;
        bool valid = FetchUser( db_, *tr, username, source, id, hash, name, type, temporary, lang, token );
        if( !valid )
            return std::string();
        if( !ValidatePassword( current, hash ) )
            return std::string();
        DeleteTokenWithUserId( db_, *tr, id );
        Sql_ABC::T_Statement st = db_.Prepare( *tr,
            "UPDATE users "
            "SET    hash = ?, temporary = ?"
            "WHERE  id = ?" );
        st->Bind( HashPassword( password ) );
        st->Bind( false );
        st->Bind( id );
        Execute( *st );
        const std::string sid = CreateToken( uuids_, db_, *tr, id, source );
        db_.Commit( *tr );
        return MakeUser( sid, username, name, type, false, lang );
    }
    catch( const std::exception& err )
    {
        LOG_ERROR( log_ ) << err.what();
        LOG_ERROR( log_ ) << "[sql] Unable to update login";
    }
    return std::string();
}