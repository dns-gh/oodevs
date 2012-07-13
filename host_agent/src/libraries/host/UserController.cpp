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
#include "Crypt_ABC.h"
#include "runtime/Utf8.h"
#include "Sql_ABC.h"
#include "UuidFactory_ABC.h"
#include "web/Reply.h"

#include <boost/algorithm/string/erase.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/uuid/uuid_io.hpp>

using namespace host;
using web::Reply;
typedef boost::property_tree::ptree Tree;

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
}

// -----------------------------------------------------------------------------
// Name: UserController::UserController
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
UserController::UserController( cpplog::BaseLogger& log,
                                const Crypt_ABC& crypt,
                                UuidFactory_ABC& uuids,
                                Sql_ABC& db )
    : log_  ( log )
    , crypt_( crypt )
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
        ", username     TEXT UNIQUE NOT NULL"
        ", hash         TEXT NOT NULL"
        ", name         TEXT"
        ", type         TEXT NOT NULL"
        ", temporary    BOOLEAN"
        ", language     TEXT"
        ", created      DATE"
        ")" ) );
    Execute( *db.Prepare( *tr, "CREATE TABLE IF NOT EXISTS tokens ("
        "  id       INTEGER PRIMARY KEY"
        ", id_users INTEGER"
        ", source   TEXT NOT NULL"
        ", token    TEXT UNIQUE NOT NULL"
        ", created  DATE"
        ")" ) );
    db.Commit( *tr );
}

// -----------------------------------------------------------------------------
// Name: UserController::CreateUser
// Created: BAX 2012-07-11
// -----------------------------------------------------------------------------
void CreateUser( Sql_ABC& db, Transaction& tr, const Crypt_ABC& crypt,
                 const std::string& username, const std::string& name,
                 const std::string& password, UserType type,
                 bool temporary, const std::string& lang )
{
    Sql_ABC::T_Statement st = db.Prepare( tr,
            "INSERT into users"
            "          ( username, hash, name, type, temporary, language, created )"
            "VALUES    ( ?, ?, ?, ?, ?, ?, DATE('now') )" );
    st->Bind( username );
    st->Bind( crypt.Hash( password ) );
    st->Bind( name );
    st->Bind( Convert( type ) );
    st->Bind( temporary );
    st->Bind( lang );
    Execute( *st );
}

