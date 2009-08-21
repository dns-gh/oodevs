// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "score_plugin_pch.h"
#include "ScorePlugin.h"
#include "ScoresModel.h"
#include "dispatcher/CompositeRegistrable.h"
#include "dispatcher/LinkResolver_ABC.h"
#include "dispatcher/Services.h"
#include "game_asn/AarSenders.h"
#include "tools/ExerciseConfig.h"
#include "tools/MessageDispatcher_ABC.h"
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <xeumeuleu/xml.h>

using namespace plugins::score;

// -----------------------------------------------------------------------------
// Name: ScorePlugin constructor
// Created: AGE 2008-08-04
// -----------------------------------------------------------------------------
ScorePlugin::ScorePlugin( tools::MessageDispatcher_ABC& dispatcher, dispatcher::LinkResolver_ABC& resolver, dispatcher::ClientPublisher_ABC& clients, const tools::ExerciseConfig& config, dispatcher::CompositeRegistrable& registrables )
    : resolver_( resolver )
    , scores_( new ScoresModel( clients ) )
{
    registrables.Add( new dispatcher::RegistrableProxy( *scores_ ) );
    scores_->Load( config.GetScoresFile() );
    dispatcher.RegisterMessage( *this, &ScorePlugin::OnReceive );
}

// -----------------------------------------------------------------------------
// Name: ScorePlugin destructor
// Created: AGE 2008-08-04
// -----------------------------------------------------------------------------
ScorePlugin::~ScorePlugin()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ScorePlugin::Register
// Created: SBO 2009-04-29
// -----------------------------------------------------------------------------
void ScorePlugin::Register( dispatcher::Services& services )
{
    services.Declare< aar::Service >(); // $$$$ SBO 2009-04-30: new service if needed (see 3a)
}

// -----------------------------------------------------------------------------
// Name: ScorePlugin::Receive
// Created: AGE 2008-08-04
// -----------------------------------------------------------------------------
void ScorePlugin::Receive( const ASN1T_MsgsSimToClient& message )
{
    scores_->Update( message );
}

// -----------------------------------------------------------------------------
// Name: ScorePlugin::Receive
// Created: SBO 2009-04-29
// -----------------------------------------------------------------------------
void ScorePlugin::Receive( const ASN1T_MsgsAarToClient& message )
{
    if( message.msg.t == T_MsgsAarToClient_msg_msg_indicator )
        scores_->Update( *message.msg.u.msg_indicator );
}

// -----------------------------------------------------------------------------
// Name: ScorePlugin::OnReceive
// Created: SBO 2009-04-29
// -----------------------------------------------------------------------------
void ScorePlugin::OnReceive( const std::string& client, const ASN1T_MsgsClientToAar& message )
{
    if( message.msg.t == T_MsgsClientToAar_msg_msg_plot_request )
    {
        const ASN1T_MsgPlotRequest& request = *message.msg.u.msg_plot_request;
        if( boost::starts_with( request.request, "indicator://" ) )
            scores_->RequestPlot( resolver_.GetPublisher( client ), request );
    }
}

// -----------------------------------------------------------------------------
// Name: ScorePlugin::NotifyClientAuthenticated
// Created: AGE 2008-08-04
// -----------------------------------------------------------------------------
void ScorePlugin::NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& , dispatcher::Profile_ABC& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ScorePlugin::NotifyClientLeft
// Created: AGE 2008-08-04
// -----------------------------------------------------------------------------
void ScorePlugin::NotifyClientLeft( dispatcher::ClientPublisher_ABC& )
{
    // NOTHING
}
