// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __StubMIL_Time_ABC_h_
#define __StubMIL_Time_ABC_h_

#include "MIL_Time_ABC.h"

// =============================================================================
/** @class  StubMIL_Time_ABC
    @brief  StubMIL_Time_ABC
*/
// Created: LDC 2009-06-10
// =============================================================================
class StubMIL_Time_ABC : public MIL_Time_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             StubMIL_Time_ABC() {}
    virtual ~StubMIL_Time_ABC() {}
    //@}
    virtual unsigned int GetCurrentTick() const
    { throw; }
    virtual unsigned int GetTickDuration() const
    { throw; }
};

#endif // __StubMIL_Time_ABC_h_
