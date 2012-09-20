// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_CAPACITY_FACTORY_h
#define SWORD_CAPACITY_FACTORY_h

#include "Entities/Objects/CapacityFactory.h"

namespace legacy
{
    class CapacityFactory;
}

namespace sword
{
// =============================================================================
/** @class  CapacityFactory
    @brief  Capacity factory
*/
// Created: LGY 2012-09-19
// =============================================================================
class CapacityFactory : public legacy::CapacityFactory
{
public:
    //! @name Constructors/Destructor
    //@{
             CapacityFactory();
    virtual ~CapacityFactory();
    //@}
};

}

#endif // SWORD_CAPACITY_FACTORY_h
