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

#include "clients_kernel/Extension_ABC.h"
#include <tools/Resolver.h>
#include <boost/noncopyable.hpp>

class Object;

// =============================================================================
/** @class  Objects
    @brief  Objects
*/
// Created: JSR 2014-11-17
// =============================================================================
class Objects : public kernel::Extension_ABC
              , public tools::Resolver< const Object >
              , private boost::noncopyable
{
public:
             Objects();
    virtual ~Objects();

    void AddObject( const Object& object );
};

#endif // __Objects_h_
