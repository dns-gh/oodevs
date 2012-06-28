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
    // NOTHING
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
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UserController::GetToken
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
Tree UserController::GetToken( const std::string& username, const std::string& password )
{
    return Tree();
}
