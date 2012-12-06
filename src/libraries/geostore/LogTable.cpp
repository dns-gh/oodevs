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
    std::ostringstream query;
    query << "SELECT last_modification FROM " << GetName() << " WHERE ( table_name = '" << layerName << "' );";

    T_ResultSet result;
    ExecuteQuery( query.str(), result );

    if( result.empty() )
    {
        time = 0;
        return false;
    }

    std::istringstream timeResult( result.back().back() );
    timeResult >> time;
    return true;
}

void LogTable::SetLastAccessTime( const std::string& layerName, const std::time_t& time )
{
    std::ostringstream query;
    query << "INSERT OR REPLACE INTO " << GetName() << " VALUES ( '" << layerName << "', " << time << " );";
    ExecuteQuery( query.str() );
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
    std::ostringstream query;
    query << "CREATE TABLE IF NOT EXISTS " << GetName() << " ( table_name TEXT PRIMARY KEY, last_modification INTEGER );";
    ExecuteQuery( query.str() );
}

// -----------------------------------------------------------------------------
// Name: LogTable::CreateStructure
// Created: AME 2010-07-28
// -----------------------------------------------------------------------------
/*void LogTable::CreateStructure()
{
    char* err_msg;
    std::string sqlRequest = "CREATE TABLE " + name_ + " (processing varchar(256) );";
    sqlite3_exec( db_, sqlRequest.c_str(), NULL, NULL, &err_msg );
    sqlite3_stmt *stmt;
    int ret = sqlite3_exec( db_, "BEGIN", NULL, NULL, &err_msg );
    sqlRequest = "INSERT INTO " + name_ + "(processing) VALUES(?)";
    ret = sqlite3_prepare_v2( db_, sqlRequest.c_str(), static_cast< int >( sqlRequest.size() ), &stmt, NULL );
    sqlite3_reset( stmt );
    sqlite3_clear_bindings( stmt );
    std::string value( "no" );
    sqlite3_bind_text( stmt, 1, value.c_str(), -1, SQLITE_TRANSIENT );
    ret = sqlite3_step( stmt );
    if( ret == SQLITE_DONE || ret == SQLITE_ROW )
        sqlite3_finalize( stmt );
    else
    {
        sqlite3_finalize( stmt );
        sqlite3_free( err_msg );
        throw std::runtime_error( __FUNCTION__ );
    }
    ret = sqlite3_exec( db_, "COMMIT" , NULL, NULL, &err_msg );
    status_ = false;
}*/

// -----------------------------------------------------------------------------
// Name: LogTable::GetStatus
// Created: AME 2010-07-28
// -----------------------------------------------------------------------------
void LogTable::GetStatus()
{
    char** results;
    int n_rows;
    int n_columns;
    std::string sqlRequest = "SELECT processing FROM " + name_ + ";"; 
    int ret = sqlite3_get_table( db_, sqlRequest.c_str(), &results, &n_rows, &n_columns, &err_msg );
    if( ret == SQLITE_OK  && n_rows != 0 )
    {
        std::string process( results[ 1 ] );
        if( process == "yes" )
            status_ = true;
    }
    else
        status_ = false;
    sqlite3_free_table( results );
}

// -----------------------------------------------------------------------------
// Name: LogTable::UpdateStatus
// Created: AME 2010-07-28
// -----------------------------------------------------------------------------
void LogTable::UpdateStatus( const std::string& status )
{
    char* err_msg;  
    sqlite3_exec( db_, "BEGIN", NULL, NULL, &err_msg );
    std::string sqlRequest = "UPDATE " + name_ + " SET processing='" + status + "';";
    sqlite3_exec( db_, sqlRequest.c_str(), NULL, NULL, &err_msg );
    sqlite3_exec( db_, "COMMIT" , NULL, NULL, &err_msg );
    sqlite3_free( err_msg );
    GetStatus();
}
