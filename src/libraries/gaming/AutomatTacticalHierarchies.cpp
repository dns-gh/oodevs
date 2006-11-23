// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "AutomatTacticalHierarchies.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Formation_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AutomatTacticalHierarchies constructor
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
AutomatTacticalHierarchies::AutomatTacticalHierarchies( Controller& controller, Entity_ABC& holder, const Resolver_ABC< Formation_ABC >& resolver, PropertiesDictionary& dictionary )
    : MergingTacticalHierarchies( controller, holder, 0 )
    , controller_( controller )
    , resolver_( resolver )
{
    // NOTHING
    // $$$$ AGE 2006-10-19: dico !
}

// -----------------------------------------------------------------------------
// Name: AutomatTacticalHierarchies destructor
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
AutomatTacticalHierarchies::~AutomatTacticalHierarchies()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatTacticalHierarchies::DoUpdate
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
void AutomatTacticalHierarchies::DoUpdate( const ASN1T_MsgAutomateCreation& message )
{
    SetSuperior( &resolver_.Get( message.oid_formation ) );
}
