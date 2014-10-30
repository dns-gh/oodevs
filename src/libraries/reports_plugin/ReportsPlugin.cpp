// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "ReportsPlugin.h"
#include "dispatcher/Model_ABC.h"
#include "dispatcher/Report.h"
#include "protocol/Protocol.h"

using namespace plugins::reports;

ReportsPlugin::ReportsPlugin( const dispatcher::Model_ABC& model )
    : model_( model )
{
    // NOTHING
}

ReportsPlugin::~ReportsPlugin()
{
    // NOTHING
}

bool ReportsPlugin::HandleClientToSim( const sword::ClientToSim& msg,
    dispatcher::RewritingPublisher_ABC& unicaster, dispatcher::ClientPublisher_ABC& )
{
    if( !msg.message().has_list_reports() )
        return false;
    const auto& message = msg.message().list_reports();
    const auto& resolver = model_.Reports();

    sword::SimToClient reply;
    auto& reports = *reply.mutable_message()->mutable_list_reports_ack();
    for( auto it = resolver.CreateIterator(); it.HasMoreElements(); )
    {
        const dispatcher::Report& report = it.NextElement();
        if( !message.has_report() || message.report() <= report.GetId() )
        {
            if( reports.reports_size() < static_cast< int >( message.max_count() ) )
                report.Fill( *reports.add_reports() );
            else
            {
                reports.set_next_report( report.GetId() );
                break;
            }
        }
    }
    unicaster.Send( reply );
    return true;
}
