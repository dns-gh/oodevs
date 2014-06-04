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
    typedef boost::property_tree::ptree Tree;
    typedef boost::uuids::uuid Uuid;
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
    virtual Tree                Login          ( const std::string& user, const std::string& password );
    virtual Tree                IsAuthenticated( const std::string& token );
    virtual void                Logout         ( const std::string& token );
    virtual Tree                UpdateLogin    ( const std::string& user, const std::string& current, const std::string& password );
    virtual std::vector< Tree > ListUsers      ( const Uuid& node, int offset, int limit ) const;
    virtual size_t              CountUsers     ( const Uuid& node ) const;
    virtual Tree                GetUser        ( const Uuid& node, int id ) const;
    virtual Tree                CreateUser     ( const Uuid& node, const std::string& username, const std::string& name, const std::string& password, web::UserType type, bool temporary );
    virtual Tree                DeleteUser     ( const Uuid& node, const std::string& token, int id );
    virtual void                DeleteUsers    ( const Uuid& node );
    virtual Tree                UpdateUser     ( const Uuid& node, const std::string& token, int id, const std::string& user, const std::string& name, bool temporary, const boost::optional< std::string >& password );
    //@}

    //! @name UserController_ABC Methods
    //@{
    virtual Tree IsAuthenticated( const std::string& token, const boost::optional< std::string >& now );
    virtual int  GetExpirationLimitDays() const;
    //@}

private:
    //! @name Private methods
    //@{
    void MigrateDatabase();
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
