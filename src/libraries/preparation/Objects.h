// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Objects_h_
#define __Objects_h_

#include "Object.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/TrackingResolver.h"
#include "clients_kernel/Serializable_ABC.h"

// =============================================================================
/** @class  Objects
    @brief  Objects
*/
// Created: JSR 2011-02-22
// =============================================================================
class Objects : public kernel::Extension_ABC
              , public tools::TrackingResolver< const Object, kernel::Object_ABC >
              , public kernel::Serializable_ABC
{
public:
    explicit Objects( kernel::Controllers& controllers );
    virtual ~Objects();

private:
    virtual void SerializeAttributes( xml::xostream& ) const;
};

#endif // __Objects_h_
