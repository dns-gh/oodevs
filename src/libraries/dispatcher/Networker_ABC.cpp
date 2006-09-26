// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "Networker_ABC.h"

using namespace dispatcher;
using namespace DIN;

// -----------------------------------------------------------------------------
// Name: Networker_ABC constructor
// Created: NLD 2006-09-20
// -----------------------------------------------------------------------------
Networker_ABC::Networker_ABC( Dispatcher& dispatcher )
    : dispatcher_( dispatcher )
    , dinEngine_ ()
{
}

// -----------------------------------------------------------------------------
// Name: Networker_ABC destructor
// Created: NLD 2006-09-20
// -----------------------------------------------------------------------------
Networker_ABC::~Networker_ABC()
{
}

// =============================================================================
// UPDATE
// =============================================================================

// -----------------------------------------------------------------------------
// Name: SimulationNetworker::Update
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
void Networker_ABC::Update()
{
    dinEngine_.Update();
}

