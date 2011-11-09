// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __LauncherService_h_
#define __LauncherService_h_

#include "tools/ServerNetworker.h"
#include "shield/ClientHandler_ABC.h"
#include <boost/shared_ptr.hpp>
#include <string>
#include <map>

namespace sword
{
    class ConnectionRequest;
}

namespace launcher
{
    class LauncherPublisher;

// =============================================================================
/** @class  LauncherService
    @brief  LauncherService
*/
// Created: SBO 2010-09-29
// =============================================================================
class LauncherService : public tools::ServerNetworker, public shield::ClientHandler_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit LauncherService( unsigned short port );
    virtual ~LauncherService();
    //@}

    //! @name Operations
    //@{
    LauncherPublisher& ResolveClient( const std::string& endpoint ) const;
    //@}

    //! @name Operations
    //@{
    virtual void Register( const std::string& endpoint, tools::MessageSender_ABC& sender, dispatcher::ClientBroadcaster_ABC& broadcaster );
    virtual void Unregister( const std::string& endpoint );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LauncherService( const LauncherService& );            //!< Copy constructor
    LauncherService& operator=( const LauncherService& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void ConnectionSucceeded( const std::string& endpoint );
    virtual void ConnectionFailed( const std::string& address, const std::string& error );
    virtual void ConnectionError( const std::string& address, const std::string& error );
    //@}

private:
    //! @name Member data
    //@{
    std::map< std::string, boost::shared_ptr< LauncherPublisher > > clients_;
    //@}
};

}

#endif // __LauncherService_h_
