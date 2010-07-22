// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __dispatcher_UrbanObject_ABC_h_
#define __dispatcher_UrbanObject_ABC_h_

#include "Sendable.h"

namespace dispatcher
{
    class Team_ABC;

// =============================================================================
/** @class  Object_ABC
    @brief  Object_ABC
*/
// Created: SBO 2010-06-07
// =============================================================================
class UrbanObject_ABC : public Sendable<>
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit UrbanObject_ABC( unsigned long id, const QString& name = "" )
        : Sendable<>( id, name ) {}
    virtual ~UrbanObject_ABC() {}
    //@}
};

}

#endif // __dispatcher_Object_ABC_h_
