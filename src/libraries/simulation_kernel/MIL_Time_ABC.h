// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_Time_ABC_h_
#define __MIL_Time_ABC_h_

// =============================================================================
/** @class  MIL_Time_ABC
    @brief  Time definition
*/
// Created: AGE 2007-08-10
// =============================================================================
class MIL_Time_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             MIL_Time_ABC() {};
    virtual ~MIL_Time_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual unsigned int GetCurrentTick() const = 0;
    //@}
};

#endif // __MIL_Time_ABC_h_
