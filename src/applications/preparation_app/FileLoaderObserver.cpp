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
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

namespace bfs = boost::filesystem;

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
bool FileLoaderObserver::NotifyInvalidXml( const std::string& file, const xml::exception& e )
{
    malformedFiles_.push_back( bfs::path( file ).filename().string() + " :\t" + tools::GetExceptionMsg( e ) );
    return true;
}

// -----------------------------------------------------------------------------
// Name: FileLoaderObserver::NotifyNoXmlSchemaSpecified
// Created: NLD 2011-02-28
// -----------------------------------------------------------------------------
void FileLoaderObserver::NotifyNoXmlSchemaSpecified( const std::string& file )
{
    missingSchemaFiles_.push_back( bfs::path( file ).filename().string() );
}

// -----------------------------------------------------------------------------
// Name: FileLoaderObserver::NotifyFileMigrated
// Created: NLD 2011-02-28
// -----------------------------------------------------------------------------
void FileLoaderObserver::NotifyFileMigrated( const std::string& /*file*/ , const std::string& /*fromVersion*/, const std::string& /*toVersion*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FileLoaderObserver::DisplayErrors
// Created: NLD 2011-02-28
// -----------------------------------------------------------------------------
void FileLoaderObserver::DisplayErrors() const
{
    if( !invalidSignatureFiles_.empty() || !missingSignatureFiles_.empty() )
    {
        QString invalidSignatureDisplay( tools::translate( "Application", "The signatures for the following files do not exist or are invalid : " ) + "\n" );
        for( auto it = invalidSignatureFiles_.begin(); it != invalidSignatureFiles_.end(); ++it )
        {
            invalidSignatureDisplay += "\n";
            invalidSignatureDisplay += it->c_str();
        }
        for( auto it = missingSignatureFiles_.begin(); it != missingSignatureFiles_.end(); ++it )
        {
            invalidSignatureDisplay += "\n";
            invalidSignatureDisplay += it->c_str();
        }
        QMessageBox::warning( 0, tools::translate( "Application", "SWORD" ), invalidSignatureDisplay );
    }

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
    invalidSignatureFiles_.clear();
    missingSignatureFiles_.clear();
    malformedFiles_.clear();
    missingSchemaFiles_.clear();
}
