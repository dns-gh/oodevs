// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_ItineraryParameter.h"
#include "protocol/Serialization.h"
#include "Tools/MIL_Tools.h"
#include <boost/make_shared.hpp>
#include <boost/serialization/optional.hpp>
#include <boost/foreach.hpp>

BOOST_SERIALIZATION_SPLIT_FREE( sword::Pathfind );
BOOST_CLASS_EXPORT_IMPLEMENT( MIL_ItineraryParameter )

namespace
{
    MT_Vector2D ToVector( const sword::PathPoint& point )
    {
        MT_Vector2D result;
        MIL_Tools::ConvertCoordMosToSim( point.coordinate(), result );
        return result;
    }

    template< typename PointsIterator >
    std::vector< boost::shared_ptr< MT_Vector2D > > ConvertPointsToWaypoints( PointsIterator begin, PointsIterator end )
    {
        std::vector< boost::shared_ptr< MT_Vector2D > > result;
        for( auto it = begin; it != end; ++it )
        {
            if( !it->has_waypoint() || it->waypoint() < 0 )
                continue;
            result.push_back( boost::make_shared< MT_Vector2D >( ToVector( *it ) ) );
        }
        return result;
    }

    boost::optional< MT_Vector2D > ToOptional( const boost::shared_ptr< MT_Vector2D >& point )
    {
        return point ? boost::make_optional( *point ) : boost::none;
    }

    template< typename Points >
    typename Points::iterator AddClosestWaypoint( Points& points, const MT_Vector2D& point )
    {
        double minDistance = std::numeric_limits< double >::max();
        auto minElement = points.begin();
        int currentWaypoint = 0;
        int lastWaypoint = 0;
        for( auto it = points.begin(); it != points.end(); ++it )
        {
            if( it->has_waypoint() && it->waypoint() >= 0 )
                currentWaypoint = it->waypoint();
            const double distance = ToVector( *it ).SquareDistance( point );
            if( distance < minDistance )
            {
                minDistance = distance;
                lastWaypoint = currentWaypoint;
                minElement = it;
            }
        }
        minElement->set_waypoint( lastWaypoint );
        for( auto it = minElement; it != points.end(); ++it )
            if( it->has_waypoint() )
                it->set_waypoint( it->waypoint() + 1 );
        return minElement;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_ItineraryParameter constructor
// -----------------------------------------------------------------------------
MIL_ItineraryParameter::MIL_ItineraryParameter( const sword::Pathfind& message, const boost::optional< MT_Vector2D >& position )
    : message_( message )
    , position_( position )
{
    Orientate( position, boost::none );
}

// -----------------------------------------------------------------------------
// Name: MIL_ItineraryParameter destructor
// -----------------------------------------------------------------------------
MIL_ItineraryParameter::~MIL_ItineraryParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ItineraryParameter::IsOfType
// -----------------------------------------------------------------------------
bool MIL_ItineraryParameter::IsOfType( MIL_ParameterType_ABC::E_Type type ) const
{
    return type == MIL_ParameterType_ABC::eItinerary || type == MIL_ParameterType_ABC::ePath;
}

// -----------------------------------------------------------------------------
// Name: MIL_ItineraryParameter::ToPath
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
bool MIL_ItineraryParameter::ToPath( std::vector< boost::shared_ptr< MT_Vector2D > >& value ) const
{
    if( !message_.has_result() )
        return false;
    value = ConvertPointsToWaypoints( message_.result().points().begin(), message_.result().points().end() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_ItineraryParameter::ToElement
// Created: MGD 2010-11-05
// -----------------------------------------------------------------------------
bool MIL_ItineraryParameter::ToElement( sword::MissionParameter_Value& elem ) const
{
    *elem.mutable_pathfind() = message_;
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_ItineraryParameter::ToItinerary
// Created: SLI 2014-07-18
// -----------------------------------------------------------------------------
bool MIL_ItineraryParameter::ToItinerary( sword::Pathfind& dst ) const
{
    dst = message_;
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_ItineraryParameter::Clip
// Created: SLI 2014-09-09
// -----------------------------------------------------------------------------
std::vector< boost::shared_ptr< MT_Vector2D > > MIL_ItineraryParameter::AddClosestWaypoints( const boost::shared_ptr< MT_Vector2D >& begin,
                                                                                             const boost::shared_ptr< MT_Vector2D >& end )
{
    Orientate( ToOptional( begin ), ToOptional( end ) );
    auto& points = *message_.mutable_result()->mutable_points();
    auto itBegin = points.begin();
    if( begin )
        itBegin = AddClosestWaypoint( points, *begin );
    auto itEnd = points.end();
    if( end )
        itEnd = AddClosestWaypoint( points, *end );
    return ConvertPointsToWaypoints( itBegin, itEnd );
}

namespace
{
    void Reverse( sword::Pathfind& message )
    {
        std::reverse( message.mutable_request()->mutable_positions()->begin(), message.mutable_request()->mutable_positions()->end() );
        std::reverse( message.mutable_result()->mutable_points()->begin(), message.mutable_result()->mutable_points()->end() );
        BOOST_FOREACH( auto point, *message.mutable_result()->mutable_points() )
            std::swap( *point.mutable_current(), *point.mutable_next() );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_ItineraryParameter::Orientate
// Created: SLI 2014-09-09
// -----------------------------------------------------------------------------
void MIL_ItineraryParameter::Orientate( const boost::optional< MT_Vector2D >& begin, const boost::optional< MT_Vector2D >& end )
{
    if( !message_.has_result() || message_.result().points().size() < 2 )
        return;
    const auto& points = message_.result().points();
    const MT_Vector2D front = ToVector( points.Get( 0 ) );
    const MT_Vector2D back = ToVector( points.Get( points.size() - 1 ) );
    if( begin && begin->Distance( front ) > begin->Distance( back ) )
        Reverse( message_ );
    else if( end && end->Distance( front ) < end->Distance( back ) )
        Reverse( message_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_ItineraryParameter::load
// Created: LGY 2011-07-25
// -----------------------------------------------------------------------------
template< typename Archive >
void MIL_ItineraryParameter::load( Archive& file, const unsigned int )
{
    file >> boost::serialization::base_object< MIL_BaseParameter >( *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_ItineraryParameter::save
// Created: LGY 2011-07-25
// -----------------------------------------------------------------------------
template< typename Archive >
void MIL_ItineraryParameter::save( Archive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< MIL_BaseParameter >( *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_ItineraryParameter::save_construct_data
// Created: SLI 2014-07-18
// -----------------------------------------------------------------------------
template< typename Archive >
void save_construct_data( Archive& ar, const MIL_ItineraryParameter* ptr, const unsigned int  )
{
    ar << ptr->message_;
    ar << ptr->position_;
}

// -----------------------------------------------------------------------------
// Name: MIL_ItineraryParameter::load_construct_data
// Created: SLI 2014-07-18
// -----------------------------------------------------------------------------
template< typename Archive >
void load_construct_data( Archive& ar, MIL_ItineraryParameter* ptr, const unsigned int )
{
    sword::Pathfind message;
    boost::optional< MT_Vector2D > position;
    ar >> message;
    ar >> position;
    ::new( ptr ) MIL_ItineraryParameter( message, position );
}
