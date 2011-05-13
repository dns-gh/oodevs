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
#include "shield/Server.h"
#include "shield/Listener_ABC.h"
#include <boost/lexical_cast.hpp>

namespace
{
    class NullLogger : public shield::Listener_ABC
    {
        virtual void Info( const std::string& /*message*/ )
        {
            // NOTHING
        }
        virtual void Error( const std::string& /*message*/ )
        {
            // NOTHING
        }
    };
}

// -----------------------------------------------------------------------------
// Name: LauncherFacade constructor
// Created: SBO 2010-11-03
// -----------------------------------------------------------------------------
LauncherFacade::LauncherFacade()
    : config_  ( new launcher::Config() )
    , launcher_( 0 )
    , proxy_   ( 0 )
{
    // NOTHING
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
        unsigned short port = config_->GetLauncherPort();
        static NullLogger logger;
        proxy_.reset( new shield::Server( port + 1, "localhost:" + boost::lexical_cast< std::string >( port ), logger ) ); // $$$$ MCO should we hard-code 30001 instead of port + 1 ?
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
            proxy_->Update();
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
    return launcher_.get() && proxy_.get();
}

// -----------------------------------------------------------------------------
// Name: LauncherFacade::GetLastError
// Created: SBO 2010-11-03
// -----------------------------------------------------------------------------
std::string LauncherFacade::GetLastError() const
{
    return lastError_;
}
