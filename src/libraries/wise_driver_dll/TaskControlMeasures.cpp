// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "wise_driver_dll_pch.h"
#include "TaskControlMeasures.h"
#include "Agent.h"
#include "Automat.h"
#include "Model.h"
#include "WiseHelpers.h"
#include "protocol/SimulationSenders.h"
#include <numeric>
#include <sstream>
#pragma warning( push )
#pragma warning( disable: 4100 4201 )
#include <wise/iwisedriversink.h>
#include <wise/WISEAttributeGroupConverter.h>
#include <wise/wisedriver.h>
#pragma warning( pop )

namespace
{
    // Must match definition from templatedb
    struct ControlMeasureType
    {
        enum Values
        {
            None          = 0,
            Heading       = 1,
            BoundaryLimit = 2,
            PhaseLine     = 3,
            Boolean       = 4,
            Agent         = 5,
            Automat       = 6,
            AgentList     = 7,
            AutomatList   = 8,
            Point         = 9,
            PointList     = 10,
            Path          = 11,
            PathList      = 12,
            Polygon       = 13,
            PolygonList   = 14,
            Location      = 15,
            LocationList  = 16
        };
    };

    std::wstring MakeIdentifier( const std::wstring& type )
    {
        static unsigned long uid = 1;
        std::wstringstream ss;
        ss << L"CM." << type << L":" << uid++;
        return ss.str();
    }
}

// -----------------------------------------------------------------------------
// Name: TaskControlMeasures constructor
// Created: SEB 2010-12-29
// -----------------------------------------------------------------------------
TaskControlMeasures::TaskControlMeasures( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime, const Model& model, const sword::MissionParameters& parameters )
    : driver_( driver )
    , database_( database )
    , currentTime_( currentTime )
    , model_( model )
{
    for( int i = 0; i < parameters.elem_size(); ++i )
        if( !AddParameter( parameters.elem( i ) ) )
            RegisterParameter( WISE_INVALID_HANDLE, ControlMeasureType::None );
}

// -----------------------------------------------------------------------------
// Name: TaskControlMeasures destructor
// Created: SEB 2010-12-29
// -----------------------------------------------------------------------------
TaskControlMeasures::~TaskControlMeasures()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TaskControlMeasures::Send
// Created: SEB 2010-12-29
// -----------------------------------------------------------------------------
void TaskControlMeasures::Send( const WISE_HANDLE& handle, std::map< std::wstring, WISE_HANDLE >& attributes ) const
{
    CHECK_WISE_RESULT_EX( driver_.GetSink()->SetEventAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle, attributes[ L"ControlMeasures.Types" ], types_ ) );
    CHECK_WISE_RESULT_EX( driver_.GetSink()->SetEventAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle, attributes[ L"ControlMeasures.Counts" ], counts_ ) );
    CHECK_WISE_RESULT_EX( driver_.GetSink()->SetEventAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle, attributes[ L"ControlMeasures.Handles" ], handles_ ) );
}

// -----------------------------------------------------------------------------
// Name: TaskControlMeasures::RegisterParameter
// Created: SEB 2010-12-29
// -----------------------------------------------------------------------------
void TaskControlMeasures::RegisterParameter( const WISE_HANDLE& handle, unsigned char type )
{
    const bool valid = ( handle != WISE_INVALID_HANDLE );
    if( valid )
        handles_.push_back( handle );
    types_.push_back( type );
    counts_.push_back( valid ? 1 : 0 );
}

// -----------------------------------------------------------------------------
// Name: TaskControlMeasures::RegisterParameter
// Created: SEB 2011-01-02
// -----------------------------------------------------------------------------
void TaskControlMeasures::RegisterParameter( std::list< WISE_HANDLE >& handles, unsigned char type )
{
    types_.push_back( type );
    counts_.push_back( unsigned char( handles.size() ) );
    handles_.splice( handles_.end(), handles );
}

