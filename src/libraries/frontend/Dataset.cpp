// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_pch.h"
#include "Dataset.h"
#include "PhysicalModel.h"

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: Dataset constructor
// Created: SBO 2007-01-29
// -----------------------------------------------------------------------------
Dataset::Dataset( kernel::Controller& controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Dataset destructor
// Created: SBO 2007-01-29
// -----------------------------------------------------------------------------
Dataset::~Dataset()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: Dataset::GetName
// Created: SBO 2007-01-29
// -----------------------------------------------------------------------------
QString Dataset::GetName() const
{
    return name_;
}
