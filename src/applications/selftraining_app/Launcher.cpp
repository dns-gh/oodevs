// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "Launcher.h"
#include "launcher_dll/LauncherFacade.h"
#include "tools/WaitEvent.h"

namespace
{
    std::auto_ptr< LauncherFacade > CreateFacade( int argc, char** argv )
    {
        std::auto_ptr< LauncherFacade > facade( new LauncherFacade() );
        facade->Initialize( argc, argv );
        return facade;
    }
}

// -----------------------------------------------------------------------------
// Name: Launcher constructor
// Created: SBO 2010-11-03
// -----------------------------------------------------------------------------
Launcher::Launcher( int argc, char** argv )
    : launcher_( CreateFacade( argc, argv ) )
    , quit_    ( new tools::WaitEvent() )
    , thread_  ( new boost::thread( boost::bind( &Launcher::Run, this ) ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Launcher destructor
// Created: SBO 2010-11-03
// -----------------------------------------------------------------------------
Launcher::~Launcher()
{
    quit_->Signal();
    thread_->join();
}

// -----------------------------------------------------------------------------
// Name: Launcher::Run
// Created: SBO 2010-11-03
// -----------------------------------------------------------------------------
void Launcher::Run()
{
    do
        launcher_->Update();
    while( !quit_->Wait( boost::posix_time::milliseconds( 25 ) ) );
}

// -----------------------------------------------------------------------------
// Name: Launcher::IsInitialized
// Created: SBO 2010-11-04
// -----------------------------------------------------------------------------
bool Launcher::IsInitialized() const
{
    return launcher_->IsInitialized();
}

// -----------------------------------------------------------------------------
// Name: Launcher::GetLastError
// Created: SBO 2010-11-04
// -----------------------------------------------------------------------------
std::string Launcher::GetLastError() const
{
    return launcher_->GetLastError();
}

// -----------------------------------------------------------------------------
// Name: Launcher::SetRootDir
// Created: ABR 2011-11-08
// -----------------------------------------------------------------------------
void Launcher::SetRootDir( const tools::Path& directory )
{
    launcher_->SetRootDir( directory );
}
