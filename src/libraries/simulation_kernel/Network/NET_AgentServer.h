// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2002 MASA Group
//
// *****************************************************************************

#ifndef __NET_AgentServer_h_
#define __NET_AgentServer_h_

#include "tools/ServerNetworker.h"
#include <boost/scoped_ptr.hpp>

class MIL_Config;
class NET_Simulation_ABC;
class NET_AS_MOSServerMsgMgr;

//=============================================================================
// Created: NLD 2002-07-12
//=============================================================================
class NET_AgentServer : public tools::ServerNetworker
{
public:
             NET_AgentServer( const MIL_Config& config, NET_Simulation_ABC& simulation );
    virtual ~NET_AgentServer();

    void Update();

protected:
    virtual void ConnectionSucceeded( const std::string& local, const std::string& remote );
    virtual void ConnectionFailed   ( const std::string& address, const std::string& error );
    virtual void ConnectionError    ( const std::string& address, const std::string& error );
    virtual void ConnectionWarning  ( const std::string& address, const std::string& warning );

private:
    boost::scoped_ptr< NET_AS_MOSServerMsgMgr > manager_;
};

#endif // __NET_AgentServer_h_
