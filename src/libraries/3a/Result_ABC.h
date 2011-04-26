// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Result_ABC_h_
#define __Result_ABC_h_

#include "Slots.h"

// =============================================================================
/** @class  Result_ABC
    @brief  Result_ABC
*/
// Created: AGE 2007-09-12
// =============================================================================
class Result_ABC : public Slot_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Result_ABC() {}
    virtual ~Result_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual std::string GetName() const = 0;
    virtual void Commit( unsigned int skippedFrames, unsigned int firstTick ) const = 0;
    //@}
};

#endif // __Result_ABC_h_
