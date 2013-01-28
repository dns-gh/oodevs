// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "LoggerProxy.h"

// -----------------------------------------------------------------------------
// Name: LoggerProxy constructor
// Created: AGE 2008-05-16
// -----------------------------------------------------------------------------
LoggerProxy::LoggerProxy()
    : base_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LoggerProxy destructor
// Created: AGE 2008-05-16
// -----------------------------------------------------------------------------
LoggerProxy::~LoggerProxy()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LoggerProxy::SetLogger
// Created: AGE 2008-05-16
// -----------------------------------------------------------------------------
void LoggerProxy::SetLogger( kernel::Logger_ABC& base )
{
    base_ = &base;
}

// -----------------------------------------------------------------------------
// Name: LoggerProxy::Info
// Created: AGE 2008-05-16
// -----------------------------------------------------------------------------
void LoggerProxy::Info( const std::string& message )
{
    if( !base_ )
        throw MASA_EXCEPTION( "Invalid logger." );
    base_->Info( message);
}

// -----------------------------------------------------------------------------
// Name: LoggerProxy::Warning
// Created: AGE 2008-05-16
// -----------------------------------------------------------------------------
void LoggerProxy::Warning( const std::string& message )
{
    if( !base_ )
        throw MASA_EXCEPTION( "Invalid logger." );
    base_->Warning( message );
}

// -----------------------------------------------------------------------------
// Name: LoggerProxy::Error
// Created: AGE 2008-05-16
// -----------------------------------------------------------------------------
void LoggerProxy::Error( const std::string& message )
{
    if( !base_ )
        throw MASA_EXCEPTION( "Invalid logger." );
    base_->Error( message );
}
