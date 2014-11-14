// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "ReportsPlugin.h"
#include "Reports.h"
#include "dispatcher/Config.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/ClientSenders.h"
#include "protocol/Protocol.h"
#include <boost/make_shared.hpp>

using namespace plugins::reports;

namespace
{
    std::unique_ptr< Reports > InitializeReports( const tools::SessionConfig& config, const std::string& name, bool replay )
    {
        const auto filename = config.BuildSessionChildFile( name.c_str() );
        if( !replay )
        {
            filename.Remove();
            if( config.HasCheckpoint() )
            {
                const auto checkpointPath = config.GetCheckpointDirectory() / tools::Path::FromUTF8( name );
                checkpointPath.Copy( filename, tools::Path::OverwriteIfExists );
            }
        }
        return std::unique_ptr< Reports >( new Reports( filename ) );
    }
}

ReportsPlugin::ReportsPlugin( const dispatcher::Config& config, bool replay )
    : reports_    ( InitializeReports( config, "reports.db", replay ) )
    , config_     ( config )
    , currentTick_( 0 )
{
    // NOTHING
}

ReportsPlugin::~ReportsPlugin()
{
    // NOTHING
}

void ReportsPlugin::Receive( const sword::SimToClient& msg )
{
    if( msg.message().has_control_information() )
        currentTick_ = msg.message().control_information().current_tick();
    else if( msg.message().has_control_end_tick() )
        currentTick_ = msg.message().control_end_tick().current_tick();
    else if( msg.message().has_report() )
        reports_->AddReport( msg.message().report(), currentTick_ );
    else if( msg.message().has_control_checkpoint_save_end() )
    {
        const auto checkpointName = tools::Path::FromUTF8( msg.message().control_checkpoint_save_end().name() );
        reports_->Save( config_.GetCheckpointDirectory( checkpointName / "reports.db" ) );
    }
}

bool ReportsPlugin::HandleClientToSim( const sword::ClientToSim& msg,
    dispatcher::RewritingPublisher_ABC& unicaster, dispatcher::ClientPublisher_ABC& )
{
    return HandleClientTo< sword::SimToClient >( msg, unicaster );
}

bool ReportsPlugin::HandleClientToReplay( const sword::ClientToReplay& msg,
    dispatcher::RewritingPublisher_ABC& unicaster, dispatcher::ClientPublisher_ABC& )
{
    return HandleClientTo< sword::ReplayToClient >( msg, unicaster );
}

template< typename R, typename M >
bool ReportsPlugin::HandleClientTo( const M& msg, dispatcher::RewritingPublisher_ABC& unicaster )
{
    if( !msg.message().has_list_reports() )
        return false;
    const auto& message = msg.message().list_reports();

    R reply;
    auto& ack = *reply.mutable_message()->mutable_list_reports_ack();
    ack.set_error_code( sword::ListReportsAck::no_error );
    reports_->ListReports( *reply.mutable_message()->mutable_list_reports_ack(),
        message.max_count(),
        message.has_report() ? boost::optional< unsigned int >( message.report() ) : boost::none,
        0,
        message.has_tick() ? message.tick() : std::numeric_limits< int >::max() );
    unicaster.Send( reply );
    return true;
}

void ReportsPlugin::SendState( dispatcher::ClientPublisher_ABC& publisher )
{
    const unsigned frequency = config_.GetReportsClearFrequency();
    sword::ListReportsAck list;
    reports_->ListReports( list, std::numeric_limits< int >::max(),
        boost::none,
        std::max< int >( 0, currentTick_ - frequency ),
        currentTick_ );

    for( int i = 0; i < list.reports_size(); i++ )
    {
        client::Report message;
        message() = list.reports( i );
        message.Send( publisher );
    }
}