// -----------------------------------------------------------------------------
// Name: TaskControlMeasures::AddParameter
// Created: SEB 2010-12-29
// -----------------------------------------------------------------------------
bool TaskControlMeasures::AddParameter( const sword::MissionParameter& parameter )
{
    if( parameter.null_value() || !parameter.value_size() )
        return false;
    const sword::MissionParameter::Value& value = parameter.value( 0 );
    if( value.has_booleanvalue() )
        CreateBoolean( value.booleanvalue() );
    else if( value.has_heading() )
        CreateHeading( value.heading() );
    else if( value.has_agent() )
    {
        if( const Agent* agent = model_.ResolveAgent( value.agent().id() ) )
            RegisterParameter( agent->GetHandle(), ControlMeasureType::Agent );
        else
            return false;
    }
    else if( value.has_automat() )
    {
        if( const Automat* automat = model_.ResolveAutomat( value.automat().id() ) )
            RegisterParameter( automat->GetHandle(), ControlMeasureType::Automat );
        else
            return false;
    }
    else if( value.has_unitlist() )
    {
        std::list< WISE_HANDLE > handles;
        for( int i = 0; i < value.unitlist().elem_size(); ++i )
        {
            const Agent* agent = model_.ResolveAgent( value.unitlist().elem( i ).id() );
            handles.push_back( agent ? agent->GetHandle() : WISE_INVALID_HANDLE );
        }
        RegisterParameter( handles, ControlMeasureType::Agent );
    }
    else if( value.has_automatlist() )
    {
        std::list< WISE_HANDLE > handles;
        for( int i = 0; i < value.automatlist().elem_size(); ++i )
        {
            const Automat* automat = model_.ResolveAutomat( value.automatlist().elem( i ).id() );
            handles.push_back( automat ? automat->GetHandle() : WISE_INVALID_HANDLE );
        }
        RegisterParameter( handles, ControlMeasureType::Automat );
    }
    else if( value.has_point() )
        CreateLocation( value.point().location(), L"Point", ControlMeasureType::Point );
    else if( value.has_pointlist() )
        CreateLocationList( value.pointlist(), L"PointList", ControlMeasureType::PointList );
    else if( value.has_path() )
        CreateLocation( value.path().location(), L"Path", ControlMeasureType::Path );
    else if( value.has_pathlist() )
        CreateLocationList( value.pathlist(), L"PathList", ControlMeasureType::PathList );
    else if( value.has_area() )
        CreateLocation( value.area().location(), L"Polygon", ControlMeasureType::Polygon );
    else if( value.has_polygonlist() )
        CreateLocationList( value.polygonlist(), L"PolygonList", ControlMeasureType::PolygonList );
    else if( value.has_location() )
        CreateLocation( value.location(), L"Location", ControlMeasureType::Location );
    else if( value.has_locationlist() )
        CreateLocationList( value.locationlist() );
    else if( value.has_limit() )
        CreateBoundaryLimit( value.limit() );
    else if( value.has_phaseline() )
        CreatePhaseLine( value.phaseline() );
    else
        return false;
    return true;
}

// -----------------------------------------------------------------------------
// Name: TaskControlMeasures::CreateBoolean
// Created: SEB 2011-01-02
// -----------------------------------------------------------------------------
void TaskControlMeasures::CreateBoolean( bool parameter )
{
    WISE_HANDLE handle = WISE_INVALID_HANDLE;
    try
    {
        std::map< std::wstring, WISE_HANDLE > attributes;
        CHECK_WISE_RESULT_EX( driver_.GetSink()->CreateObjectFromTemplate( database_, MakeIdentifier( L"Boolean" ), L"ControlMeasure.Boolean", handle, attributes ) );
        CHECK_WISE_RESULT_EX( driver_.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle, attributes[ L"Value" ], unsigned char( parameter ), currentTime_ ) );
        CHECK_WISE_RESULT_EX( driver_.GetSink()->AddObjectToDatabase( database_, handle ) );
    }
    catch( WISE_RESULT& error )
    {
        handle = WISE_INVALID_HANDLE;
        driver_.NotifyErrorMessage( L"[task-control-measure] Failed to create ControlMeasure.Boolean.", error );
    }
    RegisterParameter( handle, ControlMeasureType::Boolean );
}

