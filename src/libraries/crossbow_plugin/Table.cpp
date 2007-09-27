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

// -----------------------------------------------------------------------------
// Name: Table constructor
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
crossbow::Table::Table( ITablePtr table, const std::string& name )
    : name_( name )
    , table_( table )
    , cursor_()
    , row_( new Row() )
    , inTransaction_( false )
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

        TCHAR szFinal[ 255 ];
        _stprintf( szFinal, _T( "%s" ), ( LPCTSTR )_bstr_t( strError ) );
        MT_LOG_ERROR_MSG( szFinal ); // $$$$ SBO 2007-08-24: should throw
    }

}

// -----------------------------------------------------------------------------
// Name: Table::CreateRow
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
crossbow::Row_ABC& crossbow::Table::CreateRow()
{
    IRowPtr row;
    HRESULT res = table_->CreateRow( &row ); // $$$$ SBO 2007-08-30: check
    if( FAILED( res ) )
        ThrowError();
    row_->BindRow( row ); // $$$$ SBO 2007-08-30: only allows one row to be edited...
    return *row_;
}

// -----------------------------------------------------------------------------
// Name: Table::DeleteRows
// Created: SBO 2007-08-31
// -----------------------------------------------------------------------------
void crossbow::Table::DeleteRows( const std::string& query )
{
    IQueryFilterPtr filter( CLSID_QueryFilter );
    filter->put_WhereClause( CComBSTR( query.c_str() ) );
    table_->DeleteSearchedRows( filter );
}

// -----------------------------------------------------------------------------
// Name: Table::Clear
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void crossbow::Table::Clear()
{
    DeleteRows( "1" );
}

// -----------------------------------------------------------------------------
// Name: Table::UpdateRow
// Created: SBO 2007-09-26
// -----------------------------------------------------------------------------
void crossbow::Table::UpdateRow( const Row_ABC& row )
{
    if( &row != row_.get() )
        return;
    row_->Commit( InTransaction() ? cursor_ : NULL );
}

// -----------------------------------------------------------------------------
// Name: Table::Find
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
crossbow::Row_ABC* crossbow::Table::Find( const std::string& query )
{
    IQueryFilterPtr filter( CLSID_QueryFilter );
    filter->put_WhereClause( CComBSTR( query.c_str() ) );
    HRESULT res = InTransaction() ? table_->Update( filter, false, &cursor_ ) : table_->Search( filter, false, &cursor_ );
    if( FAILED( res ) )
        throw std::runtime_error( "Error executing query: " + query );
    return GetNextRow();
}

// -----------------------------------------------------------------------------
// Name: Table::GetNextRow
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
crossbow::Row_ABC* crossbow::Table::GetNextRow()
{
    IRowPtr row;
    if( FAILED( cursor_->NextRow( &row ) ) || row == NULL )
        return 0;
    row_->BindRow( row );
    return row_.get();
}

// -----------------------------------------------------------------------------
// Name: Table::BeginTransaction
// Created: SBO 2007-09-26
// -----------------------------------------------------------------------------
void crossbow::Table::BeginTransaction()
{
    inTransaction_ = true;
}

// -----------------------------------------------------------------------------
// Name: Table::EndTransaction
// Created: SBO 2007-09-26
// -----------------------------------------------------------------------------
void crossbow::Table::EndTransaction()
{
    inTransaction_ = false;
}

// -----------------------------------------------------------------------------
// Name: Table::InTransaction
// Created: SBO 2007-09-26
// -----------------------------------------------------------------------------
bool crossbow::Table::InTransaction() const
{
    return inTransaction_;
}
