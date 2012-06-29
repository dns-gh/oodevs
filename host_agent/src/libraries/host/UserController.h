// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef USER_CONTROLLER_H
#define USER_CONTROLLER_H

#include "UserController_ABC.h"

namespace cpplog
{
    class BaseLogger;
}

namespace host
{
    struct Sql_ABC;
    struct UuidFactory_ABC;
}

namespace host
{
// =============================================================================
/** @class  UserController
    @brief  UserController class
*/
// Created: BAX 2012-06-28
// =============================================================================
struct UserController : public UserController_ABC
{
    //! @name Constructors/Destructor
    //@{
             UserController( cpplog::BaseLogger& log,
                             UuidFactory_ABC& uuids,
                             Sql_ABC& db );
    virtual ~UserController();
    //@}

    //! @name UserController_ABC Methods
    //@{
    virtual Tree Login( const std::string& user, const std::string& password );
    virtual bool IsAuthenticated( const std::string& token );
    virtual void Logout( const std::string& token );
    //@}

private:
    //! @name Private methods
    //@{
    void SetupDatabase();
    //@}

private:
    //! @name Private members
    //@{
    cpplog::BaseLogger& log_;
    UuidFactory_ABC& uuids_;
    Sql_ABC& db_;
    //@}
};
}

#endif // USER_CONTROLLER_ABC_H