// -----------------------------------------------------------------------------
// Name: TaskControlMeasures::CreateHeading
// Created: SEB 2010-12-29
// -----------------------------------------------------------------------------
void TaskControlMeasures::CreateHeading( const sword::Heading& parameter )
{
    WISE_HANDLE handle = WISE_INVALID_HANDLE;
    try
    {
        std::map< std::wstring, WISE_HANDLE > attributes;
        CHECK_WISE_RESULT_EX( driver_.GetSink()->CreateObjectFromTemplate( database_, MakeIdentifier( L"Heading" ), L"ControlMeasure.Heading", handle, attributes ) );
        CHECK_WISE_RESULT_EX( driver_.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle, attributes[ L"Value" ], long( parameter.heading() ), currentTime_ ) );
        CHECK_WISE_RESULT_EX( driver_.GetSink()->AddObjectToDatabase( database_, handle ) );
    }
    catch( WISE_RESULT& error )
    {
        handle = WISE_INVALID_HANDLE;
        driver_.NotifyErrorMessage( L"[task-control-measure] Failed to create ControlMeasure.Heading.", error );
    }
    RegisterParameter( handle, ControlMeasureType::Heading );
}

namespace
{
    std::list< CWISEVec3 > ReadPoints( const sword::Location& message )
    {
        std::list< CWISEVec3 > points;
        for( int i = 0; i < message.coordinates().elem_size(); ++i )
        {
            const sword::CoordLatLong& point = message.coordinates().elem( i );
            points.push_back( CWISEVec3( point.latitude(), point.longitude(), 0 ) );
        }
        return points;
    }
}

// -----------------------------------------------------------------------------
// Name: TaskControlMeasures::CreateBoundaryLimit
// Created: SEB 2010-12-29
// -----------------------------------------------------------------------------
void TaskControlMeasures::CreateBoundaryLimit( const sword::Line& parameter )
{
    WISE_HANDLE handle = WISE_INVALID_HANDLE;
    try
    {
        std::map< std::wstring, WISE_HANDLE > attributes;
        std::list< CWISEVec3 > points( ReadPoints( parameter.location() ) );
        CHECK_WISE_RESULT_EX( driver_.GetSink()->CreateObjectFromTemplate( database_, MakeIdentifier( L"BoundaryLimit" ), L"ControlMeasure.BoundaryLimit", handle, attributes ) );
        CHECK_WISE_RESULT_EX( driver_.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle, attributes[ L"Points" ], points, currentTime_ ) );
        CHECK_WISE_RESULT_EX( driver_.GetSink()->AddObjectToDatabase( database_, handle ) );
    }
    catch( WISE_RESULT& error )
    {
        handle = WISE_INVALID_HANDLE;
        driver_.NotifyErrorMessage( L"[task-control-measure] Failed to create ControlMeasure.BoundaryLimit.", error );
    }
    RegisterParameter( handle, ControlMeasureType::BoundaryLimit );
}

// -----------------------------------------------------------------------------
// Name: TaskControlMeasures::CreatePhaseLine
// Created: SEB 2010-12-29
// -----------------------------------------------------------------------------
void TaskControlMeasures::CreatePhaseLine( const sword::PhaseLinesOrder& parameter )
{
    std::list< WISE_HANDLE > handles;
    try
    {
        for( int i = 0; i < parameter.elem_size(); ++i )
        {
            const sword::PhaseLineOrder& phaselines = parameter.elem( i );

            WISE_HANDLE handle = WISE_INVALID_HANDLE;
            std::map< std::wstring, WISE_HANDLE > attributes;
            std::list< CWISEVec3 > points( ReadPoints( phaselines.line().location() ) );
            // $$$$ SEB 2010-12-29: TODO: handle multiple phase line functions
            const unsigned int function = phaselines.fonctions_size() > 0 ? phaselines.fonctions( 0 ) : 0;
            const std::string schedule = phaselines.time().data();
            CHECK_WISE_RESULT_EX( driver_.GetSink()->CreateObjectFromTemplate( database_, MakeIdentifier( L"PhaseLine" ), L"ControlMeasure.PhaseLine", handle, attributes ) );
            CHECK_WISE_RESULT_EX( driver_.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle, attributes[ L"Points" ], points, currentTime_ ) );
            CHECK_WISE_RESULT_EX( driver_.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle, attributes[ L"Function" ], unsigned char( function ), currentTime_ ) );
            CHECK_WISE_RESULT_EX( driver_.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle, attributes[ L"Schedule" ], std::wstring( schedule.begin(), schedule.end() ), currentTime_ ) );
            CHECK_WISE_RESULT_EX( driver_.GetSink()->AddObjectToDatabase( database_, handle ) );
            handles.push_back( handle );
        }
    }
    catch( WISE_RESULT& error )
    {
        driver_.NotifyErrorMessage( L"[task-control-measure] Failed to create ControlMeasure.PhaseLine.", error );
    }
    RegisterParameter( handles, ControlMeasureType::PhaseLine );
}

