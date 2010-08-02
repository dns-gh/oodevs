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
#include "protocol/AarSenders.h"
#include "protocol/ClientPublisher_ABC.h"

using namespace plugins::score;

// -----------------------------------------------------------------------------
// Name: Score constructor
// Created: SBO 2009-04-29
// -----------------------------------------------------------------------------
Score::Score()
{
    // NOTHING
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
// Name: Score::Update
// Created: SBO 2009-04-29
// -----------------------------------------------------------------------------
void Score::Update( const MsgsAarToClient::MsgIndicator& message )
{
    values_.push_back( message.value() );
}

// -----------------------------------------------------------------------------
// Name: Score::Send
// Created: SBO 2009-04-29
// -----------------------------------------------------------------------------
void Score::Send( dispatcher::ClientPublisher_ABC& publisher, int context ) const
{
    std::vector< double > values; values.reserve( values_.size() );
    std::copy( values_.begin(), values_.end(), std::back_inserter( values ) );
    aar::PlotResult result;
    result().set_identifier( context );
    result().set_error( "" );
    for( std::vector< double >::const_iterator it = values_.begin(); it != values_.end(); ++it )
        result().mutable_values()->Add( static_cast< float >( *it ) );
    result.Send( publisher );
}
