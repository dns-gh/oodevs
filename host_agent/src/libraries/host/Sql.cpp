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
Sql::StatementPtr Sql::Prepare( const std::string& sql )
{
    sqlite3_stmt* stmt = 0;
    // documentation suggest to include null-terminated byte to avoid copy
    int err = sqlite3_prepare_v2( db_.get(), sql.c_str(), static_cast< int >( sql.size() + 1 ), &stmt, 0 );
    return boost::make_shared< Statement >( db_, stmt );
}

// -----------------------------------------------------------------------------
// Name: Sql::Execute
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
Sql::StatementPtr Sql::Execute( const std::string& sql )
{
    return Sql::StatementPtr();
}

// -----------------------------------------------------------------------------
// Name: Statement::Statement
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
Statement::Statement( boost::shared_ptr< sqlite3 > db, sqlite3_stmt* stmt )
    : db_  ( db )
    , stmt_( stmt, &sqlite3_finalize )
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
// Name: Statement::Bind
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
bool Statement::Bind( const void* data, size_t size )
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: Statement::Bind
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
bool Statement::Bind( double value )
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: Statement::Bind
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
bool Statement::Bind( int value )
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: Statement::Bind
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
bool Statement::Bind( int64_t value )
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: Statement::Bind
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
bool Statement::Bind( const std::string& value )
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: Statement::Bind
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
bool Statement::Bind()
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: Statement::Execute
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
bool Statement::Execute()
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: Statement::RowCount
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
size_t Statement::RowCount()
{
    return 0;
}

// -----------------------------------------------------------------------------
// Name: Statement::Read
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
bool Statement::Read( size_t col, void* data, size_t size )
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: Statement::Read
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
bool Statement::Read( size_t col, double& value )
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: Statement::Read
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
bool Statement::Read( size_t col, int& value )
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: Statement::Read
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
bool Statement::Read( size_t col, int64_t& value )
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: Statement::Read
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
bool Statement::Read( size_t col, std::string& value )
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: Statement::Next
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
bool Statement::Next()
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: Statement::Reset
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
bool Statement::Reset()
{
    return false;
}