// -----------------------------------------------------------------------------
// Name: TaskControlMeasures::CreateLocation
// Created: SBO 2011-01-04
// -----------------------------------------------------------------------------
void TaskControlMeasures::CreateLocation( const sword::Location& location, const std::wstring& concreteType, unsigned char type )
{
    WISE_HANDLE handle = WISE_INVALID_HANDLE;
    try
    {
        std::map< std::wstring, WISE_HANDLE > attributes;
        std::list< CWISEVec3 > points( ReadPoints( location ) );
        CHECK_WISE_RESULT_EX( driver_.GetSink()->CreateObjectFromTemplate( database_, MakeIdentifier( concreteType ), L"ControlMeasure.Location", handle, attributes ) );
        CHECK_WISE_RESULT_EX( driver_.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle, attributes[ L"Type" ], unsigned char( location.type() ), currentTime_ ) );
        CHECK_WISE_RESULT_EX( driver_.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle, attributes[ L"Points" ], points, currentTime_ ) );
        CHECK_WISE_RESULT_EX( driver_.GetSink()->AddObjectToDatabase( database_, handle ) );
    }
    catch( WISE_RESULT& error )
    {
        handle = WISE_INVALID_HANDLE;
        driver_.NotifyErrorMessage( L"[task-control-measure] Failed to create ControlMeasure.Location (" + concreteType + L").", error );
    }
    RegisterParameter( handle, type );
}

// -----------------------------------------------------------------------------
// Name: TaskControlMeasures::CreateLocationList
// Created: SBO 2011-01-04
// -----------------------------------------------------------------------------
template< typename T >
void TaskControlMeasures::CreateLocationList( const T& list, const std::wstring& concreteType, unsigned char type )
{
    std::list< WISE_HANDLE > handles;
    try
    {
        for( int i = 0; i < list.elem_size(); ++i )
        {
            WISE_HANDLE handle = WISE_INVALID_HANDLE;
            std::map< std::wstring, WISE_HANDLE > attributes;
            std::list< CWISEVec3 > points( ReadPoints( list.elem( i ).location() ) );
            CHECK_WISE_RESULT_EX( driver_.GetSink()->CreateObjectFromTemplate( database_, MakeIdentifier( concreteType ), L"ControlMeasure.Location", handle, attributes ) );
            CHECK_WISE_RESULT_EX( driver_.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle, attributes[ L"Type" ], unsigned char( list.elem( i ).location().type() ), currentTime_ ) );
            CHECK_WISE_RESULT_EX( driver_.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle, attributes[ L"Points" ], points, currentTime_ ) );
            CHECK_WISE_RESULT_EX( driver_.GetSink()->AddObjectToDatabase( database_, handle ) );
            handles.push_back( handle );
        }
    }
    catch( WISE_RESULT& error )
    {
        driver_.NotifyErrorMessage( L"[task-control-measure] Failed to create list of ControlMeasure.Location (" + concreteType + L").", error );
    }
    RegisterParameter( handles, type );
}

// -----------------------------------------------------------------------------
// Name: TaskControlMeasures::CreateLocationList
// Created: SBO 2011-01-04
// -----------------------------------------------------------------------------
void TaskControlMeasures::CreateLocationList( const sword::LocationList& list )
{
    std::list< WISE_HANDLE > handles;
    try
    {
        for( int i = 0; i < list.elem_size(); ++i )
        {
            WISE_HANDLE handle = WISE_INVALID_HANDLE;
            std::map< std::wstring, WISE_HANDLE > attributes;
            std::list< CWISEVec3 > points( ReadPoints( list.elem( i ) ) );
            CHECK_WISE_RESULT_EX( driver_.GetSink()->CreateObjectFromTemplate( database_, MakeIdentifier( L"LocationList" ), L"ControlMeasure.Location", handle, attributes ) );
            CHECK_WISE_RESULT_EX( driver_.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle, attributes[ L"Type" ], unsigned char( list.elem( i ).type() ), currentTime_ ) );
            CHECK_WISE_RESULT_EX( driver_.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle, attributes[ L"Points" ], points, currentTime_ ) );
            CHECK_WISE_RESULT_EX( driver_.GetSink()->AddObjectToDatabase( database_, handle ) );
            handles.push_back( handle );
        }
    }
    catch( WISE_RESULT& error )
    {
        driver_.NotifyErrorMessage( L"[task-control-measure] Failed to create list of ControlMeasure.Location.", error );
    }
    RegisterParameter( handles, ControlMeasureType::LocationList );
}

