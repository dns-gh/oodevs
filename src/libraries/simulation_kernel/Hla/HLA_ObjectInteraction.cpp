// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "HLA_ObjectInteraction.h"
#include "MIL_AgentServer.h"
#include "HLA_Federate.h"
#include "HLA_Object_ABC.h"

// -----------------------------------------------------------------------------
// Name: HLA_ObjectInteraction constructor
// Created: AGE 2004-12-06
// -----------------------------------------------------------------------------
HLA_ObjectInteraction::HLA_ObjectInteraction()
    : objectId_( unsigned int( -1 ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: HLA_ObjectInteraction constructor
// Created: AGE 2004-12-06
// -----------------------------------------------------------------------------
HLA_ObjectInteraction::HLA_ObjectInteraction( const HLA_Object_ABC& target )
    : objectId_( target.GetId() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: HLA_ObjectInteraction destructor
// Created: AGE 2004-12-06
// -----------------------------------------------------------------------------
HLA_ObjectInteraction::~HLA_ObjectInteraction()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: HLA_ObjectInteraction::GetObject
// Created: AGE 2004-12-06
// -----------------------------------------------------------------------------
MIL_Object_ABC* HLA_ObjectInteraction::GetObject() const
{
    assert( MIL_AgentServer::GetWorkspace().GetHLAFederate() );
    return MIL_AgentServer::GetWorkspace().GetHLAFederate()->GetLocalObject( objectId_ );
}
