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
class Profile;

namespace kernel
{
    class Logger_ABC;
}

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
             Network( Simulation& simu, Profile& profile, kernel::Logger_ABC& logger );
    virtual ~Network();
    //@}

    //! @name Operations
    //@{
    bool Connect( const std::string& strHost );
    bool Connect( const std::string& strHostName, unsigned short nPort );
    bool Disconnect();
    bool IsConnected() const;

    using tools::ClientNetworker::Update;

    AgentServerMsgMgr& GetMessageMgr();
    //@}

    //! @name Statistics
    //@{
    unsigned long GetReceivedAmount() const;
    unsigned long GetSentAmount() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Network( const Network& );            //!< Copy constructor
    Network& operator=( const Network& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void ConnectionSucceeded( const std::string& endpoint );
    virtual void ConnectionFailed   ( const std::string& address, const std::string& error );
    virtual void ConnectionError    ( const std::string& address, const std::string& error );
    //@}

private:
    //! @name Member data
    //@{
    Simulation& simu_;
    Profile& profile_;
    kernel::Logger_ABC& logger_;
    std::string session_;
    AgentServerMsgMgr* manager_;
    //@}
};

#endif // __Network_h_
