// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __LauncherClient_h_
#define __LauncherClient_h_

#include "tools/ClientNetworker.h"

namespace MsgsLauncherToAdmin
{
    class MsgLauncherToAdmin;
}

namespace kernel
{
    class Controller;
}

namespace tools
{
    class GeneralConfig;
}

namespace frontend
{
    class ConnectionHandler_ABC;
    class Host_ABC;
    class LauncherPublisher;
    class Model;

// =============================================================================
/** @class  LauncherClient
    @brief  LauncherClient
*/
// Created: SBO 2010-09-29
// =============================================================================
class LauncherClient : public tools::ClientNetworker
{

public:
    //! @name Constructors/Destructor
    //@{
             LauncherClient( const tools::GeneralConfig& config, kernel::Controller& controller );
    virtual ~LauncherClient();
    //@}

    //! @name Operations
    //@{
    void Connect( const std::string& host, unsigned int port, frontend::ConnectionHandler_ABC& handler );
    bool Connected() const;
    void QueryExerciseList();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LauncherClient( const LauncherClient& );            //!< Copy constructor
    LauncherClient& operator=( const LauncherClient& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void ConnectionSucceeded( const std::string& endpoint );
    virtual void ConnectionError( const std::string& address, const std::string& error );
    virtual void ConnectionFailed( const std::string& address, const std::string& error );
    void HandleLauncherToAdmin( const std::string& endpoint, const MsgsLauncherToAdmin::MsgLauncherToAdmin& message );
    void ResetConnection();
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< Model > model_;
    std::auto_ptr< LauncherPublisher > publisher_;
    Host_ABC* host_;
    ConnectionHandler_ABC* handler_;
    bool connected_;
    //@}
};

}

#endif // __LauncherClient_h_
