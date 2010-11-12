// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "Launcher_dll_pch.h"
#include "LauncherService.h"
#include "LauncherPublisher.h"

using namespace launcher;

// -----------------------------------------------------------------------------
// Name: LauncherService constructor
// Created: SBO 2010-09-29
// -----------------------------------------------------------------------------
LauncherService::LauncherService( unsigned short port )
    : tools::ServerNetworker( port )
{
    AllowConnections();
}

// -----------------------------------------------------------------------------
// Name: LauncherService destructor
// Created: SBO 2010-09-29
// -----------------------------------------------------------------------------
LauncherService::~LauncherService()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LauncherService::ConnectionSucceeded
// Created: SBO 2010-09-29
// -----------------------------------------------------------------------------
void LauncherService::ConnectionSucceeded( const std::string& endpoint )
{
    clients_[ endpoint ] = boost::shared_ptr< LauncherPublisher >( new LauncherPublisher( *this, endpoint ) );
    tools::ServerNetworker::ConnectionSucceeded( endpoint );
}

// -----------------------------------------------------------------------------
// Name: LauncherService::ConnectionFailed
// Created: SBO 2010-11-05
// -----------------------------------------------------------------------------
void LauncherService::ConnectionFailed( const std::string& address, const std::string& error )
{
    clients_.erase( address );
    tools::ServerNetworker::ConnectionFailed( address, error );
}

// -----------------------------------------------------------------------------
// Name: LauncherService::ConnectionError
// Created: SBO 2010-09-29
// -----------------------------------------------------------------------------
void LauncherService::ConnectionError( const std::string& address, const std::string& error )
{
    clients_.erase( address );
    tools::ServerNetworker::ConnectionError( address, error );
}

// -----------------------------------------------------------------------------
// Name: LauncherService::ResolveClient
// Created: SBO 2010-09-29
// -----------------------------------------------------------------------------
LauncherPublisher& LauncherService::ResolveClient( const std::string& endpoint ) const
{
    std::map< std::string, boost::shared_ptr< LauncherPublisher > >::const_iterator it = clients_.find( endpoint );
    if( it != clients_.end() )
        return *it->second;
    throw std::runtime_error( __FUNCTION__ ": client " + endpoint + " does not exist." );
}
