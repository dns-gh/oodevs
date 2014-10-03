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
#include "User.h"
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
    class  Plugins;
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
             Controller( const Plugins& plugins, cpplog::BaseLogger& log, Agent_ABC& host, UserController_ABC& users, bool secure );
    virtual ~Controller();
    //@}

    //! @name Operations
    //@{
    virtual void DoGet ( Reply_ABC& reply, Request_ABC& request );
    virtual void DoPost( Reply_ABC& reply, Request_ABC& request );
    //@}

private:
    //! @name Cluster Methods
    //@{
    void GetCluster  ( Reply_ABC& reply, const Request_ABC& request );
    void StartCluster( Reply_ABC& reply, const Request_ABC& request );
    void StopCluster ( Reply_ABC& reply, const Request_ABC& request );
    //@}

    //! @name Node Methods
    //@{
    void CreateNode( Reply_ABC& reply, Request_ABC& request );
    void UpdateNode( Reply_ABC& reply, Request_ABC& request );
    void ListNodes ( Reply_ABC& reply, const Request_ABC& request );
    void CountNodes( Reply_ABC& reply, const Request_ABC& request );
    void GetNode   ( Reply_ABC& reply, const Request_ABC& request );
    void DeleteNode( Reply_ABC& reply, const Request_ABC& request );
    void StartNode ( Reply_ABC& reply, const Request_ABC& request );
    void StopNode  ( Reply_ABC& reply, const Request_ABC& request );
    //@}

    //! @name Client Methods
    //@{
    void GetClient     ( Reply_ABC& reply, const Request_ABC& request );
    void DownloadClient( Reply_ABC& reply, const Request_ABC& request );
    //@}

    //! @name Install Methods
    //@{
    void GetInstall   ( Reply_ABC& reply, const Request_ABC& request );
    void DeleteInstall( Reply_ABC& reply, const Request_ABC& request );
    //@}

    //! @name Cache Methods
    //@{
    void UploadCache     ( Reply_ABC& reply, Request_ABC& request );
    void GetCache        ( Reply_ABC& reply, const Request_ABC& request );
    void DeleteCache     ( Reply_ABC& reply, const Request_ABC& request );
    void InstallFromCache( Reply_ABC& reply, const Request_ABC& request );
    void DownloadInstall ( Reply_ABC& reply, const Request_ABC& request );
    //@}

    //! @name Session Methods
    //@{
    void CreateSession     ( Reply_ABC& reply, Request_ABC& request );
    void UpdateSession     ( Reply_ABC& reply, Request_ABC& request );
    void ListSessions      ( Reply_ABC& reply, const Request_ABC& request );
    void CountSessions     ( Reply_ABC& reply, const Request_ABC& request );
    void GetSession        ( Reply_ABC& reply, const Request_ABC& request );
    void DeleteSession     ( Reply_ABC& reply, const Request_ABC& request );
    void StartSession      ( Reply_ABC& reply, const Request_ABC& request );
    void StopSession       ( Reply_ABC& reply, const Request_ABC& request );
    void PauseSession      ( Reply_ABC& reply, const Request_ABC& request );
    void ArchiveSession    ( Reply_ABC& reply, const Request_ABC& request );
    void RestoreSession    ( Reply_ABC& reply, const Request_ABC& request );
    void DownloadSession   ( Reply_ABC& reply, const Request_ABC& request );
    void ReplaySession     ( Reply_ABC& reply, const Request_ABC& request );
    void DownloadSessionLog( Reply_ABC& reply, Request_ABC& request );
    //@}

    //! @name Exercise Methods
    //@{
    void ListExercises ( Reply_ABC& reply, const Request_ABC& request );
    void CountExercises( Reply_ABC& reply, const Request_ABC& request );
    //@}

    //! @name Plugin Methods
    //@{
    void ListPlugins ( Reply_ABC& reply, const Request_ABC& request );
    void CountPlugins( Reply_ABC& reply, const Request_ABC& request );
    //@}

    //! @name User Methods
    //@{
    void UserLogin          ( Reply_ABC& reply, Request_ABC& request );
    void UserLogout         ( Reply_ABC& reply, const Request_ABC& request );
    void UserIsAuthenticated( Reply_ABC& reply, const Request_ABC& request );
    void UserUpdateLogin    ( Reply_ABC& reply, Request_ABC& request );
    void ListUsers          ( Reply_ABC& reply, const Request_ABC& request );
    void CountUsers         ( Reply_ABC& reply, const Request_ABC& request );
    void GetUser            ( Reply_ABC& reply, const Request_ABC& request );
    void CreateUser         ( Reply_ABC& reply, Request_ABC& request );
    void DeleteUser         ( Reply_ABC& reply, const Request_ABC& request );
    void UpdateUser         ( Reply_ABC& reply, const Request_ABC& request );
    //@}

    //! @name Licences Methods
    //@{
    void ListLicenses       ( Reply_ABC& reply, const Request_ABC& request );
    void UploadLicenses     ( Reply_ABC& reply, Request_ABC& request );

    //@}

    //! @name Helpers
    //@{
    void Authenticate    ( const Request_ABC& request, UserType required );
    User AuthenticateUser( const Request_ABC& request, UserType required, const std::string& id );
    Uuid AuthenticateNode( const Request_ABC& request, UserType required, const std::string& id );
    //@}

private:
    //! @name Member data
    //@{
    const Plugins& plugins_;
    cpplog::BaseLogger& log_;
    Agent_ABC& agent_;
    UserController_ABC& users_;
    const bool secure_;
    //@}
};
}

#endif // CONTROLLER_H
