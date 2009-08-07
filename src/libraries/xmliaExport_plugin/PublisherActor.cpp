// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "xmliaExport_plugin_pch.h"
#include "PublisherActor.h"
#include "ResponseHandler_ABC.h"
#undef Yield
#undef GetMessage
#include "tools/thread/ThreadPool.h"
#include <boost/bind.hpp>

using namespace plugins::xmliaExport;

// -----------------------------------------------------------------------------
// Name: PublisherActor constructor
// Created: AGE 2008-05-30
// -----------------------------------------------------------------------------
PublisherActor::PublisherActor( std::auto_ptr< Publisher_ABC > base )
    : base_( base )
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
// Name: PublisherActor::PushReports
// Created: AGE 2008-05-30
// -----------------------------------------------------------------------------
void PublisherActor::PushReports()
{
    thread_->Enqueue( boost::bind( &PublisherActor::DoPushReports, this ) );
}

// -----------------------------------------------------------------------------
// Name: PublisherActor::CreateReport
// Created: AGE 2008-05-30
// -----------------------------------------------------------------------------
xml::xostream& PublisherActor::CreateReport()
{
    return base_->CreateReport();
}

// -----------------------------------------------------------------------------
// Name: PublisherActor::DoPushReports
// Created: AGE 2008-05-30
// -----------------------------------------------------------------------------
void PublisherActor::DoPushReports()
{
    base_->PushReports();
}


