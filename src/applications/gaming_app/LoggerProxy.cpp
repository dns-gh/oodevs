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
LoggerProxy::LogElement LoggerProxy::Info()
{
    if( base_ )
        return base_->Info();
    throw MASA_EXCEPTION( "Invalid logger." );
}

// -----------------------------------------------------------------------------
// Name: LoggerProxy::Warning
// Created: AGE 2008-05-16
// -----------------------------------------------------------------------------
LoggerProxy::LogElement LoggerProxy::Warning()
{
    if( base_ )
        return base_->Warning();
    throw MASA_EXCEPTION( "Invalid logger." );
}

// -----------------------------------------------------------------------------
// Name: LoggerProxy::Error
// Created: AGE 2008-05-16
// -----------------------------------------------------------------------------
LoggerProxy::LogElement LoggerProxy::Error()
{
    if( base_ )
        return base_->Error();
    throw MASA_EXCEPTION( "Invalid logger." );
}

// -----------------------------------------------------------------------------
// Name: LoggerProxy::End
// Created: AGE 2008-05-16
// -----------------------------------------------------------------------------
void LoggerProxy::End( std::stringstream& output )
{
    if( base_ )
        base_->End( output );
}