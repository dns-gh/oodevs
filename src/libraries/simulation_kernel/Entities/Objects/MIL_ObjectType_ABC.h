// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __MIL_ObjectType_ABC_h_
#define __MIL_ObjectType_ABC_h_

#include "simulation_kernel/Entities/Objects/ObjectCapacity_ABC.h"
#include <tools/Extendable.h>

// =============================================================================
/** @class  MIL_ObjectType_ABC
    @brief  MIL_ObjectType_ABC
*/
// Created: JCR 2008-06-02
// =============================================================================
class MIL_ObjectType_ABC : protected tools::Extendable< ObjectCapacity_ABC >
                         , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_ObjectType_ABC() {}
    virtual ~MIL_ObjectType_ABC() {}
    //@}

    //! @name Accessors
    //@{
    virtual unsigned int GetID() const = 0;
    virtual const std::string& GetName() const = 0;
    virtual const std::string& GetRealName() const = 0;
    virtual double GetPointSize() const = 0;
    template< typename Capacity >
    const Capacity* GetCapacity() const;
    //@}
};

template< typename Capacity >
const Capacity* MIL_ObjectType_ABC::GetCapacity() const
{
    return Retrieve< Capacity >();
}

#endif // __MIL_ObjectType_ABC_h_
