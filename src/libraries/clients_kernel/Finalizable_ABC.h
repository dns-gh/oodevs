// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __Finalizable_ABC_h_
#define __Finalizable_ABC_h_

namespace kernel
{

// =============================================================================
/** @class  Finalizable_ABC
    @brief  Finalizable_ABC
*/
// Created: JSR 2011-11-04
// =============================================================================
class Finalizable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Finalizable_ABC() {}
    virtual ~Finalizable_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Finalize() = 0;
    //@}
};

}

#endif // __Finalizable_ABC_h_
