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

using namespace plugins::reports;

ReportsPlugin::ReportsPlugin( const tools::SessionConfig& config, const dispatcher::Model_ABC& model )
    : reports_( new Reports( config, model ) )
{
    // NOTHING
}

ReportsPlugin::~ReportsPlugin()
{
    // NOTHING
}

void ReportsPlugin::Receive( const sword::SimToClient& msg )
{
    if( msg.message().has_report() )
        reports_->AddReport( msg.message().report() );
    else if( msg.message().has_control_checkpoint_save_end() )
        reports_->Save( msg.message().control_checkpoint_save_end().name() );
}

bool ReportsPlugin::HandleClientToSim( const sword::ClientToSim& msg,
    dispatcher::RewritingPublisher_ABC& unicaster, dispatcher::ClientPublisher_ABC& )
{
    if( !msg.message().has_list_reports() )
        return false;
    const auto& message = msg.message().list_reports();

    sword::SimToClient reply;
    reports_->ListReports( *reply.mutable_message()->mutable_list_reports_ack(), message.max_count(),
        message.has_report() ? message.report() : 0 );
    unicaster.Send( reply );
    return true;
}
