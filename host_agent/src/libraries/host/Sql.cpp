// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "Sql.h"

#include "cpplog/cpplog.hpp"
#include "runtime/Utf8.h"

#include <boost/make_shared.hpp>
#include <boost/ref.hpp>
#include <sqlite/sqlite3.h>

using namespace host;
using runtime::Utf8Convert;

namespace
{
void SqliteClose( sqlite3* db )
{
    int err = SQLITE_BUSY;
    while( err == SQLITE_BUSY )
        err = sqlite3_close( db );
}
}

// -----------------------------------------------------------------------------
// Name: Sql::Sql
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
Sql::Sql( cpplog::BaseLogger& log,
          const Path& file )
    : log_   ( log )
    , file_  ( file )
{
    sqlite3* pdb = 0;
    int err = sqlite3_open( Utf8Convert( file_ ).c_str(), &pdb );
    if( err != SQLITE_OK )
        throw std::runtime_error( "[sql] Unable to open " + file.string() );
    db_.reset( pdb, &SqliteClose );
}

// -----------------------------------------------------------------------------
// Name: Sql::Sql
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
Sql::~Sql()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Sql::Prepare
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
Sql::Ptr Sql::Prepare( const std::string& sql )
{
    sqlite3_stmt* stmt = 0;
    boost::shared_ptr< Statement > next = boost::make_shared< Statement >( boost::ref( log_ ), boost::ref( access_ ) );
    // documentation suggest to include null-terminated byte to avoid copy
    int err = sqlite3_prepare_v2( db_.get(), sql.c_str(), static_cast< int >( sql.size() + 1 ), &stmt, 0 );
    if( err != SQLITE_OK )
    {
        LOG_ERROR( log_ ) << "[sql] " << err;
        return Ptr();
    }
    next->Assign( stmt );
    return next;
}

// -----------------------------------------------------------------------------
// Name: Statement::Statement
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
Statement::Statement( cpplog::BaseLogger& log, boost::mutex& access )
    : log_ ( log )
    , lock_( access )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Statement::~Statement
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
Statement::~Statement()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Statement::Assign
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
void Statement::Assign( sqlite3_stmt* stmt )
{
    stmt_.reset( stmt, &sqlite3_finalize );
}

#define LOG_AND_RETURN( X ) do {\
    if( ( X ) == SQLITE_OK )\
        return true;\
    LOG_INFO( log_ ) << "[sql] " << ( X );\
    return false;\
} while( 0 )

// -----------------------------------------------------------------------------
// Name: Statement::Bind
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
bool Statement::Bind( int col, double value )
{
    const int err = sqlite3_bind_double( stmt_.get(), col, value );
    LOG_AND_RETURN( err );
}

// -----------------------------------------------------------------------------
// Name: Statement::Bind
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
bool Statement::Bind( int col, int value )
{
    const int err = sqlite3_bind_int( stmt_.get(), col, value );
    LOG_AND_RETURN( err );
}

// -----------------------------------------------------------------------------
// Name: Statement::Bind
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
bool Statement::Bind( int col, int64_t value )
{
    const int err = sqlite3_bind_int64( stmt_.get(), col, value );
    LOG_AND_RETURN( err );
}

// -----------------------------------------------------------------------------
// Name: Statement::Bind
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
bool Statement::Bind( int col, const std::string& value )
{
    const int err = sqlite3_bind_text( stmt_.get(), col, value.c_str(), static_cast< int >( value.size() ), SQLITE_TRANSIENT );
    LOG_AND_RETURN( err );
}

// -----------------------------------------------------------------------------
// Name: Statement::Next
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
bool Statement::Next()
{
    const int err = sqlite3_step( stmt_.get() );
    if( err == SQLITE_ROW )
        return true;
    if( err == SQLITE_DONE )
        return false;
    LOG_AND_RETURN( err );
}

#define CHECK_TYPE( X ) do {\
    if( sqlite3_column_type( stmt_.get(), col ) != ( X ) )\
    {\
        LOG_ERROR( log_ ) << "[sql] Invalid type conversion at col " << col;\
        return false;\
    }\
} while( 0 )

// -----------------------------------------------------------------------------
// Name: Statement::Read
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
bool Statement::Read( int col, double& value )
{
    CHECK_TYPE( SQLITE_FLOAT );
    value = sqlite3_column_double( stmt_.get(), col );
    return true;
}

// -----------------------------------------------------------------------------
// Name: Statement::Read
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
bool Statement::Read( int col, int& value )
{
    CHECK_TYPE( SQLITE_INTEGER );
    value = sqlite3_column_int( stmt_.get(), col );
    return true;
}

// -----------------------------------------------------------------------------
// Name: Statement::Read
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
bool Statement::Read( int col, int64_t& value )
{
    CHECK_TYPE( SQLITE_INTEGER );
    value = sqlite3_column_int64( stmt_.get(), col );
    return true;
}

// -----------------------------------------------------------------------------
// Name: Statement::Read
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
bool Statement::Read( int col, std::string& value )
{
    CHECK_TYPE( SQLITE_TEXT );
    const unsigned char* data = sqlite3_column_text( stmt_.get(), col );
    const int size = sqlite3_column_bytes( stmt_.get(), col );
    value = std::string( reinterpret_cast< const char* >( data ), size );
    return true;
}

// -----------------------------------------------------------------------------
// Name: Statement::Reset
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
bool Statement::Reset()
{
    int err = sqlite3_reset( stmt_.get() );
    if( err == SQLITE_OK )
        err = sqlite3_clear_bindings( stmt_.get() );
    LOG_AND_RETURN( err );
}
