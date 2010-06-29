// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "CapacityAttribute.h"
#include "protocol/Simulation.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: CapacityAttribute constructor
// Created: SLG 2009-12-04
// -----------------------------------------------------------------------------
CapacityAttribute::CapacityAttribute( const Model& model, const MsgsSimToClient::MsgUrbanAttributes& message )
: UrbanObjectAttribute_ABC( model, message )
, structuralState_( 1. )
{
    Update( message );
}

// -----------------------------------------------------------------------------
// Name: CapacityAttribute destructor
// Created: SLG 2009-12-04
// -----------------------------------------------------------------------------
CapacityAttribute::~CapacityAttribute()
{

}

// -----------------------------------------------------------------------------
// Name: CapacityAttribute::Update
// Created: SLG 2009-12-04
// -----------------------------------------------------------------------------
void CapacityAttribute::Update( const MsgsSimToClient::MsgUrbanAttributes& message )
{
    if( message.has_capacity() )
    {
        if( message.capacity().has_structuralstate() )
            structuralState_ = message.capacity().structuralstate();
    }
}

// -----------------------------------------------------------------------------
// Name: ArchitectureAttribute::Send
// Created: SLG 2009-12-04
// -----------------------------------------------------------------------------
void CapacityAttribute::Send( MsgsSimToClient::MsgUrbanAttributes& message ) const
{
    message.mutable_capacity()->set_structuralstate( structuralState_ );
}

// -----------------------------------------------------------------------------
// Name: CapacityAttribute::AsnDelete
// Created: SLG 2009-12-04
// -----------------------------------------------------------------------------
void CapacityAttribute::AsnDelete( MsgsSimToClient::MsgUrbanAttributes& /*message*/ ) const
{
    //NOTHING
}
