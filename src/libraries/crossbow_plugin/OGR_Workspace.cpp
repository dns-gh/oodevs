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

#include <gdal/gdal.h>
#include <gdal/ogrsf_frmts.h>

using namespace plugins;

namespace
{
    void InitializeOGR( int argc, char* argv[] )
    {
        if( argc > 0 && GDALGeneralCmdLineProcessor( argc, &argv, 0 ) < 1 )
            throw std::runtime_error( "error while processing gdal commands." );
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
    int argc = 0;
    char *argv[] = { "crossbow_test_CrossbowConnection" };
    InitializeOGR( argc, argv );
}

// -----------------------------------------------------------------------------
// Name: OGR_Workspace destructor
// Created: JCR 2010-02-24
// -----------------------------------------------------------------------------
crossbow::OGR_Workspace::~OGR_Workspace()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OGR_Workspace constructor
// Created: JCR 2010-02-24
// -----------------------------------------------------------------------------
void crossbow::OGR_Workspace::Initialize( xml::xistream& xis, const dispatcher::Config& config )
{
    std::string reference;

    xis >> xml::optional
        >> xml::attribute( "default", reference );

    if( reference.empty() )
    {
        xis >> xml::list( "property", *this, &crossbow::OGR_Workspace::InitializeProperty, config );
    }
    else
    {
        InitializeConnectionReference( "geometry", config, reference );
        InitializeConnectionReference( "flat", config, reference );
    }
}

// -----------------------------------------------------------------------------
// Name: OGR_Workspace::InitializeProperty
// Created: JCR 2010-03-23
// -----------------------------------------------------------------------------
void crossbow::OGR_Workspace::InitializeProperty( xml::xistream& xis, const dispatcher::Config& config )
{
    InitializeConnectionReference( xis.attribute< std::string >( "name" ), config, xis.attribute< std::string >( "connection" ) );
}

// -----------------------------------------------------------------------------
// Name: OGR_Workspace::GetDatabase
// Created: JCR 2010-03-10
// -----------------------------------------------------------------------------
crossbow::Database_ABC& crossbow::OGR_Workspace::GetDatabase( const std::string& name )
{
    T_DatabasesMap::iterator it = databases_.find( name );
    if( ! it->second )
        throw std::runtime_error( "database reference " + name + " not initialized" );
    return *it->second;
}

// -----------------------------------------------------------------------------
// Name: crossbow::OGR_Workspace::Initialize
// Created: JCR 2008-01-11
// -----------------------------------------------------------------------------
void crossbow::OGR_Workspace::InitializeConnectionReference( const std::string& name, const dispatcher::Config& config, const std::string& reference )
{
    T_Database& db = databases_[ name ];

    if( !db )
    {
        CIT_DatabasesReferenceMap it = references_.find( reference );
        // connection or db is already referenced under an other tag
        if( it != references_.end() )
            db = databases_[ it->second ];
        else
        {
            DatabaseFactory factory;
            db = T_Database( factory.Create( config, reference ) );
            references_[ reference ] = name;
        }
    }
}

