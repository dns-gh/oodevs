// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "TeamHierarchies.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Controller.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: TeamHierarchies constructor
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
TeamHierarchies::TeamHierarchies( Controller& controller, kernel::Team_ABC& holder )
    : EntityHierarchies< CommunicationHierarchies >( controller, holder )
    , controller_( controller )
{
    controller_.Create( *(CommunicationHierarchies*)this );
}

// -----------------------------------------------------------------------------
// Name: TeamHierarchies destructor
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
TeamHierarchies::~TeamHierarchies()
{
    controller_.Delete( *(CommunicationHierarchies*)this );
}
