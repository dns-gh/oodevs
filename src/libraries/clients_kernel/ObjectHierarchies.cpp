// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************
#include "clients_kernel_pch.h"
#include "ObjectHierarchies.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ObjectHierarchies constructor
// Created: SBO 2006-10-19
// -----------------------------------------------------------------------------
ObjectHierarchies::ObjectHierarchies( kernel::Entity_ABC& holder, const kernel::Entity_ABC* superior )
    : kernel::SimpleHierarchies< kernel::TacticalHierarchies >( holder, superior )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectHierarchies destructor
// Created: SBO 2006-10-19
// -----------------------------------------------------------------------------
ObjectHierarchies::~ObjectHierarchies()
{
    // NOTHING
}