namespace
{

    void FillLocation( sword::Location& message, const sword::Location::Geometry& geometry, const std::list< CWISEVec3 >& points )
    {
        message.set_type( geometry );
        for( std::list< CWISEVec3 >::const_iterator it = points.begin(); it != points.end(); ++it )
        {
            sword::CoordLatLong& coordinates = *message.mutable_coordinates()->add_elem();
            coordinates.set_latitude( it->v1 );
            coordinates.set_longitude( it->v2 );
        }
    }

    void FillParameter( CWISEDriver& driver, const WISE_HANDLE& database, const Model& model, std::list< WISE_HANDLE >& handles, unsigned char type, long count, sword::MissionParameter::Value& parameter )
    {
        if( type == ControlMeasureType::Boolean )
        {
            unsigned char value = 0;
            wise::FetchObjectAttribute( driver, database, handles.front(), L"Value", value );
            parameter.set_booleanvalue( value != 0 );
        }
        else if( type == ControlMeasureType::Heading )
        {
            long value = 0;
            wise::FetchObjectAttribute( driver, database, handles.front(), L"Value", value );
            parameter.mutable_heading()->set_heading( value );
        }
        else if( type == ControlMeasureType::Agent )
        {
            if( const Entity_ABC* entity = model.ResolveEntity( handles.front() ) )
                parameter.mutable_agent()->set_id( entity->GetId() );
        }
        else if( type == ControlMeasureType::Automat )
        {
            if( const Entity_ABC* entity = model.ResolveEntity( handles.front() ) )
                parameter.mutable_automat()->set_id( entity->GetId() );
        }
        else if( type == ControlMeasureType::AgentList )
        {
            while( count-- )
            {
                parameter.mutable_unitlist()->add_elem()->set_id( handles.front() );
                handles.pop_front();
            }
            return;
        }
        else if( type == ControlMeasureType::AutomatList )
        {
            while( count-- )
            {
                parameter.mutable_automatlist()->add_elem()->set_id( handles.front() );
                handles.pop_front();
            }
            return;
        }
        else if( type == ControlMeasureType::Point )
        {
            std::list< CWISEVec3 > points;
            wise::FetchObjectAttribute( driver, database, handles.front(), L"Points", points );
            FillLocation( *parameter.mutable_point()->mutable_location(), sword::Location::point, points );
        }
        else if( type == ControlMeasureType::PointList )
        {
            while( count-- )
            {
                std::list< CWISEVec3 > points;
                wise::FetchObjectAttribute( driver, database, handles.front(), L"Points", points );
                FillLocation( *parameter.mutable_pointlist()->add_elem()->mutable_location(), sword::Location::point, points );
                handles.pop_front();
            }
            return;
        }
        else if( type == ControlMeasureType::Path )
        {
            std::list< CWISEVec3 > points;
            wise::FetchObjectAttribute( driver, database, handles.front(), L"Points", points );
            FillLocation( *parameter.mutable_path()->mutable_location(), sword::Location::line, points );
        }
        else if( type == ControlMeasureType::PathList )
        {
            while( count-- )
            {
                std::list< CWISEVec3 > points;
                wise::FetchObjectAttribute( driver, database, handles.front(), L"Points", points );
                FillLocation( *parameter.mutable_pathlist()->add_elem()->mutable_location(), sword::Location::line, points );
                handles.pop_front();
            }
            return;
        }
        else if( type == ControlMeasureType::Polygon )
        {
            std::list< CWISEVec3 > points;
            wise::FetchObjectAttribute( driver, database, handles.front(), L"Points", points );
            FillLocation( *parameter.mutable_area()->mutable_location(), sword::Location::polygon, points );
        }
        else if( type == ControlMeasureType::PolygonList )
        {
            while( count-- )
            {
                std::list< CWISEVec3 > points;
                wise::FetchObjectAttribute( driver, database, handles.front(), L"Points", points );
                FillLocation( *parameter.mutable_polygonlist()->add_elem()->mutable_location(), sword::Location::polygon, points );
                handles.pop_front();
            }
            return;
        }
        else if( type == ControlMeasureType::Location )
        {
            std::list< CWISEVec3 > points;
            wise::FetchObjectAttribute( driver, database, handles.front(), L"Points", points );
            unsigned char type;
            wise::FetchObjectAttribute( driver, database, handles.front(), L"Type", type );
            FillLocation( *parameter.mutable_location(), sword::Location::Geometry( type ), points );
        }
        else if( type == ControlMeasureType::LocationList )
        {
            while( count-- )
            {
                std::list< CWISEVec3 > points;
                wise::FetchObjectAttribute( driver, database, handles.front(), L"Points", points );
                unsigned char type;
                wise::FetchObjectAttribute( driver, database, handles.front(), L"Type", type );
                FillLocation( *parameter.mutable_locationlist()->add_elem(), sword::Location::Geometry( type ), points );
                handles.pop_front();
            }
            return;
        }
        else if( type == ControlMeasureType::BoundaryLimit )
        {
            std::list< CWISEVec3 > points;
            wise::FetchObjectAttribute( driver, database, handles.front(), L"Points", points );
            FillLocation( *parameter.mutable_limit()->mutable_location(), sword::Location::line, points );
        }
        else if( type == ControlMeasureType::PhaseLine )
        {
            while( count-- )
            {
                std::list< CWISEVec3 > points;
                wise::FetchObjectAttribute( driver, database, handles.front(), L"Points", points );
                unsigned char function = 0;
                wise::FetchObjectAttribute( driver, database, handles.front(), L"Function", function );
                std::wstring schedule;
                wise::FetchObjectAttribute( driver, database, handles.front(), L"Schedule", schedule );
                sword::PhaseLineOrder& phaselines = *parameter.mutable_phaseline()->add_elem();
                FillLocation( *phaselines.mutable_line()->mutable_location(), sword::Location::line, points );
                phaselines.add_fonctions( sword::PhaseLineOrder::Function( function ) );
                phaselines.mutable_time()->set_data( std::string( schedule.begin(), schedule.end() ) );
                handles.pop_front();
            }
            return;
        }
        while( count-- )
            handles.pop_front();
    }
}

