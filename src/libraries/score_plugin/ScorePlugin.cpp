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
#include "protocol/AarSenders.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/Protocol.h"
#include "MT_Tools/MT_Logger.h"
#include "tools/MessageDispatcher_ABC.h"
#include "tools/SessionConfig.h"
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <xeumeuleu/xml.hpp>

using namespace plugins::score;

// -----------------------------------------------------------------------------
// Name: ScorePlugin constructor
// Created: AGE 2008-08-04
// -----------------------------------------------------------------------------
ScorePlugin::ScorePlugin( tools::MessageDispatcher_ABC& dispatcher, dispatcher::LinkResolver_ABC& resolver, dispatcher::ClientPublisher_ABC& clients, const tools::SessionConfig& config, dispatcher::CompositeRegistrable& registrables )
    : resolver_( resolver )
    , config_  ( config )
    , scores_  ( new ScoresModel( config, clients ) )
{
    registrables.Add( new dispatcher::RegistrableProxy( *scores_ ) );
    scores_->Load( config_.GetScoresFile() );
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
    services.Declare( sword::service_aar ); // $$$$ SBO 2009-04-30: new service if needed (see 3a)
}

// -----------------------------------------------------------------------------
// Name: ScorePlugin::Receive
// Created: AGE 2008-08-04
// -----------------------------------------------------------------------------
void ScorePlugin::Receive( const sword::SimToClient& wrapper )
{
    scores_->Update( wrapper );
    if( wrapper.message().has_control_checkpoint_save_end() )
    {
        scores_->Export();
        scores_->SimplifiedExport( config_.GetCheckpointDirectory( tools::Path::FromUTF8( wrapper.message().control_checkpoint_save_end().name() ) ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ScorePlugin::Receive
// Created: SBO 2009-04-29
// -----------------------------------------------------------------------------
void ScorePlugin::Receive( const sword::AarToClient& wrapper )
{
    if( wrapper.message().has_indicator() )
        scores_->Update( wrapper.message().indicator() );
}

// -----------------------------------------------------------------------------
// Name: ScorePlugin::OnReceive
// Created: SBO 2009-04-29
// -----------------------------------------------------------------------------
void ScorePlugin::OnReceive( const std::string& client, const sword::ClientToAar& wrapper )
{
    if( wrapper.message().has_plot_request() )
    {
        const sword::PlotRequest& request = wrapper.message().plot_request();
        if( boost::starts_with( request.request(), "indicator://" ) )
            scores_->RequestPlot( resolver_.GetConnectedPublisher( client ), request );
    }
}

// -----------------------------------------------------------------------------
// Name: ScorePlugin::NotifyClientAuthenticated
// Created: AGE 2008-08-04
// -----------------------------------------------------------------------------
void ScorePlugin::NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& client, const std::string& /*link*/,
                                             dispatcher::Profile_ABC& profile, bool /*uncounted*/ )
{
    scores_->SendInformation( client, profile );
}
