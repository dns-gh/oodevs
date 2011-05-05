// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "edxlhave_app_pch.h"
#include "PublisherActor.h"
#include "ResponseHandler_ABC.h"
#include <boost/bind.hpp>

using namespace edxl;

// -----------------------------------------------------------------------------
// Name: PublisherActor constructor
// Created: AGE 2008-05-30
// -----------------------------------------------------------------------------
PublisherActor::PublisherActor( std::auto_ptr< Publisher_ABC > base )
    : base_  ( base )
    , io_service_()
    , work_( io_service_ )
    , worker_( boost::bind( &boost::asio::io_service::run, &io_service_ ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PublisherActor destructor
// Created: AGE 2008-05-30
// -----------------------------------------------------------------------------
PublisherActor::~PublisherActor()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: PublisherActor::PullOrder
// Created: AGE 2008-05-30
// -----------------------------------------------------------------------------
void PublisherActor::PullSituation( const std::string& message, ResponseHandler_ABC& handler )
{
    io_service_.post( boost::bind( &PublisherActor::DoPullSituation, this, message, boost::ref( handler ) ) );
}

// -----------------------------------------------------------------------------
// Name: PublisherActor::DoPullOrder
// Created: AGE 2008-05-30
// -----------------------------------------------------------------------------
void PublisherActor::DoPullSituation( const std::string& message, ResponseHandler_ABC& handler )
{
    base_->PullSituation( message, handler );
}

// -----------------------------------------------------------------------------
// Name: PublisherActor::PushReports
// Created: AGE 2008-05-30
// -----------------------------------------------------------------------------
void PublisherActor::PushReport( const std::string& message )
{
    io_service_.post( boost::bind( &PublisherActor::DoPushReport, this, message ) );
}


// -----------------------------------------------------------------------------
// Name: PublisherActor::DoPushReports
// Created: AGE 2008-05-30
// -----------------------------------------------------------------------------
void PublisherActor::DoPushReport( const std::string& message )
{
    base_->PushReport( message );
}
