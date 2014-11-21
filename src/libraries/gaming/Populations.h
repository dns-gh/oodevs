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

#include "clients_kernel/Extension_ABC.h"
#include <tools/Resolver.h>

class Population;

// =============================================================================
/** @class  Populations
    @brief  Populations
*/
// Created: MCO 2014-11-21
// =============================================================================
class Populations : public kernel::Extension_ABC
                  , public tools::Resolver< const Population >
{
public:
             Populations();
    virtual ~Populations();

    void AddPopulation( const Population& population );
};

#endif // __Populations_h_
