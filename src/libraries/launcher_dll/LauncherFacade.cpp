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
#include "tools/Codec.h"
#include <boost/lexical_cast.hpp>
#include <windows.h>

// -----------------------------------------------------------------------------
// Name: LauncherFacade constructor
// Created: SBO 2010-11-03
// -----------------------------------------------------------------------------
LauncherFacade::LauncherFacade()
    : config_( 0 )
    , launcher_( 0 )
{
    tools::SetCodec();
    config_.reset( new launcher::Config() );
}

// -----------------------------------------------------------------------------
// Name: LauncherFacade constructor
// Created: LGY 2011-07-29
// -----------------------------------------------------------------------------
LauncherFacade::LauncherFacade( const tools::Path& path )
    : launcher_( 0 )
{
    tools::SetCodec();
    ::SetCurrentDirectoryW( path.ToUnicode().c_str() );
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
    catch( const std::exception& e )
    {
        lastError_ = tools::GetExceptionMsg( e );
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
    catch( const std::exception& e )
    {
        lastError_ = tools::GetExceptionMsg( e );
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

// -----------------------------------------------------------------------------
// Name: LauncherFacade::SetRootDir
// Created: ABR 2011-11-08
// -----------------------------------------------------------------------------
void LauncherFacade::SetRootDir( const tools::Path& directory )
{
    if( IsInitialized() )
        config_->SetRootDir( directory );
}
