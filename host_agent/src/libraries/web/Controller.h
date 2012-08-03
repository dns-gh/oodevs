// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Observer_ABC.h"
#include "UserType.h"
#include <string>

namespace boost
{
namespace uuids
{
    struct uuid;
};
}

namespace cpplog
{
    class BaseLogger;
};

namespace web
{
    struct Agent_ABC;
    struct Request_ABC;
    struct UserController_ABC;
    typedef boost::uuids::uuid Uuid;
}

namespace web
{
// =============================================================================
/** @class  Controller
    @brief  Process controller
*/
// Created: BAX 2012-03-07
// =============================================================================
class Controller : public Observer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Controller( cpplog::BaseLogger& log, Agent_ABC& host, UserController_ABC& users, bool secure );
    virtual ~Controller();
    //@}

    //! @name Operations
    //@{
    virtual std::string DoGet ( Request_ABC& request );
    virtual std::string DoPost( Request_ABC& request );
    //@}

private:
    //! @name Cluster Methods
    //@{
    std::string GetCluster  ( const Request_ABC& request );
    std::string StartCluster( const Request_ABC& request );
    std::string StopCluster ( const Request_ABC& request );
    //@}

    //! @name Node Methods
    //@{
    std::string ListNodes ( const Request_ABC& request );
    std::string CountNodes( const Request_ABC& request );
    std::string GetNode   ( const Request_ABC& request );
    std::string CreateNode( const Request_ABC& request );
    std::string DeleteNode( const Request_ABC& request );
    std::string StartNode ( const Request_ABC& request );
    std::string StopNode  ( const Request_ABC& request );
    std::string UpdateNode( const Request_ABC& request );
    //@}

    //! @name Install Methods
    //@{
    std::string GetInstall   ( const Request_ABC& request );
    std::string DeleteInstall( const Request_ABC& request );
    //@}

    //! @name Cache Methods
    //@{
    std::string UploadCache     ( Request_ABC& request );
    std::string GetCache        ( const Request_ABC& request );
    std::string DeleteCache     ( const Request_ABC& request );
    std::string InstallFromCache( const Request_ABC& request );
    //@}

    //! @name Session Methods
    //@{
    std::string ListSessions ( const Request_ABC& request );
    std::string CountSessions( const Request_ABC& request );
    std::string GetSession   ( const Request_ABC& request );
    std::string CreateSession( const Request_ABC& request );
    std::string DeleteSession( const Request_ABC& request );
    std::string StartSession ( const Request_ABC& request );
    std::string StopSession  ( const Request_ABC& request );
    std::string PauseSession ( const Request_ABC& request );
    std::string UpdateSession( const Request_ABC& request );
    //@}

    //! @name Exercise Methods
    //@{
    std::string ListExercises ( const Request_ABC& request );
    std::string CountExercises( const Request_ABC& request );
    //@}

    //! @name User Methods
    //@{
    std::string UserLogin          ( Request_ABC& request );
    std::string UserLogout         ( const Request_ABC& request );
    std::string UserIsAuthenticated( const Request_ABC& request );
    std::string UserUpdateLogin    ( Request_ABC& request );
    std::string ListUsers          ( const Request_ABC& request );
    std::string CountUsers         ( const Request_ABC& request );
    std::string GetUser            ( const Request_ABC& request );
    std::string CreateUser         ( Request_ABC& request );
    std::string DeleteUser         ( const Request_ABC& request );
    std::string UpdateUser         ( const Request_ABC& request );
    //@}

    //! @name Helpers
    //@{
    void Authenticate( const Request_ABC& request, UserType required );
    Uuid AuthenticateNode( const Request_ABC& request, UserType required, const std::string& id );
    //@}

private:
    //! @name Member data
    //@{
    cpplog::BaseLogger& log_;
    Agent_ABC& agent_;
    UserController_ABC& users_;
    const bool secure_;
    //@}
};
}

#endif // CONTROLLER_H
