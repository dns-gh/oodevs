// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_DecisionCallbackAction_ABC.h"
#include "Entities/MIL_Entity_ABC.h"

// -----------------------------------------------------------------------------
// Name: PHY_DecisionCallbackAction_ABC constructor
// Created: LDC 2009-06-26
// -----------------------------------------------------------------------------
PHY_DecisionCallbackAction_ABC::PHY_DecisionCallbackAction_ABC( MIL_Entity_ABC& actor )
: PHY_Action_ABC( actor )
, decision_( actor.GetRole<DEC_Decision_ABC >() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_DecisionCallbackAction_ABC destructor
// Created: LDC 2009-06-26
// -----------------------------------------------------------------------------
PHY_DecisionCallbackAction_ABC::~PHY_DecisionCallbackAction_ABC()
{
    // NOTHING
    decision_.RemoveCallback( this );
}

// -----------------------------------------------------------------------------
// Name: PHY_DecisionCallbackAction_ABC::CallbackKnowledge
// Created: LDC 2009-06-29
// -----------------------------------------------------------------------------
void PHY_DecisionCallbackAction_ABC::CallbackKnowledge( int id )
{
    decision_.CallbackKnowledge( this, id );
}
