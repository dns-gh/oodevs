// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "TimelinePublisher.h"
#include "protocol/Protocol.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: TimelinePublisher constructor
// Created: ABR 2013-05-31
// -----------------------------------------------------------------------------
TimelinePublisher::TimelinePublisher()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TimelinePublisher destructor
// Created: ABR 2013-05-31
// -----------------------------------------------------------------------------
TimelinePublisher::~TimelinePublisher()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TimelinePublisher::Send
// Created: ABR 2013-05-31
// -----------------------------------------------------------------------------
void TimelinePublisher::Send( const sword::ClientToSim& message )
{
    message.SerializePartialToString( &currentPayload_ );
}

// -----------------------------------------------------------------------------
// Name: TimelinePublisher::Send
// Created: ABR 2013-05-31
// -----------------------------------------------------------------------------
void TimelinePublisher::Send( const sword::ClientToAuthentication& /*message*/ )
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: TimelinePublisher::Send
// Created: ABR 2013-05-31
// -----------------------------------------------------------------------------
void TimelinePublisher::Send( const sword::ClientToReplay& /*message*/ )
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: TimelinePublisher::Send
// Created: ABR 2013-05-31
// -----------------------------------------------------------------------------
void TimelinePublisher::Send( const sword::ClientToAar& /*message*/ )
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: TimelinePublisher::Send
// Created: ABR 2013-05-31
// -----------------------------------------------------------------------------
void TimelinePublisher::Send( const sword::ClientToMessenger& /*message*/ )
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: TimelinePublisher::GetPayload
// Created: ABR 2013-05-31
// -----------------------------------------------------------------------------
const std::string& TimelinePublisher::GetPayload() const
{
    return currentPayload_;
}
