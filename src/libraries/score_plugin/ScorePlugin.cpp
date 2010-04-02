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
#include "dispatcher/ClientPublisher_ABC.h"
#include "dispatcher/CompositeRegistrable.h"
#include "dispatcher/LinkResolver_ABC.h"
#include "dispatcher/Services.h"
#include "protocol/protocol.h"
#include "MT/MT_Logger/MT_Logger_lib.h"
#include "tools/ExerciseConfig.h"
#include "tools/MessageDispatcher_ABC.h"
#include <boost/algorithm/string.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/foreach.hpp>
#include <fstream>
#include <xeumeuleu/xml.hpp>

#include "protocol/aarsenders.h"
namespace bfs = boost::filesystem;

using namespace plugins::score;

// -----------------------------------------------------------------------------
// Name: ScorePlugin constructor
// Created: AGE 2008-08-04
// -----------------------------------------------------------------------------
ScorePlugin::ScorePlugin( tools::MessageDispatcher_ABC& dispatcher, dispatcher::LinkResolver_ABC& resolver, dispatcher::ClientPublisher_ABC& clients, const tools::ExerciseConfig& config, dispatcher::CompositeRegistrable& registrables )
    : resolver_( resolver )
    , config_( config )
    , scores_( new ScoresModel( clients ) )
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
    services.Declare< aar::Service >(); // $$$$ SBO 2009-04-30: new service if needed (see 3a)
}

// -----------------------------------------------------------------------------
// Name: ScorePlugin::Receive
// Created: AGE 2008-08-04
// -----------------------------------------------------------------------------
void ScorePlugin::Receive( const MsgSimToClient& wrapper )
{
    scores_->Update( wrapper );
}

// -----------------------------------------------------------------------------
// Name: ScorePlugin::Receive
// Created: SBO 2009-04-29
// -----------------------------------------------------------------------------
void ScorePlugin::Receive( const MsgAarToClient& wrapper )
{
    if( wrapper.message().has_indicator())
        scores_->Update( wrapper.message().indicator() );
}

// -----------------------------------------------------------------------------
// Name: ScorePlugin::OnReceive
// Created: SBO 2009-04-29
// -----------------------------------------------------------------------------
void ScorePlugin::OnReceive( const std::string& client, const MsgClientToAar& wrapper )
{
    if( wrapper.message().has_plot_request())
    {
        const MsgPlotRequest& request = wrapper.message().plot_request();
        if( boost::starts_with( request.request(), "indicator://" ) )
            scores_->RequestPlot( resolver_.GetPublisher( client ), request );
    }
}

// -----------------------------------------------------------------------------
// Name: ScorePlugin::NotifyClientAuthenticated
// Created: AGE 2008-08-04
// -----------------------------------------------------------------------------
void ScorePlugin::NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& client, dispatcher::Profile_ABC& )
{
    std::string content;
    aar::AarInformation message;
    const std::string file = config_.GetScoresFile();
    if( bfs::exists( bfs::path( file, bfs::native ) ) )
    {
        std::string line;
        std::ifstream is( file.c_str() );
        while( std::getline( is, line ) )
            content += line;
        message().set_information( content.c_str() );
    }
    else
        message().set_information( "" );
    message.Send( client );
}

// -----------------------------------------------------------------------------
// Name: ScorePlugin::NotifyClientLeft
// Created: AGE 2008-08-04
// -----------------------------------------------------------------------------
void ScorePlugin::NotifyClientLeft( dispatcher::ClientPublisher_ABC& )
{
    // NOTHING
}
