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
// Name: ADN_FileLoaderObserver::NotifySignatureError
// Created: NLD 2011-02-28
// -----------------------------------------------------------------------------
bool ADN_FileLoaderObserver::NotifySignatureError( const std::string& fileName, const tools::EXmlCrc32SignatureError& error )
{
    std::cout << boost::format( "Signature error %d on file %s" ) % error % fileName << std::endl;
    invalidSignedFiles_ += fileName + "\n"; ///$$$ NLD 2011-03-07 - Encore une belle merde ... je me défausse : je calque le comportement idiot de la version précédente
    return true;
}

// -----------------------------------------------------------------------------
// Name: ADN_FileLoaderObserver::NotifyInvalidXml
// Created: NLD 2011-02-28
// -----------------------------------------------------------------------------
bool ADN_FileLoaderObserver::NotifyInvalidXml( const std::string& fileName, const xml::exception& e )
{
    std::cout << boost::format( "Invalid xml file %s - %s" ) % fileName % e.what() << std::endl;
    return true;
}

// -----------------------------------------------------------------------------
// Name: ADN_FileLoaderObserver::NotifyNoXmlSchemaSpecified
// Created: NLD 2011-02-28
// -----------------------------------------------------------------------------
void ADN_FileLoaderObserver::NotifyNoXmlSchemaSpecified( const std::string& fileName )
{
    std::cout << boost::format( "Xml file %s doesn't have any schema" ) % fileName << std::endl;
}

// -----------------------------------------------------------------------------
// Name: ADN_FileLoaderObserver::NotifyFileMigrated
// Created: NLD 2011-02-28
// -----------------------------------------------------------------------------
void ADN_FileLoaderObserver::NotifyFileMigrated( const std::string& /*fileName*/, const std::string& /*fromVersion*/, const std::string& /*toVersion*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_FileLoaderObserver::NotifyFileMigrated
// Created: NLD 2011-02-28
// -----------------------------------------------------------------------------
const std::string& ADN_FileLoaderObserver::GetInvalidSignedFiles() const
{
    return invalidSignedFiles_;
}
