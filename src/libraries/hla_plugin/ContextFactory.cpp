// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "ContextFactory.h"

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: ContextFactory constructor
// Created: SLI 2011-09-09
// -----------------------------------------------------------------------------
ContextFactory::ContextFactory()
    : context_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ContextFactory destructor
// Created: SLI 2011-09-09
// -----------------------------------------------------------------------------
ContextFactory::~ContextFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ContextFactory::Create
// Created: SLI 2011-09-09
// -----------------------------------------------------------------------------
int ContextFactory::Create() const
{
    return ++context_;
}
