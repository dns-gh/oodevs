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
#include "MIL_Time_ABC.h"
#include "protocol/ClientSenders.h"
#include "tools/Codec.h"
#include <boost/foreach.hpp>
#include <boost/optional.hpp>

// -----------------------------------------------------------------------------
// Name: MIL_Schedule constructor
// Created: LGY 2011-01-19
// -----------------------------------------------------------------------------
MIL_Schedule::MIL_Schedule( MIL_LivingArea_ABC& livingArea )
    : livingArea_              ( livingArea )
    , isMoving_                ( false )
    , initialized_             ( false )
    , currentMotivationChanged_( true )
    , transferTime_( 0 )
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
    const int occurenceDuration = 900;

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
            throw MASA_EXCEPTION( "Invalid day : " + day );
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
    bpt::ptime pdate( bpt::from_time_t( MIL_Time_ABC::GetTime().GetRealTime() ) );
    unsigned int duration = MIL_Time_ABC::GetTime().GetTickDuration();
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
// Name: MIL_Schedule::SendFullState
// Created: JSR 2011-09-27
// -----------------------------------------------------------------------------
void MIL_Schedule::SendFullState( client::PopulationUpdate& msg ) const
{
    if( currentMotivation_ )
        msg().set_motivation( *currentMotivation_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_Schedule::UpdateNetwork
// Created: JSR 2011-09-27
// -----------------------------------------------------------------------------
void MIL_Schedule::UpdateNetwork( client::PopulationUpdate& msg ) const
{
    if( currentMotivation_ && currentMotivationChanged_ )
    {
        currentMotivationChanged_ = false;
        msg().set_motivation( *currentMotivation_ );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Schedule::SetMovingAfterCheckpoint
// Created: JSR 2012-04-16
// -----------------------------------------------------------------------------
void MIL_Schedule::SetMovingAfterCheckpoint()
{
    isMoving_ = true;
}

// -----------------------------------------------------------------------------
// Name: MIL_Schedule::Update
// Created: LGY 2011-01-19
// -----------------------------------------------------------------------------
void MIL_Schedule::Update( unsigned int date, unsigned int duration )
{
    if( !initialized_ )
    {
        Initialize( date );
        return;
    }

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
        if( event.motivation_ != currentMotivation_ )
        {
            currentMotivation_ = event.motivation_;
            currentMotivationChanged_ = true;
        }
        isMoving_ = true;
        event.occurence_++;
        result = true;
    }
    if( goodDay &&
        pdate.time_of_day() >= event.from_ + bpt::time_duration( 0, 0, occurenceDuration * event.occurence_ ) &&
        pdate.time_of_day() < event.from_ + bpt::time_duration( 0, 0, occurenceDuration * event.occurence_ ) + bpt::time_duration( 0, 0, duration ) )
    {
        livingArea_.MovePeople( event.motivation_, 1 + static_cast< unsigned int >( event.transfertTime_ / occurenceDuration ) );
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
 
        bpt::ptime pdate( bpt::from_time_t( date ) );
        int lastest = static_cast< int >( events_.size() ) - 1;
        int current = lastest;
        for( std::size_t i = 0; i < events_.size(); ++i )
        {
            const Event& event = events_[ i ];
            if( event.day_ < pdate.date().day_of_week() || ( event.day_ == pdate.date().day_of_week() && pdate.time_of_day() >= event.from_ ) )
                current = static_cast< int >( i );
        }

        int previous = ( current > 0 ) ? ( current - 1 ) : lastest;
        std::string previousMotivation  = events_[ previous ].motivation_;
        Event& curEvent = events_[ current ];
        currentMotivation_ = curEvent.motivation_;
        boost::posix_time::time_duration timeSpent = pdate.time_of_day() - curEvent.from_;
        if( previousMotivation == curEvent.motivation_ || curEvent.transfertTime_ == 0
          || pdate.date().day_of_week() != curEvent.day_ || timeSpent.total_seconds() >= curEvent.transfertTime_ )
        {
            livingArea_.StartMotivation( curEvent.motivation_ );
            livingArea_.FinishMoving( curEvent.motivation_ );
            curEvent.occurence_ = 0;
            isMoving_ = false;
        }
        else
        {
            livingArea_.StartMotivation( previousMotivation );
            livingArea_.FinishMoving( previousMotivation );
            livingArea_.StartMotivation( curEvent.motivation_ );
            curEvent.occurence_ = 1;
            isMoving_ = true;
            const int occurencesMax = 1 + static_cast< unsigned int >( curEvent.transfertTime_ / occurenceDuration );
            int timeSpent = ( pdate.time_of_day() - curEvent.from_ ).total_seconds();
            int occurencesSpent = timeSpent / occurenceDuration;
            curEvent.occurence_ += occurencesSpent;
            for( int i = 0; i < occurencesSpent; ++i )
                livingArea_.MovePeople( curEvent.motivation_, occurencesMax );
        }
    }
    initialized_ = true;
}
