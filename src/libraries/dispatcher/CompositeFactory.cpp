// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "CompositeFactory.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: CompositeFactory constructor
// Created: SBO 2008-02-13
// -----------------------------------------------------------------------------
CompositeFactory::CompositeFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CompositeFactory destructor
// Created: SBO 2008-02-13
// -----------------------------------------------------------------------------
CompositeFactory::~CompositeFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CompositeFactory::AddExtension
// Created: SBO 2008-02-13
// -----------------------------------------------------------------------------
void CompositeFactory::AddExtension( Extension_ABC& extension )
{
    tools::InterfaceContainer< Extension_ABC >::Register( extension );
}
