// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "esri_plugin_pch.h"
#include "DatabaseFactory.h"
#include "dispatcher/Config.h"
#include "FileDatabase.h"
#include "ConnectionDatabase.h"

using namespace plugins;
using namespace plugins::esri;

// -----------------------------------------------------------------------------
// Name: DatabaseFactory constructor
// Created: JCR 2009-02-10
// -----------------------------------------------------------------------------
DatabaseFactory::DatabaseFactory()
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: DatabaseFactory destructor
// Created: JCR 2009-02-10
// -----------------------------------------------------------------------------
DatabaseFactory::~DatabaseFactory()
{
    // NOTHING
}


namespace 
{
    bool IsFileDatabase( const std::string& url )
    {
        std::string extension( url, url.rfind( '.' ) + 1 );
        return extension == "mdb" || extension == "gdb";
    }

    bool IsConnectionDatabase( const std::string& url )
    {
        // boost::regexp_match( )
        std::string head( url, 0, min( url.size(), 6 ) );
        return head == "sde://";
    }
}

// -----------------------------------------------------------------------------
// Name: DatabaseFactory::Create
// Created: JCR 2009-02-10
// -----------------------------------------------------------------------------
Database_ABC& DatabaseFactory::Create( const dispatcher::Config& config, const std::string& name )
{
    if ( IsFileDatabase( name ) )
        return *new FileDatabase( config.BuildExerciseChildFile( name ) );    
    if ( IsConnectionDatabase( name ) )
        return *new ConnectionDatabase( name );
    throw std::runtime_error( "Unknown geodatabase connection properties: " + name );
}
