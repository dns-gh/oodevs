// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __Overridable_ABC_h_
#define __Overridable_ABC_h_

// =============================================================================
/** @class  Overridable_ABC
    @brief  Overridable_ABC
*/
// Created: JSR 2010-09-09
// =============================================================================
class Overridable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Overridable_ABC() {}
    virtual ~Overridable_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual bool IsOverriden() const = 0;
    //@}

};

#endif // __Overridable_ABC_h_
