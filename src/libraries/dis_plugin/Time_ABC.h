// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Time_ABC_h_
#define __Time_ABC_h_

namespace plugins
{
namespace dis
{

// =============================================================================
/** @class  Time_ABC
    @brief  Time_ABC
*/
// Created: AGE 2008-03-10
// =============================================================================
class Time_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Time_ABC() {}
    virtual ~Time_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual unsigned long GetTime() const = 0;
    //@}
};

}
}

#endif // __Time_ABC_h_
