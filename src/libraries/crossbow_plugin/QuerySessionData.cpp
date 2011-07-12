// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "QuerySessionData.h"
#include "dispatcher/Config.h"
#include "dispatcher/Logger_ABC.h"
#include "Database_ABC.h"
#include "Row_ABC.h"
#include "Table_ABC.h"
#include "clients_kernel/CoordinateConverter_ABC.h"

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
QuerySessionData::QuerySessionData( Database_ABC& database, dispatcher::Logger_ABC& logger )
    : database_ ( database )
    , logger_ ( logger )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Destructor
// Created: MPT 2009-07-27
// -----------------------------------------------------------------------------
QuerySessionData::~QuerySessionData()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: QuerySessionData::CreateExercise
// Created: MPT 2009-07-27
// -----------------------------------------------------------------------------
int QuerySessionData::CreateExercise( const std::string& name, const kernel::CoordinateConverter_ABC& converter )
{
    std::auto_ptr< Table_ABC > table( database_.OpenTable( "Exercises" ) );
    Row_ABC& row = table->CreateRow();
    row.SetField( "name", FieldVariant( name ) );

    geometry::Rectangle2f extent( converter.GetExtent() );
    geometry::Point2d bottomLeft( converter.ConvertToGeo( extent.BottomLeft() ) );
    geometry::Point2d topRight( converter.ConvertToGeo( extent.TopRight() ) );
    row.SetField( "extent_xmin", FieldVariant( bottomLeft.X() ) );
    row.SetField( "extent_ymin", FieldVariant( bottomLeft.Y() ) );
    row.SetField( "extent_xmax", FieldVariant( topRight.X() ) );
    row.SetField( "extent_ymax", FieldVariant( topRight.Y() ) );
    table->InsertRow( row );
    // return row.GetID();
    return FindExercise( name );
}

// -----------------------------------------------------------------------------
// Name: QueryDatabaseUpdater::CreateExercise
// Created: MPT 2009-07-27
// -----------------------------------------------------------------------------
int QuerySessionData::CreateSession( const std::string& name, int exercise_id )
{
    std::auto_ptr< Table_ABC > table( database_.OpenTable( "Sessions" ) );
    Row_ABC& row = table->CreateRow();
    row.SetField( "name", FieldVariant( name ) );
        row.SetField( "exercise_id", FieldVariant( exercise_id ) );
    table->InsertRow( row );
    return FindSession( name, exercise_id );
}

// -----------------------------------------------------------------------------
// Name: QueryDatabaseUpdater::CreateExercise
// Created: MPT 2009-07-27
// -----------------------------------------------------------------------------
int QuerySessionData::FindExercise( const std::string& name ) const
{
    try
    {
        std::auto_ptr< Table_ABC > table( database_.OpenTable( "Exercises" ) );
        Row_ABC* row = table->Find( "name='" + name + "'" );
        if( row != 0 )
            return row->GetID();
    }
    catch ( std::exception& ex )
    {
        logger_.LogError( "QuerySessionData - Unable to find exercise: " + std::string( ex.what() ) );
    }
    return -1;
}

// -----------------------------------------------------------------------------
// Name: QuerySessionData::CreateExercise
// Created: MPT 2009-07-27
// -----------------------------------------------------------------------------
int QuerySessionData::FindSession( const std::string& name, int exercise_id ) const
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
        logger_.LogError( "QuerySessionData - Unable to find session: " + std::string( ex.what() ) );
    }
    return -1;
}
