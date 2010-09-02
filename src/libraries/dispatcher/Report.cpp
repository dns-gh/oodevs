// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "Report.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/ClientSenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Report constructor
// Created: AGE 2007-10-19
// -----------------------------------------------------------------------------
Report::Report( Model_ABC& /*model*/, const MsgsSimToClient::MsgReport& report )
    : SimpleEntity< >( report.id().id() )
    , message_( report.New() )
{
    *message_ = report;
}

// -----------------------------------------------------------------------------
// Name: Report destructor
// Created: AGE 2007-10-19
// -----------------------------------------------------------------------------
Report::~Report()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Report::SendFullUpdate
// Created: AGE 2007-10-19
// -----------------------------------------------------------------------------
void Report::SendFullUpdate( ClientPublisher_ABC& ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Report::SendCreation
// Created: AGE 2007-10-19
// -----------------------------------------------------------------------------
void Report::SendCreation( ClientPublisher_ABC& publisher ) const
{
    client::Report message;
    message() = *message_;
    message.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Report::SendDestruction
// Created: AGE 2007-10-19
// -----------------------------------------------------------------------------
void Report::SendDestruction( ClientPublisher_ABC& publisher ) const
{
    client::InvalidateReport message;
    message().mutable_id()->set_id( message_->id().id() );
    *message().mutable_source() = message_->cr();
    message.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Report::Accept
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
void Report::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}
