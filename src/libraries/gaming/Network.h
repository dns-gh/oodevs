// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Network_h_
#define __Network_h_

#include "tools/ClientNetworker.h"

class AgentServerMsgMgr;
class Model;
class Simulation;

namespace kernel
{
    class Controller;
    class Logger_ABC;
}

class CommandHandler;
class Services;

// =============================================================================
/** @class  Network
    @brief  Network
*/
// Created: AGE 2006-02-08
// =============================================================================
class Network : public tools::ClientNetworker
{
public:
    //! @name Constructors/Destructor
    //@{
             Network( Services& services, Simulation& simu, kernel::Logger_ABC& logger, unsigned long timeOut );
    virtual ~Network();
    //@}

    //! @name Operations
    //@{
    bool DoConnect( const std::string& strHost );
    bool Connect( const std::string& strHostName, unsigned short nPort );
    bool Disconnect();
    bool IsConnected() const;
    bool Reconnect();

    using tools::ClientNetworker::Update;

    AgentServerMsgMgr& GetMessageMgr();
    CommandHandler& GetCommands();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Network( const Network& );            //!< Copy constructor
    Network& operator=( const Network& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void ConnectionSucceeded( const std::string& local, const std::string& remote );
    virtual void ConnectionFailed   ( const std::string& address, const std::string& error );
    virtual void ConnectionError    ( const std::string& address, const std::string& error );
    virtual void ConnectionWarning  ( const std::string& address, const std::string& error );
    //@}

private:
    //! @name Member data
    //@{
    Services& services_;
    Simulation& simu_;
    kernel::Logger_ABC& logger_;
    std::string session_;
    std::string host_;
    CommandHandler* commands_;
    AgentServerMsgMgr* manager_;
    //@}
};

#endif // __Network_h_
