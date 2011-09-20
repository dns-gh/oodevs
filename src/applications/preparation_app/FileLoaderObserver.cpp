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
#include "preparation/Tools.h"
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
        invalidSignatureFiles_.append( "\n" + bfs::path( file, bfs::native ).leaf() );
    else if( error == tools::eXmlCrc32SignatureError_NotSigned )
        missingSignatureFiles_.append( "\n" + bfs::path( file, bfs::native ).leaf() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: FileLoaderObserver::NotifyInvalidXml
// Created: NLD 2011-02-28
// -----------------------------------------------------------------------------
bool FileLoaderObserver::NotifyInvalidXml( const std::string& file, const xml::exception& )
{
    malformedFiles_.append( "\n" + bfs::path( file, bfs::native ).leaf() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: FileLoaderObserver::NotifyNoXmlSchemaSpecified
// Created: NLD 2011-02-28
// -----------------------------------------------------------------------------
void FileLoaderObserver::NotifyNoXmlSchemaSpecified( const std::string& file )
{
    missingSchemaFiles_.append( "\n" + bfs::path( file, bfs::native ).leaf() );
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
// Name: FileLoaderObserver::NotifySignatureError
// Created: NLD 2011-02-28
// -----------------------------------------------------------------------------
void FileLoaderObserver::DisplayErrors() const
{
    QSettings settings;
    settings.setPath( "MASA Group", tools::translate( "Application", "SWORD" ) );
    if( settings.readNumEntry( "/Common/NoSignatureCheck", 0 ) != 1 )
    {
        if( !invalidSignatureFiles_.empty() || !missingSignatureFiles_.empty() )
        {
            QMessageBox::warning( 0, tools::translate( "Application", "SWORD" )
                , tools::translate( "Application", "The signatures for the following files do not exist or are invalid : " ) + "\n" + invalidSignatureFiles_.c_str() + "\n" + missingSignatureFiles_.c_str() );
        }

        if( !malformedFiles_.empty() )
        {
            QMessageBox::warning( 0, tools::translate( "Application", "SWORD" )
                    , tools::translate( "Application", "The following files do not match their xsd : " ) + "\n" + malformedFiles_.c_str() );
        }
    }
    // missingSchemaFiles_ ??
}
