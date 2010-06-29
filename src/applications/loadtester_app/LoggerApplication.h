// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __LoggerApplication_h_
#define __LoggerApplication_h_

#include "protocol/Protocol.h"
#include "tools/ClientNetworker.h"

#include <string>
#include <iostream>
#include <fstream>

// =============================================================================
/** @class  LoggerApplication
    @brief  LoggerApplication
*/
// Created: LDC 2009-09-02
// =============================================================================
class LoggerApplication : public tools::ClientNetworker
{

public:
    //! @name Constructors/Destructor
    //@{
             LoggerApplication( const std::string& hostname, const std::string& logFile, const std::string& login, const std::string& password, bool verbose );
    virtual ~LoggerApplication();
    //@}

    //! @name Operations
    //@{
    int Run();
    void Send( const MsgsClientToAuthentication::MsgClientToAuthentication& wrapper );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LoggerApplication( const LoggerApplication& );            //!< Copy constructor
    LoggerApplication& operator=( const LoggerApplication& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void ConnectionSucceeded( const std::string& endpoint );
    virtual void ConnectionFailed   ( const std::string& address, const std::string& error );
    virtual void ConnectionError    ( const std::string& address, const std::string& error );

    void OnReceiveMsgSimToClient           ( const std::string& from, const MsgsSimToClient::MsgSimToClient& wrapper );
    void OnReceiveMsgAuthenticationToClient( const std::string& from, const MsgsAuthenticationToClient::MsgAuthenticationToClient& wrapper );
    void OnReceiveMsgDispatcherToClient    ( const std::string& from, const MsgsDispatcherToClient::MsgDispatcherToClient& wrapper );
    void OnReceiveMsgMessengerToClient     ( const std::string& from, const MsgsMessengerToClient::MsgMessengerToClient& wrapper );

    void OnReceiveMsgReplayToClient        ( const std::string& from, const MsgsReplayToClient::MsgReplayToClient& wrapper );
    void OnReceiveMsgAarToClient           ( const std::string& from, const MsgsAarToClient::MsgAarToClient& wrapper );

    void OnReceiveMsgControlBeginTick( int tick );
    void LogMessage( const MsgsSimToClient::MsgSimToClient& wrapper );
    void DumpTime();
    //@}

private:
    //! @name Member data
    //@{
    std::ofstream file_;
    const std::string login_;
    std::string  password_;
    std::string  endpoint_;
    bool          bConnectionLost_;
    bool          bVerbose_;
    //@}
};

#endif // __LoggerApplication_h_
