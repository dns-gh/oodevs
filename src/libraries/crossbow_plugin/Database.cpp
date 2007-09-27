// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "Database.h"
#include "FeatureClass.h"
#include "Table.h"
#include "dispatcher/Config.h"
#include "dispatcher/PluginConfig.h"

using namespace crossbow;

// -----------------------------------------------------------------------------
// Name: Database constructor
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
Database::Database( const dispatcher::Config& config )
{
    const std::string geodatabase = config.BuildGameChildFile( config.GetPluginConfig( "crossbow" ).GetParameter( "geodatabase" ) );

    IWorkspaceFactoryPtr spWorkspaceFactory( CLSID_AccessWorkspaceFactory );
    if( spWorkspaceFactory == NULL )
        throw std::runtime_error( "Unable to create Access workspace factory." );
    IWorkspacePtr workspace;
    if( FAILED( spWorkspaceFactory->OpenFromFile( CComBSTR( geodatabase.c_str() ), NULL, &workspace ) ) || workspace == NULL )
        throw std::runtime_error( "Unable to connect to geodatabase: " + geodatabase );
    if( FAILED( workspace->QueryInterface( IID_IFeatureWorkspace, (LPVOID*)&workspace_ ) ) || workspace_ == NULL )
        throw std::runtime_error( "Cannot retrieve IFeatureWorkspace interface." );
    if( FAILED( workspace->QueryInterface( IID_IWorkspaceEdit, (LPVOID*)&workspaceEdit_ ) ) || workspaceEdit_ == NULL )
        throw std::runtime_error( "Cannot retrieve IWorkspaceEdit interface." );
}

// -----------------------------------------------------------------------------
// Name: Database destructor
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
Database::~Database()
{
    VARIANT_BOOL editing;
    if( SUCCEEDED( workspaceEdit_->IsBeingEdited( &editing ) ) && editing == VARIANT_TRUE )
        UnLock();
}

// -----------------------------------------------------------------------------
// Name: Database::Lock
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void Database::Lock()
{
    try
    {
        VARIANT_BOOL editing;
        if( SUCCEEDED( workspaceEdit_->IsBeingEdited( &editing ) ) && editing == VARIANT_FALSE )
            if( SUCCEEDED( workspaceEdit_->StartEditing( VARIANT_FALSE ) ) )
                workspaceEdit_->StartEditOperation();
    }
    catch( std::exception& e )
    {
        MT_LOG_INFO_MSG( e.what() );
    }
}

// -----------------------------------------------------------------------------
// Name: Database::UnLock
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void Database::UnLock()
{
    workspaceEdit_->StopEditOperation();
    workspaceEdit_->StopEditing( VARIANT_TRUE );
}

// -----------------------------------------------------------------------------
// Name: Database::OpenTable
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
Table_ABC* Database::OpenTable( const std::string& name )
{
    IFeatureClassPtr featureClass;
    if( SUCCEEDED( workspace_->OpenFeatureClass( CComBSTR( name.c_str() ), &featureClass ) ) )
        return new FeatureClass( featureClass, name );
    ITablePtr table;
    if( SUCCEEDED( workspace_->OpenTable( CComBSTR( name.c_str() ), &table ) ) )
        return new Table( table, name );
    return 0;
    
}
