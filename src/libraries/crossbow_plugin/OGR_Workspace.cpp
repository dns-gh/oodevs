// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "OGR_Workspace.h"
#include "DatabaseFactory.h"
#include "Database_ABC.h"
#include "tools/ExerciseConfig.h"
#include <xeumeuleu/xml.hpp>
#include <gdal/gdal.h>
#include <gdal/ogr_api.h>
#include <gdal/ogrsf_frmts.h>
#include <boost/filesystem/operations.hpp>

using namespace plugins;
namespace bfs = boost::filesystem;

namespace
{
    void InitializeOGR( int argc, char* argv[] )
    {
        const std::string projDriverPath( bfs::current_path().string() + "/" );
        const std::string projPath( ( bfs::current_path() / "projection_data" ).string() );

        CPLSetConfigOption( "GDAL_DRIVER_PATH", projDriverPath.c_str() );
        CPLSetConfigOption( "GDAL_DATA", projPath.c_str() );
        CPLSetConfigOption( "OGR_SDE_GETLAYERTYPE", "TRUE" );
        CPLSetConfigOption( "OGR_SDE_SEARCHORDER", "ATTRIBUTE_FIRST" );
        MT_LOG_INFO_MSG( "OGR/GDAL path: " << bfs::current_path().string() )
        OGRRegisterAll();
    }
}

// -----------------------------------------------------------------------------
// Name: OGR_Workspace constructor
// Created: JCR 2010-02-24
// -----------------------------------------------------------------------------
crossbow::OGR_Workspace::OGR_Workspace( int argc, char* argv[] )
{
    InitializeOGR( argc, argv );
}

// -----------------------------------------------------------------------------
// Name: OGR_Workspace constructor
// Created: JCR 2010-02-24
// -----------------------------------------------------------------------------
crossbow::OGR_Workspace::OGR_Workspace()
{
    int argc = 1;
    char *argv[] = { "--verbose" };
    InitializeOGR( argc, argv );
}

// -----------------------------------------------------------------------------
// Name: OGR_Workspace destructor
// Created: JCR 2010-02-24
// -----------------------------------------------------------------------------
crossbow::OGR_Workspace::~OGR_Workspace()
{
    OGRCleanupAll();
}

// -----------------------------------------------------------------------------
// Name: OGR_Workspace constructor
// Created: JCR 2010-02-24
// -----------------------------------------------------------------------------
void crossbow::OGR_Workspace::Initialize( xml::xistream& xis, const tools::ExerciseConfig& config )
{
    std::string reference;

    xis >> xml::optional
        >> xml::attribute( "default", reference );

    path_ = config.GetExerciseFile();
    if( reference.empty() )
    {
        xis >> xml::list( *this, &crossbow::OGR_Workspace::InitializeProperty );
    }
    else
    {
        InitializeConnectionReference( "geometry", reference );
        //InitializeConnectionReference( "flat", reference );
    }
}

// -----------------------------------------------------------------------------
// Name: OGR_Workspace::InitializeProperty
// Created: JCR 2010-03-23
// -----------------------------------------------------------------------------
void crossbow::OGR_Workspace::InitializeProperty( const std::string& name, xml::xistream& xis )
{
    InitializeConnectionReference( name, xml::attribute< std::string >( xis, "connection" ) );
}

// -----------------------------------------------------------------------------
// Name: OGR_Workspace::GetDatabase
// Created: JCR 2010-03-10
// -----------------------------------------------------------------------------
crossbow::Database_ABC& crossbow::OGR_Workspace::GetDatabase( const std::string& name )
{
    T_DatabasesMap::iterator it = databases_.find( name );
    if( it == databases_.end() )
        throw std::runtime_error( "database reference " + name + " not initialized" );
    if( ! it->second.get() )
        InitializeConnection( name );
    return *it->second;
}

// -----------------------------------------------------------------------------
// Name: OGR_Workspace::Release
// Created: JCR 2010-05-12
// -----------------------------------------------------------------------------
void crossbow::OGR_Workspace::Release( const std::string& name )
{
    T_Database& db = databases_[ name ];
    db.reset( ( Database_ABC* )0 ) ;
}

// -----------------------------------------------------------------------------
// Name: OGR_Workspace::InitializeConnection
// Created: JCR 2010-05-12
// -----------------------------------------------------------------------------
void crossbow::OGR_Workspace::InitializeConnection( const std::string& name )
{
    T_Database& db = databases_[ name ];

    if( !db )
    {
        const std::string& reference = connections_[ name ];
        CIT_DatabasesReferenceMap it = references_.find( reference );
        // connection or db is already referenced under an other tag
        if( it != references_.end() )
            db = databases_[ it->second ];
        if( !db )
        {
            DatabaseFactory factory;
            db = T_Database( factory.Create( tools::GeneralConfig::BuildChildPath( path_, name ), reference ) );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: crossbow::OGR_Workspace::Initialize
// Created: JCR 2008-01-11
// -----------------------------------------------------------------------------
void crossbow::OGR_Workspace::InitializeConnectionReference( const std::string& name, const std::string& reference )
{
    connections_[ name ] = reference;
    InitializeConnection( name );
    references_[ reference ] = name;
}