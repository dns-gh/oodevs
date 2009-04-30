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
#include "game_asn/AarSenders.h"
#include "dispatcher/ClientPublisher_ABC.h"

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
void Score::Update( const ASN1T_MsgIndicator& message )
{
    values_.push_back( message.value );
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
    result().identifier = context;
    result().error      = "";
    result().values.n    = values.size();
    result().values.elem = &values.front();
    result.Send( publisher );
}
