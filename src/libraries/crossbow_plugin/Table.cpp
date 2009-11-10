// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "Table.h"
#include "Row.h"
#include "DatabaseEditor_ABC.h"

#include <boost/lexical_cast.hpp>

using namespace plugins;

// -----------------------------------------------------------------------------
// Name: Table constructor
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
crossbow::Table::Table( ITablePtr table, DatabaseEditor_ABC& editor )
    : table_ ( table )
    , cursor_ ()
    , row_ ( new Row() )
    , editor_ ( editor )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Table destructor
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
crossbow::Table::~Table()
{
    // NOTHING
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
        MT_LOG_ERROR_MSG( error ); // $$$$ SBO 2007-08-24: should throw
    }
}

// -----------------------------------------------------------------------------
// Name: Table::BeginTransaction
// Created: JCR 2009-04-22
// -----------------------------------------------------------------------------
void crossbow::Table::BeginTransaction()
{
    editor_.StartEdit();
}
    
// -----------------------------------------------------------------------------
// Name: Table::EndTransaction
// Created: JCR 2009-04-22
// -----------------------------------------------------------------------------
void crossbow::Table::EndTransaction()
{
    editor_.StopEdit();
    // check efficiency editor_.EndTransaction();
}

// -----------------------------------------------------------------------------
// Name: Table::CreateRow
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
crossbow::Row_ABC& crossbow::Table::CreateRow()
{
    IRowBufferPtr row;

    editor_.StartEdit();
    HRESULT res = table_->CreateRowBuffer( &row ); // $$$$ SBO 2007-08-30: check
    if( FAILED( res ) )
        ThrowError();
    long ids = 0;
    IQueryFilterPtr filter;
    table_->RowCount( filter, &ids );
    row->put_Value( 0, CComVariant( ids ) );
    row_->BindRow( row, ids + 1 ); // $$$$ SBO 2007-08-30: only allows one row to be edited...
    return *row_;
}

// -----------------------------------------------------------------------------
// Name: Table::DeleteRows
// Created: SBO 2007-08-31
// -----------------------------------------------------------------------------
void crossbow::Table::DeleteRows( const std::string& query )
{
    IQueryFilterPtr filter;
    if ( query != "" )
    {
        filter.CreateInstance( CLSID_QueryFilter );
        filter->put_WhereClause( CComBSTR( query.c_str() ) );
    }
    editor_.StartEdit();
    table_->DeleteSearchedRows( filter );
}

// -----------------------------------------------------------------------------
// Name: Table::Clear
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void crossbow::Table::Clear()
{
    DeleteRows( "" );
}

// -----------------------------------------------------------------------------
// Name: Table::UpdateRow
// Created: SBO 2007-09-26
// -----------------------------------------------------------------------------
void crossbow::Table::UpdateRow( const Row_ABC& row )
{
    if( &row != row_.get() )
        return;
    if( cursor_ != NULL )
        cursor_->Flush();
}

// -----------------------------------------------------------------------------
// Name: Table::UpdateRow
// Created: SBO 2007-09-26
// -----------------------------------------------------------------------------
void crossbow::Table::InsertRow( const Row_ABC& row )
{
    if( &row != row_.get() )
        return;

    ICursorPtr cursor;
    HRESULT res = table_->Insert( true, &cursor );
    if( FAILED( res ) )
        ThrowError();
    row_->Commit( cursor );
}

// -----------------------------------------------------------------------------
// Name: Table::Find
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
crossbow::Row_ABC* crossbow::Table::Find( const std::string& query, bool forceUpdate /*= false*/ )
{
    IQueryFilterPtr filter;
    
    if ( query != "" )
    {
        filter.CreateInstance( CLSID_QueryFilter );
        filter->put_WhereClause( CComBSTR( query.c_str() ) );
    }
    HRESULT res = ( forceUpdate || editor_.InTransaction() )
            ? table_->Update( filter, false, &cursor_ ) 
            : table_->Search( filter, false, &cursor_ );
    if( FAILED( res ) )
    {
        ThrowError();
        throw std::runtime_error( "Error executing query: " + query );
    }
    return GetNextRow();
}

// -----------------------------------------------------------------------------
// Name: Table::GetNextRow
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
crossbow::Row_ABC* crossbow::Table::GetNextRow()
{
    IRowPtr row;
    if( cursor_ != NULL && ( FAILED( cursor_->NextRow( &row ) ) || row == NULL ) )
        return 0;
    long oid = 0;
    row->get_OID( &oid );
    row_->BindRow( row, oid );
    return row_.get();
}
