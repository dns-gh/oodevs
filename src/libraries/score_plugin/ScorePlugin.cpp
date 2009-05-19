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
#include "Score.h"
#include "3a/AarFacade.h"
#include "3a/Task.h"
#include "dispatcher/LinkResolver_ABC.h"
#include "dispatcher/Services.h"
#include "game_asn/AarSenders.h"
#include "MT/MT_Logger/MT_Logger_lib.h"
#include "tools/ExerciseConfig.h"
#include "tools/MessageDispatcher_ABC.h"
#include <boost/algorithm/string.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/foreach.hpp>
#include <xeumeuleu/xml.h>

namespace bfs = boost::filesystem;
using namespace plugins::score;

// -----------------------------------------------------------------------------
// Name: ScorePlugin constructor
// Created: AGE 2008-08-04
// -----------------------------------------------------------------------------
ScorePlugin::ScorePlugin( tools::MessageDispatcher_ABC& dispatcher, dispatcher::LinkResolver_ABC& resolver, dispatcher::ClientPublisher_ABC& clients, const tools::ExerciseConfig& config )
    : resolver_( resolver )
    , clients_( clients )
{
    LoadScores( config.GetScoresFile() );
    dispatcher.RegisterMessage( *this, &ScorePlugin::OnReceive );
}

// -----------------------------------------------------------------------------
// Name: ScorePlugin destructor
// Created: AGE 2008-08-04
// -----------------------------------------------------------------------------
ScorePlugin::~ScorePlugin()
{
    for( T_Scores::const_iterator it = scores_.begin(); it != scores_.end(); ++it )
        delete it->second;
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
    BOOST_FOREACH( const std::vector< boost::shared_ptr< Task > >::value_type& task, tasks_ )
        task->Receive( message );
}

// -----------------------------------------------------------------------------
// Name: ScorePlugin::Receive
// Created: SBO 2009-04-29
// -----------------------------------------------------------------------------
void ScorePlugin::Receive( const ASN1T_MsgsAarToClient& message )
{
    if( message.msg.t == T_MsgsAarToClient_msg_msg_indicator )
    {
        T_Scores::const_iterator it = scores_.find( message.msg.u.msg_indicator->name );
        if( it != scores_.end() )
            it->second->Update( *message.msg.u.msg_indicator );
    }
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
        {
            T_Scores::const_iterator it = scores_.find( boost::erase_head_copy< std::string >( request.request, 12 ) );
            if( it != scores_.end() )
                it->second->Send( resolver_.GetPublisher( client ), request.identifier );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ScorePlugin::LoadScores
// Created: AGE 2008-08-04
// -----------------------------------------------------------------------------
void ScorePlugin::LoadScores( const std::string& scores )
{
    if( bfs::exists( bfs::path( scores, bfs::native ) ) )
    {
        xml::xifstream xis( scores );
        xis >> xml::start( "scores" )
                >> xml::list( "score", *this, &ScorePlugin::LoadIndicators )
            >> xml::end();
    }
}

// -----------------------------------------------------------------------------
// Name: ScorePlugin::LoadIndicators
// Created: SBO 2009-03-13
// -----------------------------------------------------------------------------
void ScorePlugin::LoadIndicators( xml::xistream& xis )
{
    scores_[ xml::attribute< std::string >( xis, "name" ) ] = new Score();
    xis >> xml::start( "indicators" )
            >> xml::list( "indicator", *this, &ScorePlugin::LoadIndicator )
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: ScorePlugin::LoadIndicator
// Created: AGE 2008-08-04
// -----------------------------------------------------------------------------
void ScorePlugin::LoadIndicator( xml::xistream& xis )
{
    try
    {
        AarFacade facade( clients_, tasks_.size() );
        tasks_.push_back( facade.CreateTask( xis ) );
    }
    catch( std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Error loading score indicators: " << e.what() );
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
