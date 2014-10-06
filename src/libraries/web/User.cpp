// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "User.h"

#include "runtime/Helpers.h"

#include <boost/static_assert.hpp>

using namespace web;

static const struct { UserType type; std::string text; } types_map[] =
{
    { USER_TYPE_ADMINISTRATOR, "administrator" },
    { USER_TYPE_MANAGER,       "manager" },
    { USER_TYPE_USER,          "user" },
    { USER_TYPE_PLAYER,        "player" },
};

BOOST_STATIC_ASSERT( COUNT_OF( types_map ) == USER_TYPE_COUNT );

// -----------------------------------------------------------------------------
// Name: Convert
// Created: BAX 2012-07-23
// -----------------------------------------------------------------------------
std::string web::ConvertUserType( UserType type )
{
    if( type < 0 || type >= USER_TYPE_COUNT )
        return "invalid";
    return types_map[type].text;
}

// -----------------------------------------------------------------------------
// Name: Convert
// Created: BAX 2012-07-23
// -----------------------------------------------------------------------------
UserType web::ConvertUserType( const std::string& type )
{
    for( size_t i = 0; i < COUNT_OF( types_map ); ++i )
        if( types_map[i].text == type )
            return types_map[i].type;
    return USER_TYPE_COUNT;
}
