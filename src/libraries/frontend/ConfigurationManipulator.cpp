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
#include "tools/SchemaWriter.h"
#include <xeumeuleu/xml.hpp>

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: ConfigurationManipulator constructor
// Created: SBO 2008-02-25
// -----------------------------------------------------------------------------
ConfigurationManipulator::ConfigurationManipulator( const tools::Path& filename )
    : outputPath_( filename )
    , document_  ( new XmlNode( outputPath_ ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ConfigurationManipulator constructor
// Created: RDS 2008-08-21
// -----------------------------------------------------------------------------
ConfigurationManipulator::ConfigurationManipulator( const tools::GeneralConfig& config, const tools::Path& exercise, const tools::Path& session )
    : outputPath_( GetSessionXml( config, exercise, session ) )
    , document_  ( new XmlNode( outputPath_ ) )
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
tools::Path ConfigurationManipulator::GetSessionXml( const tools::GeneralConfig& config, const tools::Path& exercise, const tools::Path& session )
{
    const tools::Path dir = config.BuildSessionDir( exercise, session );
    dir.CreateDirectories();
    return dir / "session.xml";
}

// -----------------------------------------------------------------------------
// Name: ConfigurationManipulator::Commit
// Created: SBO 2008-10-13
// -----------------------------------------------------------------------------
void ConfigurationManipulator::Commit()
{
    if( document_.get() )
    {
        bool ok = false;
        unsigned int nbError = 0;
        while( !ok )
        {
            try
            {
                {
                    tools::Xofstream xos( outputPath_ );
                    document_->Serialize( xos );
                }
                ok = true;
            }
            catch( const std::exception& e )
            {
                if( ++nbError == 42 )   // avoid endless loop
                    throw e;
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ConfigurationManipulator::RemoveNode
// Created: RPD 2011-03-29
// -----------------------------------------------------------------------------
bool ConfigurationManipulator::RemoveNode( const std::string& path )
{
    return document_->RemoveNode( path );
}

// -----------------------------------------------------------------------------
// Name: ConfigurationManipulator::HasNode
// Created: RPD 2011-03-30
// -----------------------------------------------------------------------------
bool ConfigurationManipulator::HasNode( const std::string& path )
{
    return document_->HasNode( path );
}

const tools::Path& ConfigurationManipulator::GetPath() const
{
    return outputPath_;
}
