// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "tools_pch.h"
#include "NullFileLoaderObserver.h"
#include <xeumeuleu/xml.hpp>
#include <boost/format.hpp>
#include <iostream>

using namespace tools;

// -----------------------------------------------------------------------------
// Name: NullFileLoaderObserver constructor
// Created: NLD 2011-02-28
// -----------------------------------------------------------------------------
NullFileLoaderObserver::NullFileLoaderObserver()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NullFileLoaderObserver destructor
// Created: NLD 2011-02-28
// -----------------------------------------------------------------------------
NullFileLoaderObserver::~NullFileLoaderObserver()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NullFileLoaderObserver::NotifySignatureError
// Created: NLD 2011-02-28
// -----------------------------------------------------------------------------
bool NullFileLoaderObserver::NotifySignatureError( const std::string& fileName, const tools::EXmlCrc32SignatureError& error )
{
    std::cout << boost::format( "Signature error %d on file %s" ) % error % fileName << std::endl;
    return true;
}

// -----------------------------------------------------------------------------
// Name: NullFileLoaderObserver::NotifyInvalidXml
// Created: NLD 2011-02-28
// -----------------------------------------------------------------------------
bool NullFileLoaderObserver::NotifyInvalidXml( const std::string& fileName, const xml::exception& e )
{
    std::cout << boost::format( "Invalid xml file %s - %s" ) % fileName % e.what() << std::endl;
    return true;
}

// -----------------------------------------------------------------------------
// Name: NullFileLoaderObserver::NotifyNoXmlSchemaSpecified
// Created: NLD 2011-02-28
// -----------------------------------------------------------------------------
void NullFileLoaderObserver::NotifyNoXmlSchemaSpecified( const std::string& fileName )
{
    std::cout << boost::format( "Xml file %s doesn't have any schema" ) % fileName << std::endl;
}


// -----------------------------------------------------------------------------
// Name: NullFileLoaderObserver::NotifyFileMigrated
// Created: NLD 2011-02-28
// -----------------------------------------------------------------------------
void NullFileLoaderObserver::NotifyFileMigrated( const std::string& /*fileName*/, const std::string& /*fromVersion*/, const std::string& /*toVersion*/ )
{
    // NOTHING
}
