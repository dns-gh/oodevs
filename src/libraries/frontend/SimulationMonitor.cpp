// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "frontend_pch.h"
#include "SimulationMonitor.h"
#include "protocol/Protocol.h"
#pragma warning( push, 0 )
#include <boost/date_time/posix_time/posix_time.hpp>
#pragma warning( pop )

using namespace frontend;

struct SimulationMonitor::TimeOut : private boost::noncopyable
{
    explicit TimeOut( unsigned int duration )
        : duration_( duration )
        , start_   ( boost::posix_time::microsec_clock::universal_time() )
    {}
    bool Expired() const
    {
        return ( boost::posix_time::microsec_clock::universal_time() - start_ ).total_milliseconds() > duration_;
    }
    const unsigned int duration_;
    boost::posix_time::ptime start_;
};

// -----------------------------------------------------------------------------
// Name: SimulationMonitor constructor
// Created: SBO 2010-10-26
// -----------------------------------------------------------------------------
SimulationMonitor::SimulationMonitor( const std::string& host )
    : tools::ClientNetworker( host, true )
    , timeOut_  ( new TimeOut(
#ifdef _DEBUG
                    600000 // 10mn
#else
                    60000 // 1mn
#endif
                ) )
    , connected_( false )
{
    RegisterMessage( *this, &SimulationMonitor::HandleAarToClient );
    RegisterMessage( *this, &SimulationMonitor::HandleAuthenticationToClient );
    RegisterMessage( *this, &SimulationMonitor::HandleDispatcherToClient );
    RegisterMessage( *this, &SimulationMonitor::HandleMessengerToClient );
    RegisterMessage( *this, &SimulationMonitor::HandleReplayToClient );
    RegisterMessage( *this, &SimulationMonitor::HandleSimToClient );
}

// -----------------------------------------------------------------------------
// Name: SimulationMonitor destructor
// Created: SBO 2010-10-26
// -----------------------------------------------------------------------------
SimulationMonitor::~SimulationMonitor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SimulationMonitor::ConnectionSucceeded
// Created: SBO 2010-10-26
// -----------------------------------------------------------------------------
void SimulationMonitor::ConnectionSucceeded( const std::string& local, const std::string& remote )
{
    tools::ClientNetworker::ConnectionSucceeded( local, remote );
    connected_ = true;
}

// -----------------------------------------------------------------------------
// Name: SimulationMonitor::ConnectionFailed
// Created: SBO 2010-10-26
// -----------------------------------------------------------------------------
void SimulationMonitor::ConnectionFailed( const std::string& endpoint, const std::string& reason )
{
    tools::ClientNetworker::ConnectionFailed( endpoint, reason );
    connected_ = false;
}

// -----------------------------------------------------------------------------
// Name: SimulationMonitor::ConnectionError
// Created: SBO 2010-10-26
// -----------------------------------------------------------------------------
void SimulationMonitor::ConnectionError( const std::string& endpoint, const std::string& reason )
{
    tools::ClientNetworker::ConnectionError( endpoint, reason );
    connected_ = false;
}

// -----------------------------------------------------------------------------
// Name: SimulationMonitor::ConnectionWarning
// Created: MCO 2011-09-28
// -----------------------------------------------------------------------------
void SimulationMonitor::ConnectionWarning( const std::string& endpoint, const std::string& reason )
{
    tools::ClientNetworker::ConnectionWarning( endpoint, reason );
}

// -----------------------------------------------------------------------------
// Name: SimulationMonitor::Connected
// Created: SBO 2010-11-12
// -----------------------------------------------------------------------------
bool SimulationMonitor::Connected() const
{
    return connected_ || timeOut_->Expired();
}

// -----------------------------------------------------------------------------
// Name: SimulationMonitor::HandleAarToClient
// Created: SBO 2010-10-28
// -----------------------------------------------------------------------------
void SimulationMonitor::HandleAarToClient( const std::string& /*endpoint*/, const sword::AarToClient& /*message*/ )
{

}

// -----------------------------------------------------------------------------
// Name: SimulationMonitor::HandleAuthenticationToClient
// Created: SBO 2010-10-28
// -----------------------------------------------------------------------------
void SimulationMonitor::HandleAuthenticationToClient( const std::string& /*endpoint*/, const sword::AuthenticationToClient& /*message*/ )
{
    connected_ = true;
}

// -----------------------------------------------------------------------------
// Name: SimulationMonitor::HandleDispatcherToClient
// Created: SBO 2010-10-28
// -----------------------------------------------------------------------------
void SimulationMonitor::HandleDispatcherToClient( const std::string& /*endpoint*/, const sword::DispatcherToClient& /*message*/ )
{
    connected_ = true;
}

// -----------------------------------------------------------------------------
// Name: SimulationMonitor::HandleMessengerToClient
// Created: SBO 2010-10-28
// -----------------------------------------------------------------------------
void SimulationMonitor::HandleMessengerToClient( const std::string& /*endpoint*/, const sword::MessengerToClient& /*message*/ )
{

}

// -----------------------------------------------------------------------------
// Name: SimulationMonitor::HandleReplayToClient
// Created: SBO 2010-10-28
// -----------------------------------------------------------------------------
void SimulationMonitor::HandleReplayToClient( const std::string& /*endpoint*/, const sword::ReplayToClient& /*message*/ )
{

}

// -----------------------------------------------------------------------------
// Name: SimulationMonitor::HandleSimToClient
// Created: SBO 2010-10-28
// -----------------------------------------------------------------------------
void SimulationMonitor::HandleSimToClient( const std::string& /*endpoint*/, const sword::SimToClient& /*message*/ )
{

}
