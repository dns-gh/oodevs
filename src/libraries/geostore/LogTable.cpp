// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "geostore_pch.h"
#include "LogTable.h"

using namespace geostore;

// -----------------------------------------------------------------------------
// Name: LogTable constructor
// Created: AME 2010-07-28
// -----------------------------------------------------------------------------
LogTable::LogTable( sqlite3* db )
    : Table( db, "MasaLog" )
{
    CreateStructure();
}

bool LogTable::GetLastAccessTime( const std::string& layerName, std::time_t& time )
{
    bool result = false;

    std::ostringstream query;
    query << "SELECT last_modification FROM " << GetName() << " WHERE table_name = '" << layerName << "'";

    sqlite3_stmt* stmt = CreateStatement( query.str() );
    sqlite3_reset( stmt );
    sqlite3_clear_bindings( stmt );

    if( StepStatement( stmt ) == SQLITE_ROW )
    {
        // Retrieve the date...
        time = sqlite3_column_int64( stmt, 0 );
        result = true;
    }

    sqlite3_finalize( stmt );
    return result;
}

void LogTable::SetLastAccessTime( const std::string& layerName, const std::time_t& time )
{
    std::ostringstream query;
    query << "REPLACE INTO " << GetName() << " VALUES ( ?, ? )";

    sqlite3_stmt* stmt = CreateStatement( query.str() );
    sqlite3_reset( stmt );
    sqlite3_clear_bindings( stmt );
    sqlite3_bind_text( stmt, 1, layerName.c_str(), static_cast< int >( layerName.length() ), SQLITE_TRANSIENT );
    sqlite3_bind_int64( stmt, 2, time );
    StepStatement( stmt );
    sqlite3_finalize( stmt );
}

// -----------------------------------------------------------------------------
// Name: LogTable destructor
// Created: AME 2010-07-28
// -----------------------------------------------------------------------------
LogTable::~LogTable()
{
    // NOTHING
}

void LogTable::CreateStructure()
{
    std::stringstream query;
    query << "CREATE TABLE IF NOT EXISTS " << GetName() << " ( table_name TEXT PRIMARY KEY, last_modification INTEGER );";
    ExecuteQuery( query.str() );
}
