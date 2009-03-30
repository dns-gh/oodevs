// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "FileDatabase.h"
#include "FeatureClass.h"
#include "Table.h"
#include "dispatcher/Config.h"

using namespace plugins;
using namespace plugins::crossbow;

namespace 
{
    IWorkspaceFactoryPtr GetWorkspaceFactory( const std::string& geodatabase )
    {
        IWorkspaceFactoryPtr spWorkspaceFactory;

        std::string extension( geodatabase, geodatabase.rfind( '.' ) + 1 );
        if ( extension == "mdb" )
            spWorkspaceFactory.CreateInstance( CLSID_AccessWorkspaceFactory );
        if ( extension == "gdb" )
            spWorkspaceFactory.CreateInstance( CLSID_FileGDBWorkspaceFactory );       
        return spWorkspaceFactory;
    }
}

// -----------------------------------------------------------------------------
// Name: FileDatabase constructor
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
FileDatabase::FileDatabase( const std::string& geodatabase )
{
    IWorkspaceFactoryPtr spWorkspaceFactory = GetWorkspaceFactory( geodatabase );
    if( spWorkspaceFactory == NULL )
        throw std::runtime_error( "Unable to create Access workspace factory." );
    IWorkspacePtr spWorkspace;    
    if( FAILED( spWorkspaceFactory->OpenFromFile( CComBSTR( geodatabase.c_str() ), 0, &spWorkspace ) ) || spWorkspace == NULL )
        throw std::runtime_error( "Unable to connect to geodatabase: " + geodatabase );    
    Initialize( spWorkspace );
}

// -----------------------------------------------------------------------------
// Name: FileDatabase destructor
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
FileDatabase::~FileDatabase()
{
    // NOTHING
}