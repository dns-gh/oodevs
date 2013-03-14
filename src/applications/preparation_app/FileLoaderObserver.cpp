// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "FileLoaderObserver.h"
#include "clients_kernel/Tools.h"

// -----------------------------------------------------------------------------
// Name: FileLoader constructor
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
bool FileLoaderObserver::NotifyInvalidXml( const tools::Path& file, const xml::exception& e )
{
    malformedFiles_.push_back( file.FileName().ToUTF8() + " :\t" + tools::GetExceptionMsg( e ) );
    return true;
}

// -----------------------------------------------------------------------------
// Name: FileLoaderObserver::NotifyNoXmlSchemaSpecified
// Created: NLD 2011-02-28
// -----------------------------------------------------------------------------
void FileLoaderObserver::NotifyNoXmlSchemaSpecified( const tools::Path& file )
{
    missingSchemaFiles_.push_back( file.FileName().ToUTF8() );
}

// -----------------------------------------------------------------------------
// Name: FileLoaderObserver::NotifyFileMigrated
// Created: NLD 2011-02-28
// -----------------------------------------------------------------------------
void FileLoaderObserver::NotifyFileMigrated( const tools::Path& /*file*/ , const std::string& /*fromVersion*/, const std::string& /*toVersion*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FileLoaderObserver::DisplayErrors
// Created: NLD 2011-02-28
// -----------------------------------------------------------------------------
void FileLoaderObserver::DisplayErrors() const
{
    if( !malformedFiles_.empty() )
    {
        QString malformedDisplay( tools::translate( "Application", "The following files do not match their xsd : " ) + "\n" );
        for( auto it = malformedFiles_.begin(); it != malformedFiles_.end(); ++it )
        {
            malformedDisplay += "\n";
            malformedDisplay += it->c_str();
        }
        QMessageBox::warning( 0, tools::translate( "Application", "SWORD" ), malformedDisplay );
    }

    // missingSchemaFiles_ ??
}

// -----------------------------------------------------------------------------
// Name: FileLoaderObserver::GetXsdErrors
// Created: MMC 2012-06-01
// -----------------------------------------------------------------------------
void FileLoaderObserver::GetXsdErrors( std::vector< std::string >& errors ) const
{
    if( !malformedFiles_.empty() )
        for( auto it = malformedFiles_.begin(); it != malformedFiles_.end(); ++it )
            errors.push_back( tools::translate( "Application", "The following files do not match their xsd : " ).append( it->c_str() ).toAscii().constData() );
}

// -----------------------------------------------------------------------------
// Name: FileLoaderObserver::Purge
// Created: ABR 2012-07-19
// -----------------------------------------------------------------------------
void FileLoaderObserver::Purge()
{
    malformedFiles_.clear();
    missingSchemaFiles_.clear();
}