// -----------------------------------------------------------------------------
// Name: TaskControlMeasures::Send
// Created: SEB 2010-12-29
// -----------------------------------------------------------------------------
void TaskControlMeasures::Send( CWISEDriver& driver, const WISE_HANDLE& database, const Model& model, const WISE_HANDLE& handle, sword::MissionParameters& parameters )
{
    std::list< unsigned char > types, counts;
    wise::FetchEventAttribute( driver, database, handle, L"ControlMeasures.Types", types );
    wise::FetchEventAttribute( driver, database, handle, L"ControlMeasures.Counts", counts );
    if( types.size() != counts.size() )
    {
        const WISE_RESULT result = MAKE_WISE_RESULT( WISE_FACILITY_COM_ADAPTER, WISE_INVALID_ARG );
        driver.NotifyErrorMessage( L"[task-control-measure] Creation failed: ControlMeasures.Counts size does not match ControlMeasures.Types size", result );
        throw result;
    }
    std::list< WISE_HANDLE > handles;
    wise::FetchEventAttribute( driver, database, handle, L"ControlMeasures.Handles", handles );
    if( std::accumulate( counts.begin(), counts.end(), 0 ) != long( handles.size() ) )
    {
        const WISE_RESULT result = MAKE_WISE_RESULT( WISE_FACILITY_COM_ADAPTER, WISE_INVALID_ARG );
        driver.NotifyErrorMessage( L"[task-control-measure] Creation failed: ControlMeasures.Handles count does not match ControlMeasures.Counts", result );
        throw result;
    }
    std::list< unsigned char >::const_iterator itTypes = types.begin();
    std::list< unsigned char >::const_iterator itCounts = counts.begin();
    for( ; itTypes != types.end() && itCounts != counts.end(); ++itTypes, ++itCounts )
    {
        const unsigned char count = *itCounts;
        sword::MissionParameter& parameter = *parameters.add_elem();
        parameter.set_null_value( !count );
        if( !parameter.null_value() )
            FillParameter( driver, database, model, handles, *itTypes, count, *parameter.add_value() );
    }
}
