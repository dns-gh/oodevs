// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef USER_TYPE_H
#define USER_TYPE_H

#include <string>

namespace web
{
// -----------------------------------------------------------------------------
// Name: UserType enumeration
// Created: BAX 2012-07-23
// -----------------------------------------------------------------------------
enum UserType
{
    // order is critical
    USER_TYPE_ADMINISTRATOR,
    USER_TYPE_MANAGER,
    USER_TYPE_USER,
    USER_TYPE_PLAYER,
    USER_TYPE_COUNT,
};

// -----------------------------------------------------------------------------
// Name: UserType conversions
// Created: BAX 2012-07-23
// -----------------------------------------------------------------------------
std::string ConvertUserType( UserType type );
UserType ConvertUserType( const std::string& type );
}

#endif // USER_TYPE_H
