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
#include "ClientAnnouncer_ABC.h"
#include "protocol/AarSenders.h"
#include "protocol/ClientPublisher_ABC.h"
#include <xeumeuleu/xml.hpp>

using namespace plugins::score;

// -----------------------------------------------------------------------------
// Name: Score constructor
// Created: SBO 2009-04-29
// -----------------------------------------------------------------------------
Score::Score( xml::xisubstream xis )
    : xml_( new xml::xibufferstream( xis ) )
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
void Score::Update( const sword::Indicator& message )
{
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
    for( std::deque< float >::const_iterator it = values_.begin(); it != values_.end(); ++it )
        result().mutable_values()->Add( static_cast< float >( *it ) );
    result.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Score::Size
// Created: SBO 2011-05-16
// -----------------------------------------------------------------------------
unsigned int Score::Size() const
{
    return values_.size();
}

// -----------------------------------------------------------------------------
// Name: Score::GetValue
// Created: SBO 2011-05-16
// -----------------------------------------------------------------------------
float Score::GetValue( unsigned int index ) const
{
    return values_.at( index );
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
void Score::Accept( ClientAnnouncer_ABC& visitor )
{
    if( profiles_.empty() )
        visitor.Visit( shared_from_this() );
    else
        for( std::vector< std::string >::const_iterator it = profiles_.begin(); it != profiles_.end(); ++it )
            visitor.Visit( *it, shared_from_this() );
}
