// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_FileLoaderObserver.h"
#include <boost/format.hpp>
#include <iostream>

using namespace tools;

// -----------------------------------------------------------------------------
// Name: ADN_FileLoaderObserver constructor
// Created: NLD 2011-02-28
// -----------------------------------------------------------------------------
ADN_FileLoaderObserver::ADN_FileLoaderObserver()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_FileLoaderObserver destructor
// Created: NLD 2011-02-28
// -----------------------------------------------------------------------------
ADN_FileLoaderObserver::~ADN_FileLoaderObserver()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_FileLoaderObserver::NotifyInvalidXml
// Created: NLD 2011-02-28
// -----------------------------------------------------------------------------
bool ADN_FileLoaderObserver::NotifyInvalidXml( const tools::Path& fileName, const xml::exception& e )
{
    MT_LOG_ERROR_MSG( boost::format( "Invalid xml file %s - %s" ) % fileName.ToUTF8() % tools::GetExceptionMsg( e ) << std::endl );
    return true;
}

// -----------------------------------------------------------------------------
// Name: ADN_FileLoaderObserver::NotifyNoXmlSchemaSpecified
// Created: NLD 2011-02-28
// -----------------------------------------------------------------------------
void ADN_FileLoaderObserver::NotifyNoXmlSchemaSpecified( const tools::Path& fileName )
{
    MT_LOG_ERROR_MSG( boost::format( "Xml file %s doesn't have any schema" ) % fileName.ToUTF8() << std::endl );
}

// -----------------------------------------------------------------------------
// Name: ADN_FileLoaderObserver::NotifyFileMigrated
// Created: NLD 2011-02-28
// -----------------------------------------------------------------------------
void ADN_FileLoaderObserver::NotifyFileMigrated( const tools::Path& /*fileName*/, const std::string& /*fromVersion*/, const std::string& /*toVersion*/ )
{
    // NOTHING
}
