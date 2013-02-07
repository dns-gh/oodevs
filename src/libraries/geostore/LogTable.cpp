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
#include <boost/assign.hpp>

using namespace geostore;

// -----------------------------------------------------------------------------
// Name: LogTable constructor
// Created: AME 2010-07-28
// -----------------------------------------------------------------------------
LogTable::LogTable( sqlite3* db )
    : Table( db, "MasaLog" )
{
    ExecuteQuery( "CREATE TABLE IF NOT EXISTS " + GetName() + " ( table_name TEXT PRIMARY KEY, last_modification INTEGER );" );
}

bool LogTable::GetLastAccessTime( const std::string& layerName, std::time_t& time )
{
    bool result = false;
    time = 0;
    sqlite3_stmt* stmt = CreateStatement( "SELECT last_modification FROM " + GetName() + " WHERE table_name = '" + layerName + "'" );
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
    sqlite3_stmt* stmt = CreateStatement( "REPLACE INTO " + GetName() + " VALUES ( ?, ? )" );
    sqlite3_bind_text( stmt, 1, layerName.c_str(), static_cast< int >( layerName.length() ), SQLITE_TRANSIENT );
    sqlite3_bind_int64( stmt, 2, time );
    StepStatement( stmt );
    sqlite3_finalize( stmt );
}
