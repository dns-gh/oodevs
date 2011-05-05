// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "edxlhave_app_pch.h"
#include "Simulation.h"
#include "SwordClient.h"

using namespace edxl;

namespace
{
    template< typename T >
    bool DecodeTime( const std::string& input, T& output )
    {
        if( input.empty() )
            return false;

        const char timeUnit = *input.rbegin();
        std::string       strTimeValue = input.substr( 0, input.size() - 1 );
        std::stringstream strTmp( strTimeValue );
        strTmp >> output;

        switch( timeUnit )
        {
            case 's': break;
            case 'm': output *= (T)60  ; break;
            case 'h': output *= (T)3600; break;
            default:
                return false;
        }
        return true;
    }
}

// -----------------------------------------------------------------------------
// Name: Simulation constructor
// Created: JCR 2010-05-31
// -----------------------------------------------------------------------------
Simulation::Simulation( const boost::program_options::variables_map& options )
    : modelLoaded_ ( false )
    , frequency_ ( 0 )
    , lastUpdate_ ( 0 )
    , current_ ( 0 )
    , tick_duration_ ( 0 )
{
    if( !DecodeTime( options[ "edxl.update.frequency" ].as< std::string >(), frequency_ ) )
        throw std::runtime_error( "Invalid time specified for checkpoint frequency" );
}

// -----------------------------------------------------------------------------
// Name: Simulation destructor
// Created: JCR 2010-05-31
// -----------------------------------------------------------------------------
Simulation::~Simulation()
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: Simulation::IsInitDone
// Created: JCR 2010-06-10
// -----------------------------------------------------------------------------
bool Simulation::IsInitDone() const
{
    return modelLoaded_;
}

// -----------------------------------------------------------------------------
// Name: Simulation::Update
// Created: JCR 2010-06-10
// -----------------------------------------------------------------------------
void Simulation::Receive( const sword::ObjectMagicActionAck& ack )
{
    modelLoaded_ |= ( sword::ObjectMagicActionAck::no_error == ack.error_code() );
}

// -----------------------------------------------------------------------------
// Name: Simulation::Receive
// Created: JCR 2010-07-09
// -----------------------------------------------------------------------------
void Simulation::Receive( const sword::ControlInformation& message )
{
    tick_duration_ = message.tick_duration();
}

// -----------------------------------------------------------------------------
// Name: Simulation::Receive
// Created: JCR 2010-05-31
// -----------------------------------------------------------------------------
void Simulation::Receive( const sword::ControlEndTick& message )
{
    current_ = ( message.current_tick() - lastUpdate_ ) * tick_duration_;
}

// -----------------------------------------------------------------------------
// Name: Simulation::NeedUpdate
// Created: JCR 2010-05-31
// -----------------------------------------------------------------------------
bool Simulation::NeedUpdate() const
{
    return IsInitDone() && frequency_ <= current_;
}

// -----------------------------------------------------------------------------
// Name: Simulation::NeedUpdate
// Created: JCR 2010-05-31
// -----------------------------------------------------------------------------
void Simulation::Update( const sword::ControlEndTick& message )
{
    current_ = 0;
    lastUpdate_ = message.current_tick();
}