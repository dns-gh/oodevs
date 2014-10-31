// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "TimelineHelpers.h"

namespace timeline_helpers
{

    // -----------------------------------------------------------------------------
    // Name: CreateEventTarget
    // Created: JSR 2014-10-31
    // -----------------------------------------------------------------------------
    std::string CreateEventTarget( const std::string& protocol, const std::string& service )
    {
        return protocol + "://" + service;
    }
}
