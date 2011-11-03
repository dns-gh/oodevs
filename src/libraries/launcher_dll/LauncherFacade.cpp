// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "launcher_dll_pch.h"
#include "LauncherFacade.h"
#include "Launcher.h"
#include "Config.h"
#include <boost/lexical_cast.hpp>
#include <windows.h>

// -----------------------------------------------------------------------------
// Name: LauncherFacade constructor
// Created: SBO 2010-11-03
// -----------------------------------------------------------------------------
LauncherFacade::LauncherFacade()
    : config_  ( new launcher::Config() )
    , launcher_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LauncherFacade constructor
// Created: LGY 2011-07-29
// -----------------------------------------------------------------------------
LauncherFacade::LauncherFacade( const std::string& path )
    : launcher_( 0 )
{
    ::SetCurrentDirectory( path.c_str() );
    config_.reset( new launcher::Config() );
}

// -----------------------------------------------------------------------------
// Name: LauncherFacade destructor
// Created: SBO 2010-11-03
// -----------------------------------------------------------------------------
LauncherFacade::~LauncherFacade()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LauncherFacade::Initialize
// Created: JSR 2011-05-11
// -----------------------------------------------------------------------------
void LauncherFacade::Initialize( int argc, char** argv )
{
    try
    {
        config_->Parse( argc, argv );
        launcher_.reset( new launcher::Launcher( *config_ ) );
    }
    catch( std::exception& e )
    {
        lastError_ = e.what();
    }
}

// -----------------------------------------------------------------------------
// Name: LauncherFacade::Update
// Created: SBO 2010-11-03
// -----------------------------------------------------------------------------
bool LauncherFacade::Update()
{
    try
    {
        if( IsInitialized() )
        {
            launcher_->Update();
        }
        return true;
    }
    catch( std::exception& e )
    {
        lastError_ = e.what();
        return false;
    }
}

// -----------------------------------------------------------------------------
// Name: LauncherFacade::IsInitialized
// Created: SBO 2010-11-03
// -----------------------------------------------------------------------------
bool LauncherFacade::IsInitialized() const
{
    return launcher_.get() != 0;
}

// -----------------------------------------------------------------------------
// Name: LauncherFacade::GetLastError
// Created: SBO 2010-11-03
// -----------------------------------------------------------------------------
std::string LauncherFacade::GetLastError() const
{
    return lastError_;
}
