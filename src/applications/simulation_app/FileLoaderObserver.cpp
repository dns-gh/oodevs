// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_app_pch.h"

#include "FileLoaderObserver.h"
#include "MT_Tools/MT_Logger.h"
#include "xeumeuleu/xml.hpp"

// -----------------------------------------------------------------------------
// Name: FileLoaderObserver constructor
// Created: NLD 2011-02-28
// -----------------------------------------------------------------------------
FileLoaderObserver::FileLoaderObserver()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FileLoaderObserver destructor
// Created: NLD 2011-02-28
// -----------------------------------------------------------------------------
FileLoaderObserver::~FileLoaderObserver()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FileLoaderObserver::NotifySignatureError
// Created: NLD 2011-02-28
// -----------------------------------------------------------------------------
bool FileLoaderObserver::NotifySignatureError( const std::string& fileName, const tools::EXmlCrc32SignatureError& error )
{
    if( error == tools::eXmlCrc32SignatureError_Invalid )
        MT_LOG_WARNING_MSG( "The signature for the file " << fileName << " is invalid." )
    else if( error == tools::eXmlCrc32SignatureError_NotSigned )
        MT_LOG_WARNING_MSG( "The file " << fileName << " is not signed." )
    return true;
}

// -----------------------------------------------------------------------------
// Name: FileLoaderObserver::NotifyInvalidXml
// Created: NLD 2011-02-28
// -----------------------------------------------------------------------------
bool FileLoaderObserver::NotifyInvalidXml( const std::string& fileName, const xml::exception& e )
{
    MT_LOG_WARNING_MSG( "Invalid XML file " << fileName << " " << e.what() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: FileLoaderObserver::NotifyInvalidXml
// Created: NLD 2011-02-28
// -----------------------------------------------------------------------------
void FileLoaderObserver::NotifyNoXmlSchemaSpecified( const std::string& fileName )
{
    MT_LOG_WARNING_MSG( "File " << fileName << " has no XML schema specified" );
}

// -----------------------------------------------------------------------------
// Name: FileLoaderObserver::NotifyFileMigrated
// Created: NLD 2011-02-28
// -----------------------------------------------------------------------------
void FileLoaderObserver::NotifyFileMigrated( const std::string& /*fileName*/, const std::string& /*fromVersion*/, const std::string& /*toVersion*/ )
{
    // NOTHING
}
