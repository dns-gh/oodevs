// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "bml_plugin_pch.h"
#include "UpdateListener.h"
#include "Publisher.h"
#include "SerializationTools.h"
#include "OrderProcessor.h"
#include <xeumeuleu/xml.h>
#pragma warning( push, 1 )
#include <boost/date_time/posix_time/posix_time.hpp>
#pragma warning( pop )
#include <iostream>

namespace bpt = boost::posix_time;
using namespace bml;

namespace
{
    std::string CurrentTime()
    {
        return bpt::to_iso_extended_string( bpt::second_clock::local_time() );
    }
}

// -----------------------------------------------------------------------------
// Name: UpdateListener constructor
// Created: SBO 2008-05-16
// -----------------------------------------------------------------------------
UpdateListener::UpdateListener( Publisher& publisher, const dispatcher::Model& model, dispatcher::SimulationPublisher_ABC& simulation )
    : publisher_( publisher )
    , orderProcessor_( new OrderProcessor( model, simulation ) )
    , lastUpdateTime_( bpt::to_iso_extended_string( bpt::from_time_t( 0 ) ).c_str() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UpdateListener destructor
// Created: SBO 2008-05-16
// -----------------------------------------------------------------------------
UpdateListener::~UpdateListener()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UpdateListener::Update
// Created: SBO 2008-05-16
// -----------------------------------------------------------------------------
void UpdateListener::Update( const ASN1T_MsgControlBeginTick& )
{
    PullOrders( lastUpdateTime_ );
    lastUpdateTime_ = CurrentTime();
}

// -----------------------------------------------------------------------------
// Name: UpdateListener::PullOrders
// Created: SBO 2008-05-16
// -----------------------------------------------------------------------------
void UpdateListener::PullOrders( const std::string& time )
{
	xml::xostringstream xos;
	xos << xml::start( "OrderPull" ) << Namespaces()
			<< xml::content( "PostedTimeCutoff", time )
		<< xml::end();
	publisher_.PullOrder( xos.str(), *orderProcessor_ );
}
