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

#include "web/UserController_ABC.h"

namespace cpplog
{
    class BaseLogger;
}

namespace host
{
    struct Crypt_ABC;
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
struct UserController : public web::UserController_ABC
{
    //! @name Constructors/Destructor
    //@{
             UserController( cpplog::BaseLogger& log,
                             const Crypt_ABC& crypt,
                             UuidFactory_ABC& uuids,
                             Sql_ABC& db );
    virtual ~UserController();
    //@}

    //! @name UserController_ABC Methods
    //@{
    virtual web::Reply Login( const std::string& user, const std::string& password, const std::string& source );
    virtual web::Reply IsAuthenticated( const std::string& token, const std::string& source );
    virtual void       Logout( const std::string& token );
    virtual web::Reply UpdateLogin( const std::string& user, const std::string& current, const std::string& password, const std::string& source );
    virtual web::Reply ListUsers( int offset, int limit ) const;
    virtual web::Reply CountUsers() const;
    virtual web::Reply GetUser( int id ) const;
    virtual web::Reply CreateUser( const std::string& username, const std::string& name, const std::string& password, bool temporary );
    virtual web::Reply DeleteUser( const std::string& token, int id );
    virtual web::Reply UpdateUser( const std::string& token, int id, const std::string& user, const std::string& name, bool temporary, const boost::optional< std::string >& password );
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
    const Crypt_ABC& crypt_;
    UuidFactory_ABC& uuids_;
    Sql_ABC& db_;
    //@}
};
}

#endif // USER_CONTROLLER_ABC_H
