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
#include "Publisher_ABC.h"
#include "SerializationTools.h"
#include "OrderProcessor.h"
#include <xeumeuleu/xml.hpp>
#pragma warning( push, 1 )
#include <boost/date_time/posix_time/posix_time.hpp>
#pragma warning( pop )
#include <iostream>

namespace bpt = boost::posix_time;
using namespace plugins::bml;

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
UpdateListener::UpdateListener( Publisher_ABC& publisher, const dispatcher::Model_ABC& model, const kernel::StaticModel& staticModel, dispatcher::SimulationPublisher_ABC& simulation )
    : publisher_     ( publisher )
    , orderProcessor_( new OrderProcessor( model, staticModel, simulation ) )
    , lastUpdateTime_( bpt::to_iso_extended_string( bpt::from_time_t( 0 ) ).c_str() )
    , receivedAnswer_( true )
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
// Name: UpdateListener::PullOrders
// Created: AGE 2008-05-30
// -----------------------------------------------------------------------------
void UpdateListener::PullOrders()
{
    if( receivedAnswer_ )
    {
        receivedAnswer_ = false;
        PullOrders( lastUpdateTime_ );
    }
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
        << xml::end;
    publisher_.PullOrder( xos.str(), *this );
}

// -----------------------------------------------------------------------------
// Name: UpdateListener::Handle
// Created: AGE 2008-05-30
// -----------------------------------------------------------------------------
void UpdateListener::Handle( const std::string& response )
{
    receivedAnswer_ = true;
    FindLastDate( response );
    orderProcessor_->Handle( response );
}

// -----------------------------------------------------------------------------
// Name: UpdateListener::FindLastDate
// Created: SBO 2008-06-06
// -----------------------------------------------------------------------------
void UpdateListener::FindLastDate( const std::string& response )
{
    xml::xistringstream xis( response );
    xis >> xml::start( NS( "OrderPush", "cbml" ) )
            >> xml::list( NS( "OrderPush", "cbml" ), *this, &UpdateListener::ReadOrder )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: UpdateListener::ReadOrder
// Created: SBO 2008-06-06
// -----------------------------------------------------------------------------
void UpdateListener::ReadOrder( xml::xistream& xis )
{
    static const std::size_t minLength = std::string( "YYYY-MM-DDTHH:MM:SS" ).length();
    static const std::size_t maxLength = std::string( "YYYY-MM-DDTHH:MM:SS,fffffffff" ).length();
    std::string when;
    xis >> xml::content( NS( "OrderIssuedWhen", "cbml" ), when );
    if( lastUpdateTime_ < when )
    {
        lastUpdateTime_ = when;
        if( when.length() > minLength && when.length() < maxLength )
            lastUpdateTime_ += '9'; // $$$$ SBO 2008-06-06: ahaha
    }
}
