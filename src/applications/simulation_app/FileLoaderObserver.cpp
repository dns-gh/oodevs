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
#include <tools/Path.h>

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
// Name: FileLoaderObserver::NotifyInvalidXml
// Created: NLD 2011-02-28
// -----------------------------------------------------------------------------
bool FileLoaderObserver::NotifyInvalidXml( const tools::Path& fileName, const xml::exception& e )
{
    MT_LOG_WARNING_MSG( "Invalid XML file " << fileName.ToUTF8() << " " << tools::GetExceptionMsg( e ) );
    return true;
}

// -----------------------------------------------------------------------------
// Name: FileLoaderObserver::NotifyInvalidXml
// Created: NLD 2011-02-28
// -----------------------------------------------------------------------------
void FileLoaderObserver::NotifyNoXmlSchemaSpecified( const tools::Path& fileName )
{
    MT_LOG_WARNING_MSG( "File " << fileName.ToUTF8() << " has no XML schema specified" );
}

// -----------------------------------------------------------------------------
// Name: FileLoaderObserver::NotifyFileMigrated
// Created: NLD 2011-02-28
// -----------------------------------------------------------------------------
void FileLoaderObserver::NotifyFileMigrated( const tools::Path& /*fileName*/, const std::string& /*fromVersion*/, const std::string& /*toVersion*/ )
{
    // NOTHING
}
