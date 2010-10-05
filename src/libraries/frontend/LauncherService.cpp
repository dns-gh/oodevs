// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "frontend_pch.h"
#include "LauncherService.h"
#include "LauncherPublisher.h"

using namespace frontend;

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
    clients_[ endpoint ] = new LauncherPublisher( *this, endpoint );
    tools::ServerNetworker::ConnectionSucceeded( endpoint );
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
    return *clients_.at( endpoint );
}
