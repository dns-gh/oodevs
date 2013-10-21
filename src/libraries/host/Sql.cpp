// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "Sql.h"

#include "runtime/Utf8.h"

#include <boost/lexical_cast.hpp>
#include <boost/make_shared.hpp>
#include <boost/ref.hpp>
#include <sqlite/sqlite3.h>

using namespace host;
using runtime::Utf8;

namespace
{
std::string GetSqliteError( int err )
{
    switch( err )
    {
        case SQLITE_OK:         return "Ok";
        case SQLITE_ERROR:      return "Error";
        case SQLITE_INTERNAL:   return "Internal error";
        case SQLITE_PERM:       return "Access denied";
        case SQLITE_ABORT:      return "Aborted";
        case SQLITE_BUSY:       return "Busy";
        case SQLITE_LOCKED:     return "Locked";
        case SQLITE_NOMEM:      return "Not enough memory";
        case SQLITE_READONLY:   return "Read only";
        case SQLITE_INTERRUPT:  return "Interrupted";
        case SQLITE_IOERR:      return "IO error";
        case SQLITE_CORRUPT:    return "Corrupted";
        case SQLITE_NOTFOUND:   return "Not found";
        case SQLITE_FULL:       return "Database full";
        case SQLITE_CANTOPEN:   return "Not opened";
        case SQLITE_PROTOCOL:   return "Protocol error";
        case SQLITE_EMPTY:      return "Database empty";
        case SQLITE_SCHEMA:     return "Schema changed";
        case SQLITE_TOOBIG:     return "Too big";
        case SQLITE_CONSTRAINT: return "Constraint violation";
        case SQLITE_MISMATCH:   return "Type Mismatch";
        case SQLITE_MISUSE:     return "API Misuse";
        case SQLITE_NOLFS:      return "Missing LFS";
        case SQLITE_AUTH:       return "Authorization denied";
        case SQLITE_FORMAT:     return "Format error";
        case SQLITE_RANGE:      return "Out of range";
        case SQLITE_NOTADB:     return "Not a database";
        case SQLITE_ROW:        return "Row ready";
        case SQLITE_DONE:       return "Done";
    }
    return boost::lexical_cast< std::string >( err );
}

void ThrowSqlException( const std::string& data, int err )
{
    throw SqlException( data + " (" + GetSqliteError( err ) + ")" );
}

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
Sql::Sql( const Path& file )
    : file_  ( file )
{
    sqlite3* pdb = 0;
    int err = sqlite3_open( Utf8( file_ ).c_str(), &pdb );
    if( err != SQLITE_OK )
        ThrowSqlException( "Unable to open " + file.string(), err );
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
// Name: Sql::Begin
// Created: BAX 2012-06-29
// -----------------------------------------------------------------------------
Sql::T_Transaction Sql::Begin( bool write )
{
    return boost::make_shared< Transaction >( boost::ref( access_ ), boost::ref( *this ), write );
}

// -----------------------------------------------------------------------------
// Name: Sql::Prepare
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
Sql::T_Statement Sql::Prepare( const Transaction& /*tr*/, const std::string& sql )
{
    sqlite3_stmt* stmt = 0;
    // documentation suggest to include null-terminated byte to avoid copy
    int err = sqlite3_prepare_v2( db_.get(), sql.c_str(), static_cast< int >( sql.size() + 1 ), &stmt, 0 );
    if( err != SQLITE_OK )
        ThrowSqlException( "Unable to prepare " + sql, err );
    return boost::make_shared< Statement >( stmt );
}

// -----------------------------------------------------------------------------
// Name: Sql::Commit
// Created: BAX 2012-06-29
// -----------------------------------------------------------------------------
void Sql::Commit( Transaction& tr )
{
    tr.Commit();
}

// -----------------------------------------------------------------------------
// Name: Sql::LastId
// Created: BAX 2012-07-11
// -----------------------------------------------------------------------------
int64_t Sql::LastId () const
{
    return sqlite3_last_insert_rowid( db_.get() );
}

// -----------------------------------------------------------------------------
// Name: Transaction::Transaction
// Created: BAX 2012-06-29
// -----------------------------------------------------------------------------
Transaction::Transaction( boost::mutex& access, Sql& db, bool write )
    : lock_ ( access )
    , db_   ( db )
    , write_( write )
{
    if( write_ )
        db_.Prepare( *this, "BEGIN TRANSACTION" )->Next();
}

// -----------------------------------------------------------------------------
// Name: Transaction::~Transaction
// Created: BAX 2012-06-29
// -----------------------------------------------------------------------------
Transaction::~Transaction()
{
    if( write_ )
        db_.Prepare( *this, "ROLLBACK TRANSACTION" )->Next();
}

// -----------------------------------------------------------------------------
// Name: Transaction::Commit
// Created: BAX 2012-06-29
// -----------------------------------------------------------------------------
void Transaction::Commit()
{
    if( write_ )
        db_.Prepare( *this, "COMMIT TRANSACTION" )->Next();
    write_ = false;
}

// -----------------------------------------------------------------------------
// Name: Statement::Statement
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
Statement::Statement( sqlite3_stmt* stmt )
    : stmt_( stmt, &sqlite3_finalize )
    , bind_( 1 )
    , read_( 0 )
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
void Statement::Bind( bool value )
{
    const int err = sqlite3_bind_int( stmt_.get(), bind_++, value );
    if( err != SQLITE_OK )
        ThrowSqlException( "Unable to bind boolean", err );
}

// -----------------------------------------------------------------------------
// Name: Statement::Bind
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
void Statement::Bind( int value )
{
    const int err = sqlite3_bind_int( stmt_.get(), bind_++, value );
    if( err != SQLITE_OK )
        ThrowSqlException( "Unable to bind integer", err );
}

// -----------------------------------------------------------------------------
// Name: Statement::Bind
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
void Statement::Bind( int64_t value )
{
    const int err = sqlite3_bind_int64( stmt_.get(), bind_++, value );
    if( err != SQLITE_OK )
        ThrowSqlException( "Unable to bind 64-bit integer", err );
}

// -----------------------------------------------------------------------------
// Name: Statement::Bind
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
void Statement::Bind( double value )
{
    const int err = sqlite3_bind_double( stmt_.get(), bind_++, value );
    if( err != SQLITE_OK )
        ThrowSqlException( "Unable to bind double", err );
}

// -----------------------------------------------------------------------------
// Name: Statement::Bind
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
void Statement::Bind( const std::string& value )
{
    const int err = sqlite3_bind_text( stmt_.get(), bind_++, value.c_str(), static_cast< int >( value.size() ), SQLITE_TRANSIENT );
    if( err != SQLITE_OK )
        ThrowSqlException( "Unable to bind text", err );
}

// -----------------------------------------------------------------------------
// Name: Statement::Bind
// Created: BAX 2012-07-02
// -----------------------------------------------------------------------------
void Statement::Bind( const char* value )
{
    Bind( std::string( value ) );
}

// -----------------------------------------------------------------------------
// Name: Statement::Bind
// Created: BAX 2012-07-23
// -----------------------------------------------------------------------------
void Statement::Bind()
{
    const int err = sqlite3_bind_null( stmt_.get(), bind_++ );
    if( err != SQLITE_OK )
        ThrowSqlException( "Unable to bind null", err );
}

// -----------------------------------------------------------------------------
// Name: Statement::Next
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
bool Statement::Next()
{
    read_ = 0;
    const int err = sqlite3_step( stmt_.get() );
    if( err == SQLITE_ROW )
        return true;
    if( err == SQLITE_DONE )
        return false;
    ThrowSqlException( "Unable to step statement", err );
    return false;
}

// -----------------------------------------------------------------------------
// Name: Statement::IsColumnDefined
// Created: BAX 2012-07-02
// -----------------------------------------------------------------------------
bool Statement::IsColumnDefined()
{
    const int type = sqlite3_column_type( stmt_.get(), read_ );
    return type != SQLITE_NULL;
}

// -----------------------------------------------------------------------------
// Name: Statement::SkipNull
// Created: BAX 2012-07-23
// -----------------------------------------------------------------------------
void Statement::SkipNull()
{
    read_++;
}

namespace
{
void CheckType( sqlite3_stmt* stmt, int expected, int col )
{
    const int actual = sqlite3_column_type( stmt, col );
    if( expected != actual )
        throw SqlException( "Invalid type conversion" );
}
}

// -----------------------------------------------------------------------------
// Name: Statement::ReadBool
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
bool Statement::ReadBool()
{
    return !!ReadInt();
}

// -----------------------------------------------------------------------------
// Name: Statement::ReadInt
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
int Statement::ReadInt()
{
    CheckType( stmt_.get(), SQLITE_INTEGER, read_ );
    return sqlite3_column_int( stmt_.get(), read_++ );
}

// -----------------------------------------------------------------------------
// Name: Statement::ReadInt64
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
int64_t Statement::ReadInt64()
{
    CheckType( stmt_.get(), SQLITE_INTEGER, read_ );
    return sqlite3_column_int64( stmt_.get(), read_++ );
}

// -----------------------------------------------------------------------------
// Name: Statement::ReadDouble
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
double Statement::ReadDouble()
{
    CheckType( stmt_.get(), SQLITE_FLOAT, read_ );
    return sqlite3_column_double( stmt_.get(), read_++ );
}

// -----------------------------------------------------------------------------
// Name: Statement::ReadText
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
std::string Statement::ReadText()
{
    const int col = read_++;
    CheckType( stmt_.get(), SQLITE_TEXT, col );
    const unsigned char* data = sqlite3_column_text( stmt_.get(), col );
    const int size = sqlite3_column_bytes( stmt_.get(), col );
    return std::string( reinterpret_cast< const char* >( data ), size );
}

// -----------------------------------------------------------------------------
// Name: Statement::Reset
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
void Statement::Reset()
{
    bind_ = 1;
    sqlite3_clear_bindings( stmt_.get() );
    const int err = sqlite3_reset( stmt_.get() );
    if( err != SQLITE_OK )
        ThrowSqlException( "Unable to reset statement", err );
}