// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_pch.h"
#include "PhysicalModelFactory.h"
#include "PhysicalModel.h"
#include "clients_kernel/Controllers.h"

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: PhysicalModelFactory constructor
// Created: SBO 2007-01-29
// -----------------------------------------------------------------------------
PhysicalModelFactory::PhysicalModelFactory( kernel::Controllers& controllers )
    : controllers_( controllers )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PhysicalModelFactory destructor
// Created: SBO 2007-01-29
// -----------------------------------------------------------------------------
PhysicalModelFactory::~PhysicalModelFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PhysicalModelFactory::Create
// Created: SBO 2007-01-29
// -----------------------------------------------------------------------------
PhysicalModel* PhysicalModelFactory::Create()
{
    return new PhysicalModel( controllers_.controller_ );
}
