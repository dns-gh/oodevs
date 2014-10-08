// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "score_plugin_pch.h"
#include "Score.h"
#include "ScoreAnnouncer.h"
#include "protocol/AarSenders.h"
#include "protocol/ClientPublisher_ABC.h"
#include <limits>
#include <xeumeuleu/xml.hpp>

using namespace plugins::score;

// -----------------------------------------------------------------------------
// Name: Score constructor
// Created: SBO 2009-04-29
// -----------------------------------------------------------------------------
Score::Score( xml::xisubstream xis )
    : xml_( new xml::xibufferstream( xis ) )
    , beginTick_( 0 )
{
    xis >> xml::optional >> xml::start( "profiles" )
            >> xml::list( "profile", *this, &Score::ReadProfile );
}

// -----------------------------------------------------------------------------
// Name: Score destructor
// Created: SBO 2009-04-29
// -----------------------------------------------------------------------------
Score::~Score()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Score::ReadProfile
// Created: SBO 2011-05-17
// -----------------------------------------------------------------------------
void Score::ReadProfile( xml::xistream& xis )
{
    profiles_.push_back( xis.attribute< std::string >( "name" ) );
}

// -----------------------------------------------------------------------------
// Name: Score::Update
// Created: SBO 2009-04-29
// -----------------------------------------------------------------------------
void Score::Update( unsigned int currentTick, const sword::Indicator& message )
{
    if( !beginTick_ )
        beginTick_ = currentTick;
    values_.push_back( message.value() );
}

// -----------------------------------------------------------------------------
// Name: Score::Send
// Created: SBO 2009-04-29
// -----------------------------------------------------------------------------
void Score::Send( dispatcher::ClientPublisher_ABC& publisher, int context ) const
{
    aar::PlotResult result;
    result().set_identifier( context );
    result().set_error( "" );
    result().set_begin_tick( beginTick_ );
    for( auto it = values_.begin(); it != values_.end(); ++it )
        result().add_values( static_cast< float >( *it ) );
    result.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Score::Size
// Created: SBO 2011-05-16
// -----------------------------------------------------------------------------
std::size_t Score::Size() const
{
    return values_.size();
}

// -----------------------------------------------------------------------------
// Name: Score::GetValue
// Created: SBO 2011-05-16
// -----------------------------------------------------------------------------
float Score::GetValue( std::size_t index ) const
{
    if( index < beginTick_ || index >= beginTick_ + values_.size() )
        return std::numeric_limits< float >::quiet_NaN();
    return values_.at( index - beginTick_ );
}

// -----------------------------------------------------------------------------
// Name: Score::Serialize
// Created: SBO 2011-05-16
// -----------------------------------------------------------------------------
void Score::Serialize( xml::xostream& xos ) const
{
    xos << xml::start( "score" )
            << *xml_
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: Score::Accept
// Created: SBO 2011-05-17
// -----------------------------------------------------------------------------
void Score::Accept( ScoreAnnouncer& visitor )
{
    if( profiles_.empty() )
        visitor.Visit( shared_from_this() );
    else
        for( std::vector< std::string >::const_iterator it = profiles_.begin(); it != profiles_.end(); ++it )
            visitor.Visit( *it, shared_from_this() );
}
