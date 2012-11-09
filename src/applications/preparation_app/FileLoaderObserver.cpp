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
}

// -----------------------------------------------------------------------------
// Name: FileLoaderObserver destructor
// Created: NLD 2011-02-28
// -----------------------------------------------------------------------------
FileLoaderObserver::~FileLoaderObserver()
{
}

// -----------------------------------------------------------------------------
// Name: FileLoaderObserver::NotifySignatureError
// Created: NLD 2011-02-28
// -----------------------------------------------------------------------------
bool FileLoaderObserver::NotifySignatureError( const std::string& file, const tools::EXmlCrc32SignatureError& error )
{
    if( error == tools::eXmlCrc32SignatureError_Invalid )
        invalidSignatureFiles_.push_back( bfs::path( file ).filename().string() );
    else if( error == tools::eXmlCrc32SignatureError_NotSigned )
        missingSignatureFiles_.push_back( bfs::path( file ).filename().string() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: FileLoaderObserver::NotifyInvalidXml
// Created: NLD 2011-02-28
// -----------------------------------------------------------------------------
bool FileLoaderObserver::NotifyInvalidXml( const std::string& file, const xml::exception& e )
{
    malformedFiles_.push_back( bfs::path( file ).filename().string() + " :\t" + e.what() );
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
        for( CIT_Error it = invalidSignatureFiles_.begin(); it != invalidSignatureFiles_.end(); ++it )
        {
            invalidSignatureDisplay += "\n";
            invalidSignatureDisplay += it->c_str();
        }
        for( CIT_Error it = missingSignatureFiles_.begin(); it != missingSignatureFiles_.end(); ++it )
        {
            invalidSignatureDisplay += "\n";
            invalidSignatureDisplay += it->c_str();
        }
        QMessageBox::warning( 0, tools::translate( "Application", "SWORD" ), invalidSignatureDisplay );
    }

    if( !malformedFiles_.empty() )
    {
        QString malformedDisplay( tools::translate( "Application", "The following files do not match their xsd : " ) + "\n" );
        for( CIT_Error it = malformedFiles_.begin(); it != malformedFiles_.end(); ++it )
        {
            malformedDisplay += "\n";
            malformedDisplay += it->c_str();
        }
        QMessageBox::warning( 0, tools::translate( "Application", "SWORD" ), malformedDisplay );
    }

    // missingSchemaFiles_ ??
}

// -----------------------------------------------------------------------------
// Name: FileLoaderObserver::GetErrors
// Created: MMC 2012-06-01
// -----------------------------------------------------------------------------
void FileLoaderObserver::GetErrors( std::vector< std::string >& errors ) const
{
    if( !invalidSignatureFiles_.empty() || !missingSignatureFiles_.empty() )
    {
        QString invalidSignatureDisplay( tools::translate( "Application", "The signatures for the following files do not exist or are invalid : " ) + "\n" );
        for( CIT_Error it = invalidSignatureFiles_.begin(); it != invalidSignatureFiles_.end(); ++it )
            errors.push_back( tools::translate( "Application", "The signatures for the following files do not exist or are invalid : " ).append( it->c_str() ).toStdString() );
        for( CIT_Error it = missingSignatureFiles_.begin(); it != missingSignatureFiles_.end(); ++it )
            errors.push_back( tools::translate( "Application", "The signatures for the following files do not exist or are invalid : " ).append( it->c_str() ).toStdString() );
    }

    if( !malformedFiles_.empty() )
        for( CIT_Error it = malformedFiles_.begin(); it != malformedFiles_.end(); ++it )
            errors.push_back( tools::translate( "Application", "The following files do not match their xsd : " ).append( it->c_str() ).toStdString() );
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
