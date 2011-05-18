// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "score_plugin_pch.h"
#include "ScoreAnnouncer.h"
#include "Score.h"
#include "dispatcher/Profile_ABC.h"
#include "protocol/AarSenders.h"
#include "protocol/ClientPublisher_ABC.h"
#include <boost/foreach.hpp>
#include <xeumeuleu/xml.hpp>

using namespace plugins::score;

// -----------------------------------------------------------------------------
// Name: ScoreAnnouncer constructor
// Created: SBO 2011-05-17
// -----------------------------------------------------------------------------
ScoreAnnouncer::ScoreAnnouncer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ScoreAnnouncer destructor
// Created: SBO 2011-05-17
// -----------------------------------------------------------------------------
ScoreAnnouncer::~ScoreAnnouncer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ScoreAnnouncer::Announce
// Created: SBO 2011-05-17
// -----------------------------------------------------------------------------
void ScoreAnnouncer::Announce( dispatcher::ClientPublisher_ABC& client, dispatcher::Profile_ABC& profile )
{
    xml::xostringstream xos;
    xos << xml::start( "scores" );
    T_ProfileScores::const_iterator it = profileScores_.find( profile.GetName() );
    if( it != profileScores_.end() )
        BOOST_FOREACH( T_Score score, it->second )
            score->Serialize( xos );
    BOOST_FOREACH( T_Score score, everybodyScores_ )
        score->Serialize( xos );
    xos << xml::end;
    aar::AarInformation message;
    message().set_information( xos.str() );
    message.Send( client );
}

// -----------------------------------------------------------------------------
// Name: ScoreAnnouncer::Visit
// Created: SBO 2011-05-17
// -----------------------------------------------------------------------------
void ScoreAnnouncer::Visit( boost::shared_ptr< Score > score )
{
    everybodyScores_.push_back( score );
}

// -----------------------------------------------------------------------------
// Name: ScoreAnnouncer::Visit
// Created: SBO 2011-05-17
// -----------------------------------------------------------------------------
void ScoreAnnouncer::Visit( const std::string& profile, boost::shared_ptr< Score > score )
{
    profileScores_[ profile ].push_back( score );
}
