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

#include <boost/shared_ptr.hpp>

using namespace host;
using runtime::Utf8Convert;

// -----------------------------------------------------------------------------
// Name: UserController::UserController
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
UserController::UserController( cpplog::BaseLogger& log,
                                Sql_ABC& db )
    : log_( log )
    , db_ ( db )
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
        ", email    TEXT"
        ", name     TEXT"
        ", hash     TEXT"
        ", salt     TEXT"
        ", reset    BOOLEAN"
        ", language TEXT"
        ")" )->Next();
    db.Prepare( *tr, "CREATE TABLE IF NOT EXISTS tokens ("
        "  id       INTEGER PRIMARY KEY"
        ", token    TEXT"
        ", expire   TEXT"
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
          "( email, name, hash, salt, reset, language )"
          "VALUES ( ?, ?, ?, ?, ?, ? )" );
    st->Bind( "admin" );
    st->Bind( "Admin" );
    st->Bind( "hash" );//FIXME
    st->Bind( "salt" );//FIXME
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

// -----------------------------------------------------------------------------
// Name: UserController::GetToken
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
Tree UserController::GetToken( const std::string& username, const std::string& password )
{
    return Tree();
}
