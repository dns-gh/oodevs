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

Table::Table( sqlite3* db, const std::string& name )
    : db_  ( db )
    , name_( name )
{
    // NOTHING
}

const std::string& Table::GetName() const
{
    return name_;
}

namespace
{
    std::vector< std::string > Fill( int colsNb, char** colsData )
    {
        std::vector< std::string > row;
        for( int i = 0; i < colsNb; ++i )
            row.push_back( colsData[ i ] ? colsData[ i ] : "NULL" );
        return row;
    }
    int ResultCallback( void* context, int colsNb, char** colsValues, char** colsNames )
    {
        Table::T_ResultSet* set = static_cast< Table::T_ResultSet* >( context );
        if( set->empty() )
            set->push_back( Fill( colsNb, colsNames ) );
        set->push_back( Fill( colsNb, colsValues ) );
        return 0;
    }
}

Table::T_ResultSet Table::ExecuteQuery( const std::string& query )
{
    T_ResultSet result;
    if( SQLITE_OK != sqlite3_exec( db_, query.c_str(), &ResultCallback, &result, 0 ) )
        throw MASA_EXCEPTION_SQLITE( sqlite3_errmsg( db_ ) );
    return result;
}

sqlite3_stmt* Table::CreateStatement( const std::string& query ) const
{
    sqlite3_stmt* result = 0;
    if( SQLITE_OK != sqlite3_prepare_v2( db_, query.c_str(), static_cast< int >( query.length() ), &result, 0 ) )
    {
        sqlite3_finalize( result );
        throw MASA_EXCEPTION_SQLITE( sqlite3_errmsg( db_ ) );
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
        throw MASA_EXCEPTION_SQLITE( sqlite3_errmsg( db_ ) );
        break;
    }
    return err;
}
