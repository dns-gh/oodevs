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

namespace sword
{
    class LauncherToAdmin;
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
    class LauncherPublisher;
    class ResponseHandler_ABC;
    class ResponseHandlerProxy;

// =============================================================================
/** @class  LauncherClient
    @brief  Launcher client
*/
// Created: SBO 2010-09-29
// =============================================================================
class LauncherClient : public tools::ClientNetworker
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit LauncherClient( kernel::Controller& controller );
    virtual ~LauncherClient();
    //@}

    //! @name Operations
    //@{
    const std::string& Endpoint() const;
    //@}

    //! @name Operations
    //@{
    void Connect( const std::string& host, unsigned int port, frontend::ConnectionHandler_ABC& handler );
    bool Connected() const;
    void RegisterLauncherHandler( boost::shared_ptr< ResponseHandler_ABC > handler );
    void QueryExerciseList();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LauncherClient( const LauncherClient& );            //!< Copy constructor
    LauncherClient& operator=( const LauncherClient& ); //!< Assignment operator
    //@}

    //! @name Operations
    //@{
    virtual void ConnectionSucceeded( const std::string& local, const std::string& remote );
    virtual void ConnectionFailed( const std::string& address, const std::string& error );
    virtual void ConnectionError( const std::string& address, const std::string& error );
    virtual void ConnectionWarning( const std::string& address, const std::string& warning );
    //@}

    //! @name Helpers
    //@{
    void HandleLauncherToAdmin( const std::string& endpoint, const sword::LauncherToAdmin& message );
    void ResetConnection();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    ConnectionHandler_ABC* handler_;
    std::auto_ptr< LauncherPublisher > publisher_;
    std::auto_ptr< ResponseHandlerProxy > responseHandler_;
    bool connected_;
    //@}
};

}

#endif // __LauncherClient_h_
