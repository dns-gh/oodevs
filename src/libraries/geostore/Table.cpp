// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "geostore_pch.h"
#include "Table.h"

#include "DatabaseException.h"

using namespace geostore;

namespace
{
    void Fill( std::vector< std::string >& row, int colsNb, char** colsData )
    {
        row.clear();
        row.reserve( colsNb );
        for( int i = 0; i < colsNb; ++i )
        {
            std::string value( colsData[ i ] ? colsData[ i ] : "NULL" );
            row.emplace_back( value );
        }
    }

    int ResultCallback( void* context, int colsNb, char** colsValues, char** colsNames )
    {
        Table::T_ResultSet* set = static_cast< Table::T_ResultSet* >( context );

        if( set->empty() )
        {
            std::vector< std::string > row;
            Fill( row, colsNb, colsNames );
            set->emplace_back( row );
        }

        std::vector< std::string > row;
        Fill( row, colsNb, colsValues );
        set->emplace_back( row );

        return 0;
    }
}

Table::Table( sqlite3* db, const std::string& name )
    : db_  ( db )
    , name_( name )
{
}

const std::string& Table::GetName() const
{
    return name_;
}

void Table::ExecuteQuery( const std::string& query )
{
    int err = sqlite3_exec( db_, query.c_str(), nullptr, nullptr, nullptr );
    if( SQLITE_OK != err )
    {
        throw MASA_EXCEPTION_SQLITE( err, sqlite3_errmsg( db_ ) );
    }
}

void Table::ExecuteQuery( const std::string& query, T_ResultSet& result )
{
    int err = sqlite3_exec( db_, query.c_str(), &ResultCallback, &result, nullptr );
    if( SQLITE_OK != err )
    {
        throw MASA_EXCEPTION_SQLITE( err, sqlite3_errmsg( db_ ) );
    }
}

sqlite3_stmt* Table::CreateStatement( const std::string& query ) const
{
    sqlite3_stmt* result = nullptr;

    int err = sqlite3_prepare_v2( db_, query.c_str(), static_cast< int >( query.length() ), &result, NULL );
    if( SQLITE_OK != err )
    {
        sqlite3_finalize( result );
        throw MASA_EXCEPTION_SQLITE( err, sqlite3_errmsg( db_ ) );
    }

    return result;
}

int Table::StepStatement( sqlite3_stmt* stmt ) const
{
    int err = sqlite3_step( stmt );

    switch( err )
    {
    case SQLITE_OK:
    case SQLITE_ROW:
    case SQLITE_DONE:
        // Nominal cases
        break;
    default:
        // Well, something unexpected happened...
        sqlite3_finalize( stmt );
        throw MASA_EXCEPTION_SQLITE( err, sqlite3_errmsg( db_ ) );
        break;
    }

    return err;
}

sqlite3* Table::GetDB() const
{
    return db_;
}
