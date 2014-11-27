// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __Objects_h_
#define __Objects_h_

#include "Object.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/TrackingResolver.h"

// =============================================================================
/** @class  Objects
    @brief  Objects
*/
// Created: JSR 2014-11-17
// =============================================================================
class Objects : public kernel::Extension_ABC
              , public tools::TrackingResolver< const Object, kernel::Object_ABC >
{
public:
    explicit Objects( kernel::Controllers& controllers );
    virtual ~Objects();
};

#endif // __Objects_h_