// -----------------------------------------------------------------------------
// Name: MakeDefaultDatabase
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
void MakeDefaultDatabase( const Crypt_ABC& crypt, Sql_ABC& db )
{
    Sql_ABC::T_Transaction tr = db.Begin();
    Sql_ABC::T_Statement st = db.Prepare( *tr,
            "INSERT INTO revisions"
            "          ( revision, created )"
            "VALUES    ( ?, DATE('NOW') )" );
    st->Bind( 1 );
    Execute( *st );
    st.reset();
    CreateUser( db, *tr, crypt, "admin@masagroup.net", "Default", "admin",
                USER_TYPE_ADMINISTRATOR, false, "eng" );
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
void MigrateDatabase( const Crypt_ABC& crypt, Sql_ABC& db )
{
    int rev = GetRevision( db );
    switch( rev )
    {
        case 0:
            MakeDefaultDatabase( crypt, db );
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
    MigrateDatabase( crypt_, db_ );
}

namespace
{
// -----------------------------------------------------------------------------
// Name: PutUser
// Created: BAX 2012-07-11
// -----------------------------------------------------------------------------
void PutUser( Tree& dst, Statement_ABC& st )
{
    dst.put( "id", st.ReadInt() );
    dst.put( "username", st.ReadText() );
    dst.put( "name", st.ReadText() );
    dst.put( "type", st.ReadText() );
    dst.put( "temporary", st.ReadBool() );
    dst.put( "language", st.ReadText() );
}

// -----------------------------------------------------------------------------
// Name: PutUser
// Created: BAX 2012-07-11
// -----------------------------------------------------------------------------
void PutUser( Tree& dst, int id, const std::string& username, const std::string& name,
              const std::string& type, bool temporary, const std::string& lang )
{
    dst.put( "id", id );
    dst.put( "username", username );
    dst.put( "name", name );
    dst.put( "type", type );
    dst.put( "temporary", temporary );
    dst.put( "language", lang );
}

// -----------------------------------------------------------------------------
// Name: MakeToken
// Created: BAX 2012-07-04
// -----------------------------------------------------------------------------
Tree MakeToken( const std::string& sid, Statement_ABC& st )
{
    Tree rpy;
    rpy.put( "sid", sid );
    PutUser( rpy, st );
    return rpy;
}

// -----------------------------------------------------------------------------
// Name: MakeToken
// Created: BAX 2012-07-04
// -----------------------------------------------------------------------------
Tree MakeToken( const std::string& sid, int id, const std::string& username,
                const std::string& name, const std::string& type,
                bool temporary, const std::string& lang )
{
    Tree rpy;
    rpy.put( "sid", sid );
    PutUser( rpy, id, username, name, type, temporary, lang );
    return rpy;
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
Reply UserController::Login( const std::string& username, const std::string& password, const std::string& source )
{
    try
    {
        Sql_ABC::T_Transaction tr = db_.Begin();
        int id; std::string hash, name, type, lang, token; bool temporary;
        bool valid = FetchUser( db_, *tr, username, source, id, hash, name, type, temporary, lang, token );
        if( !valid )
            return Reply( web::NOT_FOUND );

        if( !crypt_.Validate( password, hash ) )
            return Reply( web::UNAUTHORIZED );

        if( !token.empty() )
            DeleteTokenWithToken( db_, *tr, token );
        const std::string sid = CreateToken( uuids_, db_, *tr, id, source );
        db_.Commit( *tr );
        return Reply( MakeToken( sid, id, username, name, type, temporary, lang ) );
    }
    catch( const std::exception& err )
    {
        LOG_ERROR( log_ ) << err.what();
        LOG_ERROR( log_ ) << "[sql] Unable to login";
    }
    return Reply( web::INTERNAL_SERVER_ERROR );
}

// -----------------------------------------------------------------------------
// Name: UserController::IsAuthenticated
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
Reply UserController::IsAuthenticated( const std::string& token, const std::string& source )
{
    try
    {
        Sql_ABC::T_Transaction tr = db_.Begin( false );
        Sql_ABC::T_Statement st = db_.Prepare( *tr,
            "SELECT users.id, users.username, users.name, users.type,"
            "       users.temporary, users.language "
            "FROM   tokens "
            "JOIN   users "
            "ON     users.id = tokens.id_users "
            "WHERE  tokens.source = ? "
            "AND    tokens.token  = ? "
            "LIMIT  1 "
            );
        st->Bind( source );
        st->Bind( token );
        if( st->Next() )
            return Reply( MakeToken( token, *st ) );
    }
    catch( const std::exception& err )
    {
        LOG_ERROR( log_ ) << err.what();
        LOG_ERROR( log_ ) << "[sql] Unable to authenticate token";
    }
    return Reply( web::UNAUTHORIZED );
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
Reply UserController::UpdateLogin( const std::string& username, const std::string& current, const std::string& password, const std::string& source )
{
    try
    {
        Sql_ABC::T_Transaction tr = db_.Begin();
        int id; std::string hash, name, type, lang, token; bool temporary;
        bool valid = FetchUser( db_, *tr, username, source, id, hash, name, type, temporary, lang, token );
        if( !valid )
            return Reply( web::NOT_FOUND );

        if( !crypt_.Validate( current, hash ) )
            return Reply( web::UNAUTHORIZED );

        DeleteTokenWithUserId( db_, *tr, id );
        Sql_ABC::T_Statement st = db_.Prepare( *tr,
            "UPDATE users "
            "SET    hash = ?, temporary = ?"
            "WHERE  id = ?" );
        st->Bind( crypt_.Hash( password ) );
        st->Bind( false );
        st->Bind( id );
        Execute( *st );
        const std::string sid = CreateToken( uuids_, db_, *tr, id, source );
        db_.Commit( *tr );
        return Reply( MakeToken( sid, id, username, name, type, false, lang ) );
    }
    catch( const std::exception& err )
    {
        LOG_ERROR( log_ ) << err.what();
        LOG_ERROR( log_ ) << "[sql] Unable to update login";
    }
    return Reply( web::INTERNAL_SERVER_ERROR );
}

// -----------------------------------------------------------------------------
// Name: UserController::ListUsers
// Created: BAX 2012-07-11
// -----------------------------------------------------------------------------
Reply UserController::ListUsers( int offset, int limit ) const
{
    try
    {
        Sql_ABC::T_Transaction tr = db_.Begin( false );
        Sql_ABC::T_Statement st = db_.Prepare( *tr,
            "SELECT id, username, name, type, temporary, language "
            "FROM users "
            "LIMIT ? OFFSET ?" );
        st->Bind( limit );
        st->Bind( offset );
        std::vector< Tree > list;
        while( st->Next() )
        {
            Tree tree;
            PutUser( tree, *st );
            list.push_back( tree );
        }
        return Reply( list );
    }
    catch( const std::exception& err )
    {
        LOG_ERROR( log_ ) << err.what();
        LOG_ERROR( log_ ) << "[sql] Unable to list users";
    }
    return Reply( web::INTERNAL_SERVER_ERROR );
}

// -----------------------------------------------------------------------------
// Name: UserController::CountUsers
// Created: BAX 2012-07-11
// -----------------------------------------------------------------------------
Reply UserController::CountUsers() const
{
    try
    {
        Sql_ABC::T_Transaction tr = db_.Begin( false );
        Sql_ABC::T_Statement st = db_.Prepare( *tr,
                "SELECT DISTINCT id FROM users" );
        const size_t size = Execute( *st );
        Tree tree;
        tree.put( "count", size );
        return Reply( tree );
    }
    catch( const std::exception& err )
    {
        LOG_ERROR( log_ ) << err.what();
        LOG_ERROR( log_ ) << "[sql] Unable to count users";
    }
    return Reply( web::INTERNAL_SERVER_ERROR );
}

// -----------------------------------------------------------------------------
// Name: UserController::GetUser
// Created: BAX 2012-07-11
// -----------------------------------------------------------------------------
Reply UserController::GetUser( int id ) const
{
    try
    {
        Sql_ABC::T_Transaction tr = db_.Begin( false );
        Sql_ABC::T_Statement st = db_.Prepare( *tr,
            "SELECT id, username, name, type, temporary, language "
            "FROM users "
            "WHERE id = ?" );
        st->Bind( id );
        if( !st->Next() )
            return Reply( web::NOT_FOUND );

        Tree tree;
        PutUser( tree, *st );
        return Reply( tree );
    }
    catch( const std::exception& err )
    {
        LOG_ERROR( log_ ) << err.what();
        LOG_ERROR( log_ ) << "[sql] Unable to get user";
    }
    return Reply( web::INTERNAL_SERVER_ERROR );
}

// -----------------------------------------------------------------------------
// Name: UserController::CreateUser
// Created: BAX 2012-07-11
// -----------------------------------------------------------------------------
Reply UserController::CreateUser( const std::string& username, const std::string& name, const std::string& password, bool temporary )
{
    try
    {
        Sql_ABC::T_Transaction tr = db_.Begin();
        const UserType type = USER_TYPE_ADMINISTRATOR;
        const std::string lang = "eng";
        ::CreateUser( db_, *tr, crypt_, username, name, password, type, temporary, lang );
        db_.Commit( *tr );
        tr.reset();
        Tree tree;
        PutUser( tree, static_cast< int >( db_.LastId() ), username, name, Convert( type ), temporary, lang );
        return Reply( tree );
    }
    catch( const std::exception& err )
    {
        LOG_ERROR( log_ ) << err.what();
        LOG_ERROR( log_ ) << "[sql] Unable to create user";
    }
    return Reply( web::INTERNAL_SERVER_ERROR );
}

// -----------------------------------------------------------------------------
// Name: UserController::DeleteUser
// Created: BAX 2012-07-11
// -----------------------------------------------------------------------------
Reply UserController::DeleteUser( const std::string& token, int id )
{
    try
    {
        Sql_ABC::T_Transaction tr = db_.Begin();
        Sql_ABC::T_Statement st = db_.Prepare( *tr,
            "SELECT     users.id, users.username, users.name, users.type, users.temporary, users.language, tokens.token "
            "FROM       users "
            "LEFT JOIN  tokens "
            "ON         tokens.id_users = users.id "
            "AND        tokens.token    = ? "
            "WHERE      users.id = ?" );
        st->Bind( token );
        st->Bind( id );
        bool valid = st->Next();
        if( !valid )
            return Reply( web::NOT_FOUND );

        Tree tree;
        PutUser( tree, *st );
        // check whether we are not deleting ourselves, which is forbidden
        if( st->IsColumnDefined() )
            return Reply( web::FORBIDDEN );

        DeleteTokenWithUserId( db_, *tr, id );
        st = db_.Prepare( *tr, "DELETE FROM users WHERE users.id = ?" );
        st->Bind( id );
        Execute( *st );
        db_.Commit( *tr );
        return Reply( tree );
    }
    catch( const std::exception& err )
    {
        LOG_ERROR( log_ ) << err.what();
        LOG_ERROR( log_ ) << "[sql] Unable to create user";
    }
    return Reply( web::INTERNAL_SERVER_ERROR );
}

// -----------------------------------------------------------------------------
// Name: UserController::UpdateUser
// Created: BAX 2012-07-11
// -----------------------------------------------------------------------------
Reply UserController::UpdateUser( int id, const std::string& username, const std::string& name, bool temporary )
{
    try
    {
        Sql_ABC::T_Transaction tr = db_.Begin();
        Sql_ABC::T_Statement st = db_.Prepare( *tr,
            "UPDATE users "
            "SET    username = ?, name = ?, temporary = ? "
            "WHERE  users.id = ?" );
        st->Bind( username );
        st->Bind( name );
        st->Bind( temporary );
        st->Bind( id );
        Execute( *st );
        st.reset();
        std::string dummy, type, lang;
        bool done = FetchUser( db_, *tr, username, dummy, id, dummy, dummy, type, temporary, lang, dummy );
        if( !done )
            return Reply( web::INTERNAL_SERVER_ERROR );
        db_.Commit( *tr );
        Tree tree;
        PutUser( tree, id, username, name, type, temporary, lang );
        return Reply( tree );
    }
    catch( const std::exception& err )
    {
        LOG_ERROR( log_ ) << err.what();
        LOG_ERROR( log_ ) << "[sql] Unable to create user";
    }
    return Reply( web::INTERNAL_SERVER_ERROR );
}