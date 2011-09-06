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
#include "MIL_AgentServer.h"
#include "tools/xmlcodecs.h"
#include <boost/foreach.hpp>
#include <boost/optional.hpp>
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: MIL_Schedule constructor
// Created: LGY 2011-01-19
// -----------------------------------------------------------------------------
MIL_Schedule::MIL_Schedule( MIL_LivingArea_ABC& livingArea )
    : livingArea_ ( livingArea )
    , isMoving_   ( false )
    , initialized_( false )
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
    event.to_ = ConvertTime( xis.attribute< std::string >( "to" ) ); // Useless
    event.motivation_ = xis.attribute< std::string >( "motivation" );
    event.transfertTime_ = transferTime_;
    event.occurence_ = 0;
    events_.push_back( event );
}

// -----------------------------------------------------------------------------
// Name: MIL_Schedule::AddEvent
// Created: ABR 2011-03-23
// -----------------------------------------------------------------------------
void MIL_Schedule::AddEvent( const std::string& motivation, int transfertTimeInSecond /*= 0*/ )
{
    bpt::ptime pdate( bpt::from_time_t( MIL_AgentServer::GetWorkspace().GetRealTime() ) );
    unsigned int duration = MIL_AgentServer::GetWorkspace().GetTickDuration();
    int transfertTime = ( transfertTimeInSecond ) ? transfertTimeInSecond : transferTime_;

    Event event;
    event.day_= pdate.date().day_of_week();
    event.from_ = pdate.time_of_day();
    event.to_ = pdate.time_of_day() + bpt::time_duration( 0, 0, transfertTime ); // Useless
    event.motivation_ = motivation;
    event.transfertTime_ = transfertTime;
    event.occurence_ = 0;

    optionalEvent_ = event;
    Check( *optionalEvent_, pdate, duration );
}

// -----------------------------------------------------------------------------
// Name: MIL_Schedule::RestartLastEvent
// Created: ABR 2011-03-22
// -----------------------------------------------------------------------------
void MIL_Schedule::RestartLastEvent()
{
    if( !currentMotivation_ )
        return;
    AddEvent( *currentMotivation_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_Schedule::Update
// Created: LGY 2011-01-19
// -----------------------------------------------------------------------------
void MIL_Schedule::Update( unsigned int date, unsigned int duration )
{
    if( !initialized_ )
        Initialize( date );

    bool hasAlreadyMoveSomebody = false;
    bpt::ptime pdate( bpt::from_time_t( date ) );

    BOOST_FOREACH( Event& event, events_ )
        hasAlreadyMoveSomebody = hasAlreadyMoveSomebody || Check( event, pdate, duration );

    if( optionalEvent_ )
    {
        if( hasAlreadyMoveSomebody )
            optionalEvent_ = boost::none;
        else
            Check( *optionalEvent_, pdate, duration );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Schedule::Check
// Created: LGY 2011-01-20
// -----------------------------------------------------------------------------
bool MIL_Schedule::Check( Event& event, const bpt::ptime& pdate, unsigned int duration )
{
    bool result = false;
    bool goodDay = pdate.date().day_of_week() == event.day_;
    if( goodDay &&
        pdate.time_of_day() >= event.from_ &&
        pdate.time_of_day() < ( event.from_ + bpt::time_duration( 0, 0, duration ) )  )
    {
        event.occurence_ = 0;
        livingArea_.StartMotivation( event.motivation_ );
        currentMotivation_ = event.motivation_;
        isMoving_ = true;
        event.occurence_++;
        result = true;
    }
    if( goodDay &&
        pdate.time_of_day() >= event.from_ + bpt::time_duration( 0, 0, 900 * event.occurence_ ) &&
        pdate.time_of_day() < event.from_ + bpt::time_duration( 0, 0, 900 * event.occurence_ ) + bpt::time_duration( 0, 0, duration ) )
    {
        livingArea_.MovePeople( event.motivation_, 1 + static_cast< unsigned int >( event.transfertTime_ / 900 ) );
        event.occurence_++;
        result = true;
    }
    if( goodDay &&
        pdate.time_of_day() >= event.from_ + bpt::time_duration( 0, 0, boost::posix_time::time_duration::sec_type( event.transfertTime_ ) ) &&
        pdate.time_of_day() < event.from_ + bpt::time_duration( 0, 0, boost::posix_time::time_duration::sec_type( event.transfertTime_ ) ) + bpt::time_duration( 0, 0, duration ) )
    {
        livingArea_.FinishMoving( event.motivation_ );
        event.occurence_ = 0;
        isMoving_ = false;
        result = true;
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: MIL_Schedule::IsMoving
// Created: SLG 2011-01-26
// -----------------------------------------------------------------------------
bool MIL_Schedule::IsMoving() const
{
    return isMoving_;
}

namespace
{
    template< typename T >
    bool Compare( const T& lhs, const T& rhs )
    {
        return lhs.day_ < rhs.day_ || ( lhs.day_ == rhs.day_ && lhs.from_ <= rhs.from_ );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Schedule::Initialize
// Created: LGY 2011-03-21
// -----------------------------------------------------------------------------
void MIL_Schedule::Initialize( unsigned int date )
{
    if( !events_.empty() )
    {
        std::sort( events_.begin(), events_.end(), boost::bind( &Compare< Event >, _1, _2 ) );
        boost::optional< std::string > motivation;
        BOOST_FOREACH( const Event& event, events_ )
        {
            bpt::ptime pdate( bpt::from_time_t( date ) );
            if( event.day_ < pdate.date().day_of_week() || ( event.day_ == pdate.date().day_of_week() && pdate.time_of_day() >= event.from_ ) )
                motivation = event.motivation_;
        }
        if( !motivation )
            motivation = events_.back().motivation_;
        livingArea_.StartMotivation( *motivation );
        livingArea_.FinishMoving( *motivation );
    }
    initialized_ = true;
}
