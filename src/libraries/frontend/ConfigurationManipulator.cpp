// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_pch.h"
#include "ConfigurationManipulator.h"
#include "tools/GeneralConfig.h"
#include <boost/filesystem.hpp>
#include <tools/XmlCrc32Signature.h>
#include <xeumeuleu/xml.hpp>

namespace bfs = boost::filesystem;

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: ConfigurationManipulator constructor
// Created: SBO 2008-02-25
// -----------------------------------------------------------------------------
ConfigurationManipulator::ConfigurationManipulator( const std::string& filename )
    : outputPath_( filename )
    , document_  ( new XmlNode( filename ) )
    , output_    ( new xml::xofstream( filename ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ConfigurationManipulator constructor
// Created: RDS 2008-08-21
// -----------------------------------------------------------------------------
ConfigurationManipulator::ConfigurationManipulator( const tools::GeneralConfig& config, const std::string& exercise, const std::string& session )
    : outputPath_( GetSessionXml( config, exercise, session ) )
    , document_  ( new XmlNode( outputPath_ ) )
    , output_    ( new xml::xofstream( outputPath_ ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ConfigurationManipulator destructor
// Created: SBO 2008-02-25
// -----------------------------------------------------------------------------
ConfigurationManipulator::~ConfigurationManipulator()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ConfigurationManipulator::GetSessionXml
// Created: AGE 2007-10-09
// -----------------------------------------------------------------------------
std::string ConfigurationManipulator::GetSessionXml( const tools::GeneralConfig& config, const std::string& exercise, const std::string& session )
{
    const bfs::path dir( config.BuildSessionDir( exercise, session ), bfs::native );
    bfs::create_directories( dir );
    return ( dir / "session.xml" ).native_file_string();
}

// -----------------------------------------------------------------------------
// Name: ConfigurationManipulator::Commit
// Created: SBO 2008-10-13
// -----------------------------------------------------------------------------
void ConfigurationManipulator::Commit()
{
    if( document_.get() && output_.get() )
        document_->Serialize( *output_ );
    tools::WriteXmlCrc32Signature( outputPath_ );
}
