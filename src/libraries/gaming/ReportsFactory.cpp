// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ReportsFactory.h"
#include "AgentsModel.h"
#include "Reports.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "protocol/ServerPublisher_ABC.h"
#include "protocol/Protocol.h"

ReportsFactory::ReportsFactory( Publisher_ABC& publisher, AgentsModel& agents )
    : publisher_ ( publisher )
    , agents_    ( agents )
    , tick_      ( 0 )
{
    Publisher_ABC::T_ReplayHandler handler = [&]( const sword::ReplayToClient& message )
    {
        if( message.message().has_control_skip_to_tick_ack() )
        {
            tick_ = message.message().control_skip_to_tick_ack().tick();
            ClearReports();
            SendRequest();
        }
        else if( message.message().has_list_reports_ack() )
            FillReports( message.message().list_reports_ack() );
    };
    publisher_.Register( handler );
}

ReportsFactory::~ReportsFactory()
{
    // NOTHING
}

namespace
{
    const unsigned int MAX_COUNT = 50u;

    template< typename T >
    void ClearReport( T& entity )
    {
        if( Reports* reports = entity.Retrieve< Reports>() )
            reports->Clear();
    }
    void CreateReport( AgentsModel& model, const sword::Report& message )
    {
        const auto& tasker = message.source();
        if( tasker.has_unit() )
            model.GetAgent( tasker.unit().id() ).Update( message );
        else if( tasker.has_automat() )
            model.GetAutomat( tasker.automat().id() ).Update( message );
        else if( tasker.has_crowd() )
            model.GetPopulation( tasker.crowd().id() ).Update( message );
    }
}

void ReportsFactory::ClearReports()
{
    agents_.Resolver< kernel::Agent_ABC >::Apply(
        []( kernel::Agent_ABC& entity ) { ClearReport( entity ); } );
    agents_.Resolver< kernel::Automat_ABC >::Apply(
        []( kernel::Automat_ABC& entity ) { ClearReport( entity ); } );
    agents_.Resolver< kernel::Population_ABC >::Apply(
        []( kernel::Population_ABC& entity ) { ClearReport( entity ); } );
}

void ReportsFactory::SendRequest( unsigned int report )
{
    sword::ClientToReplay message;
    sword::ListReports& list = *message.mutable_message()->mutable_list_reports();
    list.set_max_count( MAX_COUNT );
    list.set_tick( tick_ );
    if( report != 0 )
        list.set_report( report );
    publisher_.Send( message );
}

void ReportsFactory::FillReports( const sword::ListReportsAck& ack )
{
    for( int i = 0; i < ack.reports_size(); i++ )
        CreateReport( agents_, ack.reports( i ) );
    if( ack.has_next_report() )
        SendRequest( ack.next_report() );
}
