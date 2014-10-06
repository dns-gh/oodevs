// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "VisionCone.h"
#include "protocol/ClientPublisher_ABC.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "protocol/ClientSenders.h"

using namespace dispatcher;

VisionCone::VisionCone( Model& /*model*/, const sword::UnitVisionCones& msg )
    : SimpleEntity<>( msg.unit().id() )
    , msg_( msg )
{
    AddExtension( *this );
}

VisionCone::~VisionCone()
{
    // NOTHING
}

void VisionCone::SendFullUpdate( ClientPublisher_ABC& publisher ) const
{
    client::UnitVisionCones msg;
    msg() = msg_;
    msg.Send( publisher );
}

void VisionCone::SendCreation( ClientPublisher_ABC& /*publisher*/ ) const
{
    // NOTHING
}

void VisionCone::SendDestruction( ClientPublisher_ABC& /*publisher*/ ) const
{
    // NOTHING
}

void VisionCone::DoUpdate( const sword::UnitVisionCones& msg )
{
    msg_ = msg;
}

void VisionCone::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}
