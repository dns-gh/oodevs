// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "Simulation.h"
#include "protocol/ClientSenders.h"

using namespace plugins;
using namespace logger;

namespace
{
    QDateTime MakeDate( const std::string& str )
    {
        // $$$$ AGE 2007-10-12: ...
        QString extended( str.c_str() );
        extended.insert( 13, ':' ); extended.insert( 11, ':' );
        extended.insert(  6, '-' ); extended.insert(  4, '-' );
        return QDateTime::fromString( extended, Qt::ISODate );
    }
}

// -----------------------------------------------------------------------------
// Name: Simulation constructor
// Created: SBO 2010-05-11
// -----------------------------------------------------------------------------
Simulation::Simulation()
    : tickDuration_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Simulation destructor
// Created: SBO 2010-05-11
// -----------------------------------------------------------------------------
Simulation::~Simulation()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Simulation::GetDateTime
// Created: SBO 2010-05-11
// -----------------------------------------------------------------------------
QDateTime Simulation::GetDateTime() const
{
    return current_;
}

// -----------------------------------------------------------------------------
// Name: Simulation::GetInitialDateTime
// Created: SBO 2010-05-11
// -----------------------------------------------------------------------------
QDateTime Simulation::GetInitialDateTime() const
{
    return initial_;
}

// -----------------------------------------------------------------------------
// Name: Simulation::GetTickDuration
// Created: SBO 2010-05-11
// -----------------------------------------------------------------------------
unsigned int Simulation::GetTickDuration() const
{
    return tickDuration_;
}

// -----------------------------------------------------------------------------
// Name: Simulation::Update
// Created: SBO 2010-05-11
// -----------------------------------------------------------------------------
void Simulation::Update( const sword::ControlInformation& message )
{
    initial_ = MakeDate( message.initial_date_time().data() );
    current_ = MakeDate( message.date_time().data() );
    tickDuration_ = message.tick_duration();
}

// -----------------------------------------------------------------------------
// Name: Simulation::Update
// Created: SBO 2010-05-11
// -----------------------------------------------------------------------------
void Simulation::Update( const sword::ControlBeginTick& message )
{
    if( message.has_date_time() )
    {
        current_ = MakeDate( message.date_time().data() );
        if( initial_.isNull() )
            initial_ = current_;
    }
}

// -----------------------------------------------------------------------------
// Name: Simulation::GetTimeAsString
// Created: ABR 2011-08-10
// -----------------------------------------------------------------------------
QString Simulation::GetTimeAsString() const
{
    return GetDateTime().date().toString();
}
