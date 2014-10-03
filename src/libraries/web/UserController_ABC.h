// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef USER_CONTROLLER_ABC_H
#define USER_CONTROLLER_ABC_H

#include "User.h"

#include <boost/noncopyable.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <string>
#include <vector>

namespace boost
{
    template< typename T > class optional;
namespace uuids
{
    struct uuid;
}
}

namespace web
{
    typedef boost::property_tree::ptree Tree;
    typedef boost::uuids::uuid Uuid;
}

namespace web
{
// =============================================================================
/** @class  UserController_ABC
    @brief  UserController_ABC interface
*/
// Created: BAX 2012-06-28
// =============================================================================
struct UserController_ABC : public boost::noncopyable
{
    //! @name Constructors/Destructor
    //@{
             UserController_ABC() {}
    virtual ~UserController_ABC() {}
    //@}

    //! @name Methods
    //@{
    virtual Tree                Login          ( const std::string& user, const std::string& password ) = 0;
    virtual Tree                IsAuthenticated( const std::string& token ) = 0;
    virtual void                Logout         ( const std::string& token ) = 0;
    virtual Tree                UpdateLogin    ( const std::string& user, const std::string& current, const std::string& password ) = 0;
    virtual std::vector< Tree > ListUsers      ( const Uuid& node, int offset, int limit ) const = 0;
    virtual size_t              CountUsers     ( const Uuid& node ) const = 0;
    virtual Tree                GetUser        ( const Uuid& node, int id ) const = 0;
    virtual Tree                CreateUser     ( const Uuid& node, const std::string& username, const std::string& name, const std::string& password, UserType type, bool temporary ) = 0;
    virtual Tree                DeleteUser     ( const Uuid& node, const std::string& token, int id ) = 0;
    virtual void                DeleteUsers    ( const Uuid& node ) = 0;
    virtual Tree                UpdateUser     ( const Uuid& node, const std::string& token, int id, const std::string& user, const std::string& name, bool temporary, const boost::optional< std::string >& password ) = 0;
    //@}
};
}

#endif // USER_CONTROLLER_ABC_H
