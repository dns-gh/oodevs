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

BOOST_SERIALIZATION_SPLIT_FREE( sword::Pathfind );
BOOST_CLASS_EXPORT_IMPLEMENT( MIL_ItineraryParameter )

// -----------------------------------------------------------------------------
// Name: MIL_ItineraryParameter constructor
// -----------------------------------------------------------------------------
MIL_ItineraryParameter::MIL_ItineraryParameter( const sword::Pathfind& message )
    : message_( message )
{
    // NOTHING
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

namespace
{
    std::vector< boost::shared_ptr< MT_Vector2D > > ConvertPointsToWaypoints( const sword::PathResult& src )
    {
        std::vector< boost::shared_ptr< MT_Vector2D > > result;
        const auto& points = src.points();
        for( auto it = points.begin(); it != points.end(); ++it )
        {
            if( !it->has_waypoint() || it->waypoint() < 0 )
                continue;
            result.push_back( boost::make_shared< MT_Vector2D >() );
            MIL_Tools::ConvertCoordMosToSim( it->coordinate(), *result.back() );
        }
        return result;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_ItineraryParameter::ToPath
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
bool MIL_ItineraryParameter::ToPath( std::vector< boost::shared_ptr< MT_Vector2D > >& value ) const
{
    if( !message_.has_result() )
        return false;
    value = ConvertPointsToWaypoints( message_.result() );
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
}

// -----------------------------------------------------------------------------
// Name: MIL_ItineraryParameter::load_construct_data
// Created: SLI 2014-07-18
// -----------------------------------------------------------------------------
template< typename Archive >
void load_construct_data( Archive& ar, MIL_ItineraryParameter* ptr, const unsigned int )
{
    sword::Pathfind message;
    ar >> message;
    ::new( ptr ) MIL_ItineraryParameter( message );
}
