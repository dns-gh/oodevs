// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "StructureAttribute.h"
#include "protocol/Simulation.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: StructureAttribute constructor
// Created: SLG 2009-12-04
// -----------------------------------------------------------------------------
StructureAttribute::StructureAttribute( const MsgsSimToClient::UrbanAttributes& message )
    : structuralState_( 100 )
{
    Update( message );
}

// -----------------------------------------------------------------------------
// Name: StructureAttribute destructor
// Created: SLG 2009-12-04
// -----------------------------------------------------------------------------
StructureAttribute::~StructureAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: StructureAttribute::Update
// Created: SLG 2009-12-04
// -----------------------------------------------------------------------------
void StructureAttribute::Update( const MsgsSimToClient::UrbanAttributes& message )
{
    if( message.has_structure() && message.structure().has_state() )
        structuralState_ = message.structure().state();
}

// -----------------------------------------------------------------------------
// Name: StructureAttribute::Send
// Created: SLG 2009-12-04
// -----------------------------------------------------------------------------
void StructureAttribute::Send( MsgsSimToClient::UrbanAttributes& message ) const
{
    message.mutable_structure()->set_state( structuralState_ );
}
