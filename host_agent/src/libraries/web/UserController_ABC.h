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
#include <string>

namespace boost
{
    template< typename T > class optional;
}

namespace web
{
    struct Reply;
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
    virtual Reply Login( const std::string& user, const std::string& password, const std::string& source ) = 0;
    virtual Reply IsAuthenticated( const std::string& token, const std::string& source ) = 0;
    virtual void  Logout( const std::string& token ) = 0;
    virtual Reply UpdateLogin( const std::string& user, const std::string& current, const std::string& password, const std::string& source ) = 0;
    virtual Reply ListUsers( int offset, int limit ) const = 0;
    virtual Reply CountUsers() const = 0;
    virtual Reply GetUser( int id ) const = 0;
    virtual Reply CreateUser( const std::string& username, const std::string& name, const std::string& password, bool temporary ) = 0;
    virtual Reply DeleteUser( const std::string& token, int id ) = 0;
    virtual Reply UpdateUser( const std::string& token, int id, const std::string& user, const std::string& name, bool temporary, const boost::optional< std::string >& password ) = 0;
    //@}
};
}

#endif // USER_CONTROLLER_ABC_H
