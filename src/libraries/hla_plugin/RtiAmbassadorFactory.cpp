// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "RtiAmbassadorFactory.h"
#include <hla/RtiAmbassador_ABC.h>
#include <xeumeuleu/xml.hpp>
#include <map>
#include <boost/assign.hpp>
#include <boost/algorithm/string.hpp>
#include <windows.h>

using namespace plugins::hla;

namespace
{
#ifdef _DEBUG
    const std::string LIBRARY = "debug";
#else
    const std::string LIBRARY = "release";
#endif

    std::string GetLastErrorMessage()
    {
        LPVOID lpMsgBuf;
        FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            GetLastError(),
            MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
            (LPTSTR) &lpMsgBuf,
            0, NULL );
        const std::string result( static_cast< char* >( lpMsgBuf ) );
        LocalFree( lpMsgBuf );
        return boost::algorithm::erase_last_copy( result, "\r\n" );
    }
}

// -----------------------------------------------------------------------------
// Name: RtiAmbassadorFactory constructor
// Created: SLI 2011-01-10
// -----------------------------------------------------------------------------
RtiAmbassadorFactory::RtiAmbassadorFactory( xml::xisubstream configuration, xml::xisubstream protocols )
{
    const std::string protocol = configuration.attribute< std::string >( "protocol", "hla-1516e" );
    std::string library;
    protocols >> xml::start( "protocols" )
                  >> xml::start( protocol )
                      >> xml::content( LIBRARY, library );
    HMODULE module = LoadLibrary( library.c_str() );
    if( !module )
        throw MASA_EXCEPTION( "failed to load protocol library: '" + library + "', reason '" + GetLastErrorMessage() + "'" );
    createAmbassador = (T_CreateAmbassador)GetProcAddress( module, "CreateAmbassador" );
    if( !createAmbassador )
        throw MASA_EXCEPTION( "unable to find function CreateAmbassador function" );
    deleteAmbassador = (T_DeleteAmbassador)GetProcAddress( module, "DeleteAmbassador" );
    if( !deleteAmbassador )
        throw MASA_EXCEPTION( "unable to find function DeleteAmbassador function" );
}

// -----------------------------------------------------------------------------
// Name: RtiAmbassadorFactory destructor
// Created: SLI 2011-01-10
// -----------------------------------------------------------------------------
RtiAmbassadorFactory::~RtiAmbassadorFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RtiAmbassadorFactory::CreateAmbassador
// Created: SLI 2011-01-10
// -----------------------------------------------------------------------------
::hla::RtiAmbassador_ABC* RtiAmbassadorFactory::CreateAmbassador( ::hla::TimeFactory_ABC& timeFactory, ::hla::TimeIntervalFactory_ABC& timeIntervalFactory,
                                                                                  ::hla::RtiAmbassador_ABC::E_MessagePolicy policy,
                                                                                  const std::string& lrcSettings ) const
{
    return createAmbassador( timeFactory, timeIntervalFactory, policy, lrcSettings );
}

// -----------------------------------------------------------------------------
// Name: DebugRtiAmbassadorFactory::DeleteAmbassador
// Created: AHC 2012-04-18
// -----------------------------------------------------------------------------
void RtiAmbassadorFactory::DeleteAmbassador( ::hla::RtiAmbassador_ABC* ambassador ) const
{
    deleteAmbassador( ambassador );
}
