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

using namespace plugins;
using namespace plugins::crossbow;


// -----------------------------------------------------------------------------
// Name: Database::Database::Database
// Created: JCR 2009-02-03
// -----------------------------------------------------------------------------
Database::Database()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Database destructor
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
Database::~Database()
{
    VARIANT_BOOL editing;
    if( SUCCEEDED( workspaceEdit_->IsBeingEdited( &editing ) ) && editing == VARIANT_TRUE )
    {
        StopEdit();
        UnLock();
    }
}

// -----------------------------------------------------------------------------
// Name: Database::BuildWorkspace
// Created: JCR 2009-02-03
// -----------------------------------------------------------------------------
void Database::Initialize( IWorkspacePtr spWorkspace )
{
    if( FAILED( spWorkspace->QueryInterface( IID_IFeatureWorkspace, (LPVOID*)&workspace_ ) ) || workspace_ == NULL )
        throw std::runtime_error( "Cannot retrieve IFeatureWorkspace interface." );    
    if( FAILED( spWorkspace->QueryInterface( IID_IWorkspaceEdit, (LPVOID*)&workspaceEdit_ ) ) || workspaceEdit_ == NULL )
        throw std::runtime_error( "Cannot retrieve IWorkspaceEdit interface." );
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
            workspaceEdit_->StartEditing( VARIANT_FALSE );
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
    try
    {
        VARIANT_BOOL editing;
        if( SUCCEEDED( workspaceEdit_->IsBeingEdited( &editing ) ) && editing == VARIANT_TRUE )
            workspaceEdit_->StopEditing( VARIANT_TRUE );
    }
    catch( std::exception& e )
    {
        MT_LOG_INFO_MSG( e.what() );
    }
}

// -----------------------------------------------------------------------------
// Name: Database::StartEdit
// Created: JCR 2007-11-21
// -----------------------------------------------------------------------------
void Database::StartEdit()
{
    workspaceEdit_->StartEditOperation();
}
    
// -----------------------------------------------------------------------------
// Name: Database::StopEdit
// Created: JCR 2007-11-21
// -----------------------------------------------------------------------------
void Database::StopEdit()
{
    workspaceEdit_->StopEditOperation();
}

// -----------------------------------------------------------------------------
// Name: Database::OpenTable
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
Table_ABC* Database::OpenTable( const std::string& name )
{
    Table_ABC* table = OpenWrappedTable( name );
    if( !table )
        throw std::runtime_error( "Unable to open table : " + name );

    // if( table && clear )
    //     table->Clear();    
    
    return table;
}

// -----------------------------------------------------------------------------
// Name: Database::OpenBufferedTable
// Created: JCR 2008-07-28
// -----------------------------------------------------------------------------
Table_ABC& Database::OpenBufferedTable( const std::string& name, bool clear /*= true*/ )
{
    Table_ABC*& table = openedTables_[name];
    if( !table )
    {
        table = OpenWrappedTable( name );
        if( table && clear )
            table->Clear();
    }
    if( !table )
        throw std::runtime_error( "Unable to open table : " + name );
    return *table;
}

// -----------------------------------------------------------------------------
// Name: Database::ClearTable
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void Database::ClearTable( const std::string& name )
{
    ITablePtr table;
    IQueryFilterPtr filter;
    if( !SUCCEEDED( workspace_->OpenTable( CComBSTR( name.c_str() ), &table ) ) )
        throw std::runtime_error( "Unable to open table : " + name );    
    table->DeleteSearchedRows( filter );        
}

// -----------------------------------------------------------------------------
// Name: Database::OpenWrappedTable
// Created: SBO 2007-09-27
// -----------------------------------------------------------------------------
Table_ABC* Database::OpenWrappedTable( const std::string& name )
{
    IFeatureClassPtr featureClass;
    if( SUCCEEDED( workspace_->OpenFeatureClass( CComBSTR( name.c_str() ), &featureClass ) ) )
        return new FeatureClass( featureClass, name );
    ITablePtr table;
    if( SUCCEEDED( workspace_->OpenTable( CComBSTR( name.c_str() ), &table ) ) )
        return new Table( table, name );    
    return 0;
}

// -----------------------------------------------------------------------------
// Name: Database::ReleaseTable
// Created: JCR 2007-11-21
// -----------------------------------------------------------------------------
void Database::ReleaseTable( const std::string& name )
{
    IT_Tables it = openedTables_.find( name );
    if ( it != openedTables_.end() && it->second )
    {
        delete it->second;
        openedTables_.erase( it );
    }
}
