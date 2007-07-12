// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ValueHandler_ABC_h_
#define __ValueHandler_ABC_h_

// =============================================================================
/** @class  ValueHandler_ABC
    @brief  ValueHandler_ABC
*/
// Created: AGE 2007-07-12
// =============================================================================
class ValueHandler_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    virtual ~ValueHandler_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual void Handle( float value ) = 0;
    //@}
};

#endif // __ValueHandler_ABC_h_
