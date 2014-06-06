// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "timeline_plugin_pch.h"
#include "PublisherActor.h"
#include "ResponseHandler_ABC.h"

#undef Yield
#undef GetMessage
#include "tools/thread/ThreadPool.h"
#include <boost/bind.hpp>

using namespace plugins::timeline;

// -----------------------------------------------------------------------------
// Name: PublisherActor constructor
// Created: AGE 2008-05-30
// -----------------------------------------------------------------------------
PublisherActor::PublisherActor( std::unique_ptr< Publisher_ABC > base )
    : base_( std::move( base ) )
    , thread_( new tools::thread::ThreadPool( 1 ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PublisherActor destructor
// Created: AGE 2008-05-30
// -----------------------------------------------------------------------------
PublisherActor::~PublisherActor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PublisherActor::PullOrder
// Created: AGE 2008-05-30
// -----------------------------------------------------------------------------
void PublisherActor::PullSituation( const std::string& message, const std::string& timestamp, ResponseHandler_ABC& handler )
{
    thread_->Enqueue( boost::bind( &PublisherActor::DoPullSituation, this, message, timestamp, boost::ref( handler ) ) );
}

// -----------------------------------------------------------------------------
// Name: PublisherActor::DoPullOrder
// Created: AGE 2008-05-30
// -----------------------------------------------------------------------------
void PublisherActor::DoPullSituation( const std::string& message, const std::string& timestamp, ResponseHandler_ABC& handler )
{
    base_->PullSituation( message, timestamp, handler );
}

// -----------------------------------------------------------------------------
// Name: PublisherActor::PushReports
// Created: AGE 2008-05-30
// -----------------------------------------------------------------------------
void PublisherActor::PushReport( const std::string& message )
{
    thread_->Enqueue( boost::bind( &PublisherActor::DoPushReport, this, message ) );
}

// -----------------------------------------------------------------------------
// Name: PublisherActor::DoPushReports
// Created: AGE 2008-05-30
// -----------------------------------------------------------------------------
void PublisherActor::DoPushReport( const std::string& message )
{
    base_->PushReport( message );
}

// -----------------------------------------------------------------------------
// Name: PublisherActor::RestartScenario
// Created: JCR 2011-03-30
// -----------------------------------------------------------------------------
void PublisherActor::RestartScenario()
{
    thread_->Enqueue( boost::bind( &PublisherActor::DoRestartScenario, this ) );
}

// -----------------------------------------------------------------------------
// Name: PublisherActor::DoRestartScenario
// Created: JCR 2011-03-30
// -----------------------------------------------------------------------------
void PublisherActor::DoRestartScenario()
{
    base_->RestartScenario();
}
