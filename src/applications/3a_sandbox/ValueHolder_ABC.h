// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ValueHolder_ABC_h_
#define __ValueHolder_ABC_h_

// =============================================================================
/** @class  ValueHolder_ABC
    @brief  ValueHolder_ABC
*/
// Created: AGE 2007-07-12
// =============================================================================
class ValueHolder_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    virtual ~ValueHolder_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual float GetValue() const = 0;
    //@}
};

#endif // __ValueHolder_ABC_h_
