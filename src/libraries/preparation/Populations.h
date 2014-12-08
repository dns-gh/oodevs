// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 MASA Group
//
// *****************************************************************************

#ifndef __Populations_h_
#define __Populations_h_

#include "Population.h"
#include "clients_kernel/Serializable_ABC.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/TrackingResolver.h"

// =============================================================================
/** @class  Populations
    @brief  Populations
*/
// Created: SBO 2006-11-09
// =============================================================================
class Populations : public kernel::Extension_ABC
                  , public kernel::Serializable_ABC
                  , public tools::TrackingResolver< Population, kernel::Population_ABC >
{
public:
    explicit Populations( kernel::Controllers& controllers );
    virtual ~Populations();

private:
    virtual void SerializeAttributes( xml::xostream& ) const;
};

#endif // __Populations_h_
