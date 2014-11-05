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
#include "protocol/Protocol.h"
#include "tools/SessionConfig.h"
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
                const auto checkpointPath = config.GetCheckpointDirectory() / name.c_str();
                checkpointPath.Copy( filename, tools::Path::OverwriteIfExists );
            }
        }
        return std::unique_ptr< Reports >( new Reports( filename ) );
    }
}

ReportsPlugin::ReportsPlugin( const tools::SessionConfig& config, bool replay )
    : reports_( InitializeReports( config, "reports.db", replay ) )
    , config_ ( config )
{
    // NOTHING
}

ReportsPlugin::~ReportsPlugin()
{
    // NOTHING
}

void ReportsPlugin::Receive( const sword::SimToClient& msg )
{
    if( msg.message().has_control_end_tick() )
        reports_->SetTick ( msg.message().control_end_tick().current_tick() );
    else if( msg.message().has_report() )
        reports_->AddReport( msg.message().report() );
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
    reports_->ListReports( *reply.mutable_message()->mutable_list_reports_ack(), message.max_count(),
        message.has_report() ? message.report() : 0 );
    unicaster.Send( reply );
    return true;
}
