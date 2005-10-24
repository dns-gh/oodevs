// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: NLD 2005-09-30 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "PHY_MovingEntity_ABC.h"

// -----------------------------------------------------------------------------
// Name: PHY_MovingEntity_ABC constructor
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
PHY_MovingEntity_ABC::PHY_MovingEntity_ABC()
    : pathWalker_( *this )
{

}
    
// -----------------------------------------------------------------------------
// Name: PHY_MovingEntity_ABC destructor
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
PHY_MovingEntity_ABC::~PHY_MovingEntity_ABC()
{

}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_MovingEntity_ABC::Clean
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
void PHY_MovingEntity_ABC::Clean()
{
    pathWalker_.Clean();
}
