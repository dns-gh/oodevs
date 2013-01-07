// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef MODULE_FIXTURE_H
#define MODULE_FIXTURE_H

#include "module_tester/ApiFixture.h"

#define REGISTERED_HOOKS( APPLY ) \

#define USED_HOOKS( APPLY ) \
    APPLY( GetModifiedAltitude, 2, double, ( double x, double y ) )

#define REGISTERED_AND_USED_HOOKS( APPLY ) \

HOOK_FIXTURE( HOOKS )

namespace sword
{
namespace propagation
{
    struct ModuleFixture : core::HookFixture
    {
        ModuleFixture()
            : HookFixture( "propagation_module", boost::assign::list_of( "flood command" )
                                                                       ( "propagation command" ) )
        {}
    };
}
}

#endif // MODULE_FIXTURE_H
