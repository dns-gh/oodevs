// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_Schedule.h"
#include "MIL_LivingArea_ABC.h"
#include "simulation_terrain/TER_Localisation.h"
#include "tools/xmlcodecs.h"
#include <boost/foreach.hpp>
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: MIL_Schedule constructor
// Created: LGY 2011-01-19
// -----------------------------------------------------------------------------
MIL_Schedule::MIL_Schedule( MIL_LivingArea_ABC& livingArea )
    : livingArea_( livingArea )
    , occurence_( 0 )
    , isMoving_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Schedule destructor
// Created: LGY 2011-01-19
// -----------------------------------------------------------------------------
MIL_Schedule::~MIL_Schedule()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Schedule::Configure
// Created: LGY 2011-01-19
// -----------------------------------------------------------------------------
void MIL_Schedule::Configure( xml::xistream& xis )
{
    tools::ReadTimeAttribute( xis, "transfer-time", transferTime_ );
    xis >> xml::list( "event", *this, &MIL_Schedule::ReadEvent );
}

namespace
{
    bpt::time_duration ConvertTime( const std::string& time )
    {
        return bpt::duration_from_string( time + ":00.000" );
    }

    unsigned int ConvertDay( const std::string& day )
    {
        if( day == "sunday" )
            return 0u;
        else if( day == "monday" )
            return 1u;
        else if( day == "tuesday" )
            return 2u;
        else if( day == "wednesday" )
            return 3u;
        else if( day == "thursday" )
            return 4u;
        else if( day == "friday" )
            return 5u;
        else if( day == "saturday" )
            return 6u;
        else
            throw std::runtime_error( "Invalid day : " + day );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Schedule::ReadEvent
// Created: LGY 2011-01-19
// -----------------------------------------------------------------------------
void MIL_Schedule::ReadEvent( xml::xistream& xis )
{
    Event event;
    event.day_= ConvertDay( xis.attribute< std::string >( "day" ) );
    event.from_ = ConvertTime( xis.attribute< std::string >( "from" ) );
    event.to_ = ConvertTime( xis.attribute< std::string >( "to" ) );
    event.motivation_ = xis.attribute< std::string >( "motivation" );
    events_.push_back( event );
}

// -----------------------------------------------------------------------------
// Name: MIL_Schedule::Update
// Created: LGY 2011-01-19
// -----------------------------------------------------------------------------
void MIL_Schedule::Update( unsigned int date, unsigned int duration )
{
    BOOST_FOREACH( const Event& event, events_ )
        Check( event, date, duration );
}

// -----------------------------------------------------------------------------
// Name: MIL_Schedule::Check
// Created: LGY 2011-01-20
// -----------------------------------------------------------------------------
void MIL_Schedule::Check( const Event& event, unsigned int date, unsigned int duration )
{
    bpt::ptime pdate( bpt::from_time_t( date ) );
    if( pdate.date().day_of_week() == event.day_ && pdate.time_of_day() >= event.from_ && pdate.time_of_day() < ( event.from_ + bpt::time_duration( 0, 0, duration ) ) )
    {
        occurence_ = 0;
        livingArea_.StartMotivation( event.motivation_ );
        isMoving_ = true;
        occurence_++;
    }
    if( pdate.date().day_of_week() == event.day_ && pdate.time_of_day() >= event.from_ + bpt::time_duration( 0, 0, 900 * occurence_ ) && pdate.time_of_day() < ( event.from_ + bpt::time_duration( 0, 0, 900 * occurence_ ) + bpt::time_duration( 0, 0, duration ) ) )
    {
        livingArea_.MovePeople( 1 + static_cast< unsigned int >( transferTime_ / 900 ) );
        occurence_++;
    }
    if( pdate.date().day_of_week() == event.day_ && pdate.time_of_day() >= event.from_ + bpt::time_duration( 0, 0, transferTime_ ) && pdate.time_of_day() < ( event.from_ + bpt::time_duration( 0, 0, transferTime_ ) + bpt::time_duration( 0, 0, duration ) ) ) 
    {
        livingArea_.FinishMoving();
        occurence_ = 0;
        isMoving_ = false;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Schedule::IsMoving
// Created: SLG 2011-01-26
// -----------------------------------------------------------------------------
bool MIL_Schedule::IsMoving() const
{
    return isMoving_;
}
