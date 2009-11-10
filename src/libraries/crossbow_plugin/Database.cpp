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
#include "DatabaseEditor.h"
#include "QueryBuilder_ABC.h"
#include <boost/lexical_cast.hpp>

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
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Database::BuildWorkspace
// Created: JCR 2009-02-03
// -----------------------------------------------------------------------------
void Database::Initialize( IWorkspacePtr spWorkspace )
{
    workspace_ = spWorkspace;
    if( spWorkspace == 0 || FAILED( spWorkspace->QueryInterface( IID_IFeatureWorkspace, (LPVOID*)&featureWorkspace_ ) ) || featureWorkspace_ == NULL )
        throw std::runtime_error( "Cannot retrieve IFeatureWorkspace interface." );
    std::cout << "Initialize workspace." << std::endl;
    editor_.reset( new DatabaseEditor( spWorkspace ) );    
}

// -----------------------------------------------------------------------------
// Name: Database::IsValid
// Created: JCR 2009-05-27
// -----------------------------------------------------------------------------
bool Database::IsValid() const
{
    return workspace_ != 0 && featureWorkspace_ != 0;
}

// -----------------------------------------------------------------------------
// Name: Database::OpenTable
// Created: JCR 2009-02-03
// -----------------------------------------------------------------------------
Table_ABC* Database::OpenTable( const std::string& name )
{
    Table_ABC* table = OpenWrappedTable( GetTableName( name ) );
    if( !table )
        throw std::runtime_error( "Unable to open table : " + name );
    return table;
}

// -----------------------------------------------------------------------------
// Name: Database::OpenBufferedTable
// Created: JCR 2009-02-03
// -----------------------------------------------------------------------------
Table_ABC& Database::OpenBufferedTable( const std::string& name, bool clear /*= true*/ )
{
    T_TablePtr table = openedTables_[ name ];
    if( !table )
    {
        table.reset( OpenWrappedTable( GetTableName( name ) ) );
        if( table && clear )
            table->Clear();
    }
    if( !table )
        throw std::runtime_error( "Unable to open table : " + GetTableName( name ) );
    return *table;
}

// -----------------------------------------------------------------------------
// Name: Database::ClearTable
// Created: JCR 2009-02-03
// -----------------------------------------------------------------------------
void Database::ClearTable( const std::string& name )
{
    ITablePtr table;
    IQueryFilterPtr filter;    
    if( ! IsValid() || !SUCCEEDED( featureWorkspace_->OpenTable( CComBSTR( GetTableName( name ).c_str() ), &table ) ) )
        throw std::runtime_error( "Unable to open table : " + GetTableName( name ) );
    table->DeleteSearchedRows( filter );        
}

// -----------------------------------------------------------------------------
// Name: Database::OpenWrappedTable
// Created: JCR 2009-02-03
// -----------------------------------------------------------------------------
Table_ABC* Database::OpenWrappedTable( const std::string& name )
{
    IFeatureClassPtr featureClass;
    if( IsValid() && SUCCEEDED( featureWorkspace_->OpenFeatureClass( CComBSTR( name.c_str() ), &featureClass ) ) )
        return new FeatureClass( featureClass, *editor_ );
    ITablePtr table;
    if( IsValid() && SUCCEEDED( featureWorkspace_->OpenTable( CComBSTR( name.c_str() ), &table ) ) )
        return new Table( table, *editor_ );
    return 0;
}

// -----------------------------------------------------------------------------
// Name: Database::ReleaseTable
// Created: JCR 2009-02-03
// -----------------------------------------------------------------------------
void Database::ReleaseTable( const std::string& name )
{
    IT_Tables it = openedTables_.find( name );
    if ( it != openedTables_.end() && it->second )    
        openedTables_.erase( it );    
}

// -----------------------------------------------------------------------------
// Name: Database::Flush
// Created: JCR 2009-04-22
// -----------------------------------------------------------------------------
void Database::Flush()
{
    if( editor_.get() )    
        editor_->UnLock();
    openedTables_.clear();
}

// -----------------------------------------------------------------------------
// Name: Database::GetTableName
// Created: JCR 2009-04-24
// -----------------------------------------------------------------------------
std::string Database::GetTableName( const std::string& name ) const
{
    return name;
}

namespace
{
    void ThrowError()
    {
        IErrorInfoPtr ipError;
        BSTR strError;
        ::GetErrorInfo( 0, &ipError );
        ipError->GetDescription( &strError );
        const std::string error = boost::lexical_cast< std::string >( _bstr_t( strError ) );
        MT_LOG_ERROR_MSG( error ); // $$$$ SBO 2008-05-15: should throw
    }
}

// -----------------------------------------------------------------------------
// Name: Database::Execute
// Created: JCR 2009-04-24
// -----------------------------------------------------------------------------
void Database::Execute( const QueryBuilder_ABC& builder )
{
    std::string message( "Try Execute Query: " );
    if( IsValid() && builder.IsValid() )
    {
        const std::string query( builder.Create() );
        message += query;
        if( FAILED( workspace_->ExecuteSQL( CComBSTR( query.c_str() ) ) ) )
            ThrowError();
    }
	else{
		message += "Invalid query on table " + builder.GetTableName();
	}
    MT_LOG_MESSAGE_MSG( message );
}
