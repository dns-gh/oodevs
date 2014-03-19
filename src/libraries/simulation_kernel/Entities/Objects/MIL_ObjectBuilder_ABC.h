// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __MIL_ObjectBuilder_ABC_h_
#define __MIL_ObjectBuilder_ABC_h_

#include <boost/noncopyable.hpp>

class MIL_ObjectType_ABC;
class MIL_Object_ABC;

// =============================================================================
/** @class  MIL_ObjectBuilder_ABC
    @brief  MIL Object builder definition
*/
// Created: JCR 2008-06-02
// =============================================================================
class MIL_ObjectBuilder_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_ObjectBuilder_ABC() {}
    virtual ~MIL_ObjectBuilder_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual const MIL_ObjectType_ABC& GetType() const = 0;
    virtual void Build( MIL_Object_ABC& object ) const = 0;
    //@}
};

#endif // __MIL_ObjectBuilder_ABC_h_
