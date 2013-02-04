// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_Effect_ABC.h"

namespace
{
    int counter_ = 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_ABC constructor
// Created: LDC 2013-02-04
// -----------------------------------------------------------------------------
MIL_Effect_ABC::MIL_Effect_ABC()
    : id_( ++counter_ )
{
        // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_ABC destructor
// Created: LDC 2013-02-04
// -----------------------------------------------------------------------------
MIL_Effect_ABC::~MIL_Effect_ABC()
{
        // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_ABC::GetID
// Created: LDC 2013-02-04
// -----------------------------------------------------------------------------
const int MIL_Effect_ABC::GetId() const
{
    return id_;
}
