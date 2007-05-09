// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "ESRI_Config.h"
#include "tools/GeneralConfig.h"
#include <xeumeuleu/xml.h>

#pragma warning( push )
#pragma warning( disable: 4127 4512 4511 )
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#pragma warning( pop )

namespace bfs = boost::filesystem;
using namespace dispatcher::esri;

namespace
{
    std::string BuildChildPath( const std::string& parent, const std::string& child )
    {
        return ( bfs::path( parent, bfs::native ).branch_path() / bfs::path( child, bfs::native ) ).native_file_string();
    }
}

// -----------------------------------------------------------------------------
// Name: ESRI_Config::Config
// Created: JCR 2007-05-02
// -----------------------------------------------------------------------------
Config::Config( const tools::GeneralConfig& config )    
{   
    xml::xifstream xis( config.GetExerciseFile() );
    std::string dataset; std::string physical; std::string terrain;
    xis >> xml::start( "exercise" )
            >> xml::start( "terrain" )
                >> xml::attribute( "name", terrain )                
            >> xml::end()
            >> xml::start( "model" )
                >> xml::attribute( "dataset", dataset )
                >> xml::attribute( "physical", physical )
            >> xml::end()
        >> xml::end();
    physical_ = config.GetPhysicalFile( dataset, physical );
    terrain_ = config.GetTerrainFile( terrain );
}
 
// -----------------------------------------------------------------------------
// Name: ESRI_Config::Config
// Created: JCR 2007-05-02
// -----------------------------------------------------------------------------
Config::Config( const Config& rhs )
    : physical_ ( rhs.physical_ )
    , terrain_ ( rhs.terrain_ )
    , geodatabase_ ( rhs.geodatabase_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ESRI_Config::~Config
// Created: JCR 2007-05-02
// -----------------------------------------------------------------------------
Config::~Config()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ESRI_Config::UpdateGeodatabase
// Created: JCR 2007-05-02
// -----------------------------------------------------------------------------
void Config::UpdateGeodatabase( const std::string& geodatabase )
{
    geodatabase_ = geodatabase;
}

// -----------------------------------------------------------------------------
// Name: ESRI_Config::GetPhysicalFile
// Created: JCR 2007-05-02
// -----------------------------------------------------------------------------
const std::string& Config::GetPhysicalFile() const
{
    return physical_;
}

// -----------------------------------------------------------------------------
// Name: ESRI_Config::BuildPhysicalChildFile
// Created: JCR 2007-05-03
// -----------------------------------------------------------------------------
std::string Config::BuildPhysicalChildFile( const std::string& file ) const
{
    return BuildChildPath( GetPhysicalFile(), file );
}

// -----------------------------------------------------------------------------
// Name: ESRI_Config::GetGeodatabaseFile
// Created: JCR 2007-05-02
// -----------------------------------------------------------------------------
const std::string& Config::GetGeodatabaseFile() const
{
    return geodatabase_;
}

// -----------------------------------------------------------------------------
// Name: ESRI_Config::GetTerrainFile
// Created: JCR 2007-05-03
// -----------------------------------------------------------------------------
const std::string& Config::GetTerrainFile() const
{
    return terrain_;
}

// -----------------------------------------------------------------------------
// Name: ESRI_Config::BuildTerrainChildFile
// Created: JCR 2007-05-03
// -----------------------------------------------------------------------------
std::string Config::BuildTerrainChildFile( const std::string& file ) const
{
    return BuildChildPath( GetTerrainFile(), file );
}

