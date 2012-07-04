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
    virtual std::string Login( const std::string& user, const std::string& password, const std::string& source ) = 0;
    virtual std::string IsAuthenticated( const std::string& token, const std::string& source ) = 0;
    virtual void        Logout( const std::string& token ) = 0;
    //@}
};
}

#endif // USER_CONTROLLER_ABC_H
