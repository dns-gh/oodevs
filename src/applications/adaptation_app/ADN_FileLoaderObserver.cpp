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
    MT_LOG_ERROR_MSG( "Invalid xml file " << fileName << " - " << tools::GetExceptionMsg( e ) );
    return true;
}

// -----------------------------------------------------------------------------
// Name: ADN_FileLoaderObserver::NotifyNoXmlSchemaSpecified
// Created: NLD 2011-02-28
// -----------------------------------------------------------------------------
void ADN_FileLoaderObserver::NotifyNoXmlSchemaSpecified( const tools::Path& fileName )
{
    MT_LOG_ERROR_MSG( "Xml file " << fileName << " doesn't have any schema" );
}

// -----------------------------------------------------------------------------
// Name: ADN_FileLoaderObserver::NotifyFileMigrated
// Created: NLD 2011-02-28
// -----------------------------------------------------------------------------
void ADN_FileLoaderObserver::NotifyFileMigrated( const tools::Path& /*fileName*/, const std::string& /*fromVersion*/, const std::string& /*toVersion*/ )
{
    // NOTHING
}
