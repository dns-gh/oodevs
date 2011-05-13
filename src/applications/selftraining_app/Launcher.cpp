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

// -----------------------------------------------------------------------------
// Name: Launcher constructor
// Created: SBO 2010-11-03
// -----------------------------------------------------------------------------
Launcher::Launcher( int argc, char** argv )
    : launcher_( new LauncherFacade() )
    , running_ ( true )
    , thread_  ( new boost::thread( boost::bind( &Launcher::Run, this ) ) )
{
    launcher_->Initialize( argc, argv );
}

// -----------------------------------------------------------------------------
// Name: Launcher destructor
// Created: SBO 2010-11-03
// -----------------------------------------------------------------------------
Launcher::~Launcher()
{
    if( thread_.get() )
    {
        Stop();
        thread_->join();
    }
}

// -----------------------------------------------------------------------------
// Name: Launcher::Run
// Created: SBO 2010-11-03
// -----------------------------------------------------------------------------
void Launcher::Run()
{
    if( IsInitialized() )
        while( running_ )
        {
            launcher_->Update();
            boost::this_thread::sleep( boost::posix_time::milliseconds( 25 ) ) ;
        }
}

// -----------------------------------------------------------------------------
// Name: Launcher::Stop
// Created: SBO 2010-11-03
// -----------------------------------------------------------------------------
void Launcher::Stop()
{
    boost::recursive_mutex::scoped_lock locker( mutex_ );
    running_ = false;
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
