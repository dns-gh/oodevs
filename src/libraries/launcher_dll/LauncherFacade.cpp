// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "Launcher_dll_pch.h"
#include "LauncherFacade.h"
#include "Config.h"
#include "Launcher.h"

// -----------------------------------------------------------------------------
// Name: LauncherFacade constructor
// Created: SBO 2010-11-03
// -----------------------------------------------------------------------------
LauncherFacade::LauncherFacade( int argc, char** argv )
    : config_( new launcher::Config() )
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
// Name: LauncherFacade destructor
// Created: SBO 2010-11-03
// -----------------------------------------------------------------------------
LauncherFacade::~LauncherFacade()
{
    // NOTHING
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
            launcher_->Update();
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
