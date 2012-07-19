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

#include <boost/noncopyable.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <string>
#include <vector>

namespace boost
{
    template< typename T > class optional;
}

namespace web
{
    typedef boost::property_tree::ptree Tree;
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
    virtual Tree                Login          ( const std::string& user, const std::string& password, const std::string& source ) = 0;
    virtual Tree                IsAuthenticated( const std::string& token, const std::string& source ) = 0;
    virtual void                Logout         ( const std::string& token ) = 0;
    virtual Tree                UpdateLogin    ( const std::string& user, const std::string& current, const std::string& password, const std::string& source ) = 0;
    virtual std::vector< Tree > ListUsers      ( int offset, int limit ) const = 0;
    virtual size_t              CountUsers     () const = 0;
    virtual Tree                GetUser        ( int id ) const = 0;
    virtual Tree                CreateUser     ( const std::string& username, const std::string& name, const std::string& password, bool temporary ) = 0;
    virtual Tree                DeleteUser     ( const std::string& token, int id ) = 0;
    virtual Tree                UpdateUser     ( const std::string& token, int id, const std::string& user, const std::string& name, bool temporary, const boost::optional< std::string >& password ) = 0;
    //@}
};
}

#endif // USER_CONTROLLER_ABC_H
