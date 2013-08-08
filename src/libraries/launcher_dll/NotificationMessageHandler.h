// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef launcher_NotificationMessageHandler_h
#define launcher_NotificationMessageHandler_h

#include "ClientMessageHandlerBase.h"
#include <boost/shared_ptr.hpp>

namespace launcher
{
// =============================================================================
/** @class  NotificationMessageHandler
    @brief  Notification message handler
*/
// Created: LGY 2011-05-18
// =============================================================================
class NotificationMessageHandler : public ClientMessageHandlerBase
{
public:
    //! @name Constructors/Destructor
    //@{
             NotificationMessageHandler( boost::shared_ptr< LauncherPublisher > publisher, const std::string& exercise, const std::string& session );
    virtual ~NotificationMessageHandler();
    //@}

    //! @name Operations
    //@{
    virtual bool OnReceiveMessage( const sword::SimToClient& message );
    virtual bool OnReceiveMessage( const sword::AuthenticationToClient& message );
    virtual bool OnReceiveMessage( const sword::MessengerToClient& message );
    virtual void ResetCache();
    //@}

private:

    //! @name Types
    //@{
    struct EntityCreationChecker
    {
        EntityCreationChecker( unsigned int superiorAutomat, unsigned int superiorFormation )
            : superiorAutomat_( superiorAutomat ), superiorFormation_( superiorFormation ), bSent_( false ) {}
        EntityCreationChecker(): superiorAutomat_( 0 ), superiorFormation_( 0 ), bSent_( false ) {}

        void AddEntityCreation( bool isAutomat, SessionNotification& response, unsigned int id, const std::string& longName );

        std::string longName_;
        unsigned int superiorAutomat_;
        unsigned int superiorFormation_;
        bool bSent_;
    };
    //@}

    std::map< unsigned int , EntityCreationChecker > unitsCreationChecker_;
    std::map< unsigned int , EntityCreationChecker > automatCreationChecker_;
};

}

#endif // launcher_NotificationMessageHandler_h
