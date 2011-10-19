// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __Progress_ABC_h_
#define __Progress_ABC_h_

#include <boost/noncopyable.hpp>

// =============================================================================
/** @class  Progress_ABC
    @brief  Progress declaration
*/
// Created: LGY 2011-10-19
// =============================================================================
class Progress_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Progress_ABC() {}
    virtual ~Progress_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Update( unsigned int value ) = 0;
    //@}
};


#endif // __Progress_ABC_h_
