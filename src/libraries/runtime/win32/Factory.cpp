// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
#include <runtime/Factory.h>
#include "Api.h"
#include "Runtime.h"

namespace runtime
{
// =============================================================================
/** @class  Private
    @brief  Factory private data definition
*/
// Created: BAX 2012-03-08
// =============================================================================
struct Factory::Private
{
    Private( cpplog::BaseLogger& log )
        : api    ( log )
        , runtime( log, api )
    {
        // NOTHING
    }
    runtime::Api     api;
    runtime::Runtime runtime;
};

// -----------------------------------------------------------------------------
// Name: Factory::Factory
// Created: BAX 2012-03-08
// -----------------------------------------------------------------------------
Factory::Factory( cpplog::BaseLogger& log )
    : private_( new Private( log ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Factory::~Factory
// Created: BAX 2012-03-08
// -----------------------------------------------------------------------------
Factory::~Factory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Factory::Runtime
// Created: BAX 2012-03-08
// -----------------------------------------------------------------------------
const Runtime_ABC& Factory::GetRuntime() const
{
    return private_->runtime;
}
}
