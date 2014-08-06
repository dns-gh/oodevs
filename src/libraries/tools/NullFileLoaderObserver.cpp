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
#include "MT_Tools/MT_Logger.h"
#include <tools/Path.h>
#include <xeumeuleu/xml.hpp>
#include <boost/make_shared.hpp>

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
// Name: NullFileLoaderObserver::NotifyInvalidXml
// Created: NLD 2011-02-28
// -----------------------------------------------------------------------------
bool NullFileLoaderObserver::NotifyInvalidXml( const Path& fileName, const xml::exception& e )
{
    MT_LOG_ERROR_MSG( "Invalid xml file " << fileName << " - " << tools::GetExceptionMsg( e ) );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NullFileLoaderObserver::NotifyNoXmlSchemaSpecified
// Created: NLD 2011-02-28
// -----------------------------------------------------------------------------
void NullFileLoaderObserver::NotifyNoXmlSchemaSpecified( const Path& fileName )
{
    MT_LOG_WARNING_MSG( "Xml file " << fileName << " doesn't have any schema " );
}

// -----------------------------------------------------------------------------
// Name: NullFileLoaderObserver::NotifyFileMigrated
// Created: NLD 2011-02-28
// -----------------------------------------------------------------------------
void NullFileLoaderObserver::NotifyFileMigrated( const Path& /*fileName*/, const std::string& /*fromVersion*/, const std::string& /*toVersion*/ )
{
    // NOTHING
}

boost::shared_ptr< NullFileLoaderObserver > tools::CreateNullFileLoaderObserver()
{
    return boost::make_shared< NullFileLoaderObserver >();
}
