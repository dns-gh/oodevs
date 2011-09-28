// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "wise_driver_dll_pch.h"
#include "SwordFacade.h"
#include "Model.h"
#include "client_proxy/SwordProxy.h"
#include <sstream>
#pragma warning( push )
#pragma warning( disable: 4100 4201 )
#include <wise/iwisedriversettings.h>
#include <wise/wisedriver.h>
#pragma warning( pop )

namespace
{
    std::string ToString( const std::wstring& wideString )
    {
        return std::string( wideString.begin(), wideString.end() );
    }
    std::wstring ToWideString( const std::string& narrowString )
    {
        return std::wstring( narrowString.begin(), narrowString.end() );
    }

    SwordProxy* CreateProxy( IWISEDriverSettings& pSettings )
    {
        std::wstring host, profile, password;
        long port;
        CHECK_WISE_RESULT_EX( pSettings.GetSetting( L"SimulationHost", host ) );
        CHECK_WISE_RESULT_EX( pSettings.GetSetting( L"SimulationPort", port ) );
        CHECK_WISE_RESULT_EX( pSettings.GetSetting( L"ExerciseProfile", profile ) );
        CHECK_WISE_RESULT_EX( pSettings.GetSetting( L"ExercisePassword", password ) );
        return new SwordProxy( ToString( host ), unsigned short( port ), ToString( profile ), ToString( password ) );
    }
}

// -----------------------------------------------------------------------------
// Name: SwordFacade constructor
// Created: SEB 2010-10-12
// -----------------------------------------------------------------------------
SwordFacade::SwordFacade( IWISEDriverSettings& pSettings, CWISEDriver& driver )
    : driver_( driver )
    , proxy_( CreateProxy( pSettings ) )
    , disconnect_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SwordFacade destructor
// Created: SEB 2010-10-12
// -----------------------------------------------------------------------------
SwordFacade::~SwordFacade()
{
    if( model_.get() )
        proxy_->UnregisterMessageHandler( model_.get() );
}

// -----------------------------------------------------------------------------
// Name: SwordFacade::Connect
// Created: SEB 2010-10-12
// -----------------------------------------------------------------------------
void SwordFacade::Connect( const WISE_HANDLE& database )
{
    if( model_.get() )
        proxy_->RegisterMessageHandler( model_.get() );
    model_.reset( new Model( driver_, database, *proxy_ ) );
    proxy_->RegisterMessageHandler( model_.get() );
    proxy_->Connect( this );
}

// -----------------------------------------------------------------------------
// Name: SwordFacade::Disconnect
// Created: SEB 2010-10-12
// -----------------------------------------------------------------------------
void SwordFacade::Disconnect()
{
    disconnect_ = true;
    proxy_->Disconnect();
    model_.reset();
}

// -----------------------------------------------------------------------------
// Name: SwordFacade::OnRun
// Created: SEB 2010-10-12
// -----------------------------------------------------------------------------
void SwordFacade::OnRun()
{
    proxy_->Update();
}

// -----------------------------------------------------------------------------
// Name: SwordFacade::OnEventReceived
// Created: SEB 2010-10-14
// -----------------------------------------------------------------------------
void SwordFacade::OnEventReceived( const WISE_HANDLE& handle )
{
    if( model_.get() )
        model_->OnReceiveEvent( handle );
}

// -----------------------------------------------------------------------------
// Name: SwordFacade::OnUpdateReceived
// Created: SEB 2010-12-13
// -----------------------------------------------------------------------------
void SwordFacade::OnUpdateReceived( const WISE_HANDLE& object, const WISE_HANDLE& attribute, const CWISEValueUnion& value )
{
    if( model_.get() )
        model_->OnReceiveUpdate( object, attribute, value );
}

// -----------------------------------------------------------------------------
// Name: SwordFacade::OnConnectionSucceeded
// Created: SEB 2010-10-12
// -----------------------------------------------------------------------------
void SwordFacade::OnConnectionSucceeded( const std::string& endpoint )
{
    std::wstringstream message;
    message << L"Connected to simulation '" << ToWideString( endpoint ) << L"'";
    driver_.NotifyInfoMessage( message.str() );
}

// -----------------------------------------------------------------------------
// Name: SwordFacade::OnConnectionFailed
// Created: SEB 2010-10-12
// -----------------------------------------------------------------------------
void SwordFacade::OnConnectionFailed( const std::string& endpoint, const std::string& reason )
{
    std::wstringstream message;
    message << L"Connection to simulation '" << ToWideString( endpoint ) << L"' failed. Reason: " << ToWideString( reason );
    driver_.NotifyErrorMessage( message.str(), MAKE_WISE_RESULT( WISE_FACILITY_COM_ADAPTER, WISE_OPEN_FAILED ) );
}

// -----------------------------------------------------------------------------
// Name: SwordFacade::OnConnectionError
// Created: SEB 2010-10-13
// -----------------------------------------------------------------------------
void SwordFacade::OnConnectionError( const std::string& endpoint, const std::string& reason )
{
    std::wstringstream message;
    message << L"Connection to simulation '" << ToWideString( endpoint ) << L"' lost. Reason: " << ToWideString( reason );
    if( disconnect_ )
        driver_.NotifyInfoMessage( message.str() );
    else
        driver_.NotifyErrorMessage( message.str(), MAKE_WISE_RESULT( WISE_FACILITY_COM_ADAPTER, WISE_OPEN_FAILED ) );
}

// -----------------------------------------------------------------------------
// Name: SwordFacade::OnConnectionWarning
// Created: MCO 2011-09-28
// -----------------------------------------------------------------------------
void SwordFacade::OnConnectionWarning( const std::string& endpoint, const std::string& reason )
{
    std::wstringstream message;
    message << L"Connection to simulation '" << ToWideString( endpoint ) << L"' warning. Reason: " << ToWideString( reason );
    if( disconnect_ )
        driver_.NotifyInfoMessage( message.str() );
    else
        driver_.NotifyErrorMessage( message.str(), MAKE_WISE_RESULT( WISE_FACILITY_COM_ADAPTER, WISE_OPEN_FAILED ) );
}

// -----------------------------------------------------------------------------
// Name: SwordFacade::OnAuthenticationSucceeded
// Created: SEB 2010-10-12
// -----------------------------------------------------------------------------
void SwordFacade::OnAuthenticationSucceeded( const std::string& profile )
{
    std::wstringstream message;
    message << L"Authenticated using profile '" << ToWideString( profile ) << L"'";
    driver_.NotifyInfoMessage( message.str() );
}

// -----------------------------------------------------------------------------
// Name: SwordFacade::OnAuthenticationFailed
// Created: SEB 2010-10-12
// -----------------------------------------------------------------------------
void SwordFacade::OnAuthenticationFailed( const std::string& profile, const std::string& reason )
{
    std::wstringstream message;
    message << L"Authentication failed using profile '" << ToWideString( profile ) << L"'. Reason: " << ToWideString( reason );
    driver_.NotifyErrorMessage( message.str(), MAKE_WISE_RESULT( WISE_FACILITY_COM_ADAPTER, WISE_ERROR ) );
}
