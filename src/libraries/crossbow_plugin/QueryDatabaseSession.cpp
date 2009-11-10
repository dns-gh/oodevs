// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "QueryDatabaseSession.h"
#include "dispatcher/Config.h"
#include "QueryBuilder.h"
#include "Database_ABC.h"
#include "Row_ABC.h"
#include "Table_ABC.h"

#pragma warning( push )
#pragma warning( disable: 4127 4512 4511 )
#include <boost/filesystem/path.hpp>
#pragma warning( pop )

using namespace plugins;
using namespace plugins::crossbow;
namespace bfs = boost::filesystem;


// -----------------------------------------------------------------------------
// Name: Constructor
// Created: MPT 2009-07-27
// -----------------------------------------------------------------------------
QueryDatabaseSession::QueryDatabaseSession( Database_ABC& database )
    : database_ ( database )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Destructor
// Created: MPT 2009-07-27
// -----------------------------------------------------------------------------
QueryDatabaseSession::~QueryDatabaseSession()
{
    // NOTHING
}



// -----------------------------------------------------------------------------
// Name: QueryDatabaseSession::CreateExercise
// Created: MPT 2009-07-27
// -----------------------------------------------------------------------------
int QueryDatabaseSession::CreateExercise( const std::string& name )
{
    InsertQueryBuilder builder( database_.GetTableName( "Exercises" ) );
	builder.SetId( "id" );
    builder.SetField( "name", name );
    database_.Execute( builder );
    return FindExercise( name );
}

// -----------------------------------------------------------------------------
// Name: QueryDatabaseUpdater::CreateExercise
// Created: MPT 2009-07-27
// -----------------------------------------------------------------------------
int QueryDatabaseSession::CreateSession( const std::string& name, int exercise_id )
{
    InsertQueryBuilder builder( database_.GetTableName( "Sessions" ) );
	builder.SetId( "id" );
    builder.SetField( "name", name );
	builder.SetField( "exercise_id", exercise_id );
    database_.Execute( builder );
    return FindSession( name, exercise_id );
}

// -----------------------------------------------------------------------------
// Name: QueryDatabaseUpdater::CreateExercise
// Created: MPT 2009-07-27
// -----------------------------------------------------------------------------
int QueryDatabaseSession::FindExercise( const std::string& name ) const
{
    try 
    {
        std::stringstream ss;
        ss << "name='" << name << "'";
        std::auto_ptr< Table_ABC > table( database_.OpenTable( "Exercises" ) );
        Row_ABC* row = table->Find( ss.str() );
        if ( row != 0 )
            return row->GetID();
    }
    catch ( std::exception& ex )
    {
        MT_LOG_ERROR_MSG( "QueryDatabaseSession - Unable to find exercise: " + std::string( ex.what() ) );
    }
	return -1;
}

// -----------------------------------------------------------------------------
// Name: QueryDatabaseSession::CreateExercise
// Created: MPT 2009-07-27
// -----------------------------------------------------------------------------
int QueryDatabaseSession::FindSession( const std::string& name, int exercise_id ) const
{
    try 
    {
        std::stringstream ss;
        ss << "name='" << name << "' AND exercise_id=" << exercise_id;
        std::auto_ptr< Table_ABC > table( database_.OpenTable( "Sessions" ) );
        Row_ABC* row = table->Find( ss.str() );
        if( row != 0 )
            return row->GetID();
    }
    catch ( std::exception& ex )
    {
        MT_LOG_ERROR_MSG( "QueryDatabaseSession - Unable to find session: " + std::string( ex.what() ) );
    }
	return -1;
}