// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "xmlia_plugin_pch.h"
#include "ReportManagerActor.h"
#include "ReportManager.h"
#undef Yield
#undef GetMessage
#include "tools/thread/ThreadPool.h"
#include <boost/bind.hpp>
#include "Publisher_ABC.h"

using namespace plugins::xmlia;

// -----------------------------------------------------------------------------
// Name: ReportManagerActor constructor
// Created: SLG 2008-05-30
// -----------------------------------------------------------------------------
ReportManagerActor::ReportManagerActor( ReportManager& base )
: base_( base )
, thread_( new tools::thread::ThreadPool( 1 ) )
{
  // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ReportManagerActor destructor
// Created: SLG 2008-05-30
// -----------------------------------------------------------------------------
ReportManagerActor::~ReportManagerActor()
{
  // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ReportManagerActor::PushReports
// Created: SLG 2008-05-30
// -----------------------------------------------------------------------------
void ReportManagerActor::Send( Publisher_ABC& publisher )
{
  thread_->Enqueue( boost::bind( &ReportManagerActor::DoPushReports, this, boost::ref( publisher ) ) );
}


// -----------------------------------------------------------------------------
// Name: ReportManagerActor::PullOrder
// Created: SLG 2008-05-30
// -----------------------------------------------------------------------------
void ReportManagerActor::Receive( Publisher_ABC& publisher )
{
  thread_->Enqueue( boost::bind( &ReportManagerActor::DoReceiveReports, this, boost::ref( publisher ) ) );
}

// -----------------------------------------------------------------------------
// Name: ReportManagerActor::DoPushReports
// Created: SLG 2008-05-30
// -----------------------------------------------------------------------------
void ReportManagerActor::DoPushReports( Publisher_ABC& publisher )
{
  base_.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: ReportManagerActor::DoPullOrder
// Created: SLG 2008-05-30
// -----------------------------------------------------------------------------
void ReportManagerActor::DoReceiveReports( Publisher_ABC& publisher )
{
  base_.CleanReceivedRapport();
  base_.Receive( publisher );
  base_.UpdateSimulation();
}


