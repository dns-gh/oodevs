// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "TacticalLine_ABC.h"

using namespace kernel;

const std::string TacticalLine_ABC::typeName_ = "tacticalLine";

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC constructor
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
TacticalLine_ABC::TacticalLine_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC destructor
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
TacticalLine_ABC::~TacticalLine_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::GetTypeName
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
const std::string& TacticalLine_ABC::GetTypeName() const
{
    return typeName_;
}

