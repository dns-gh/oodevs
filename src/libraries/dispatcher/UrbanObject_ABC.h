// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __dispatcher_UrbanObject_ABC_h_
#define __dispatcher_UrbanObject_ABC_h_

#include "Sendable.h"
#include "clients_kernel/UrbanObject_ABC.h"

namespace dispatcher
{

// =============================================================================
/** @class  UrbanObject_ABC
    @brief  UrbanObject_ABC
*/
// Created: JSR 2012-06-06
// =============================================================================
class UrbanObject_ABC : public Sendable< kernel::UrbanObject_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit UrbanObject_ABC( unsigned long id, const QString& name = "" )
        : Sendable< kernel::UrbanObject_ABC >( id, name ) {}
    virtual ~UrbanObject_ABC() {}
    //@}
};

}

#endif // __dispatcher_Object_ABC_h_
