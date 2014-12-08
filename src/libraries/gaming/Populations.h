// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __Populations_h_
#define __Populations_h_

#include "Population.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/TrackingResolver.h"

// =============================================================================
/** @class  Populations
    @brief  Populations
*/
// Created: MCO 2014-11-21
// =============================================================================
class Populations : public kernel::Extension_ABC
                  , public tools::TrackingResolver< const Population, kernel::Population_ABC >
{
public:
    explicit Populations( kernel::Controllers& controllers );
    virtual ~Populations();
};

#endif // __Populations_h_
