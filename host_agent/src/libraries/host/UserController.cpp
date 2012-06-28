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
    MigrateDatabase();
}

// -----------------------------------------------------------------------------
// Name: UserController::UserController
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
UserController::~UserController()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UserController::MigrateDatabase
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
void UserController::MigrateDatabase()
{
    Sql_ABC::Ptr ptr = db_.Prepare(
        "CREATE TABLE IF NOT EXISTS revisions ("
        "  id       INTEGER PRIMARY KEY"
        ", revision INTEGER"
        ")" );
    ptr->Next();
    ptr = db_.Prepare(
        "CREATE TABLE IF NOT EXISTS users ("
        "  id    INTEGER PRIMARY KEY"
        ", email TEXT"
        ", name  TEXT"
        ", hash  TEXT"
        ", salt  TEXT"
        ", reset BOOLEAN"
        ")" );
    ptr->Next();
}

// -----------------------------------------------------------------------------
// Name: UserController::GetToken
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
Tree UserController::GetToken( const std::string& username, const std::string& password )
{
    return Tree();
}
