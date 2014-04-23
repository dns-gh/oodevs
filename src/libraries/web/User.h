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
#include <boost/uuid/uuid.hpp>

namespace web
{
    typedef boost::uuids::uuid Uuid;
}

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

// -----------------------------------------------------------------------------
// Name: User struct
// Created: LGY 2014-04-04
// -----------------------------------------------------------------------------
struct User
{
    User( int id, const std::string& name, UserType type, const Uuid& node )
        : id( id )
        , name( name )
        , type( type )
        , node( node )
    {
        // NOTHING
    }
    User( int id, const std::string& name )
        : id( id )
        , name( name )
    {
        // NOTHING
    }
    int         id;
    std::string name;
    UserType    type;
    Uuid        node;
};
}

#endif // USER_TYPE_H
