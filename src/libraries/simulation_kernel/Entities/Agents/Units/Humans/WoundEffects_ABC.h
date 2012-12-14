// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef WoundEffects_ABC_h
#define WoundEffects_ABC_h

#include <boost/noncopyable.hpp>

class Human_ABC;

// =============================================================================
/** @class  WoundEffects_ABC
    @brief  Wound effects definition
*/
// Created: LGY 2012-12-13
// =============================================================================
class WoundEffects_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             WoundEffects_ABC() {}
    virtual ~WoundEffects_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void ApplyWound( Human_ABC& human ) const = 0;
    //@}
};

#endif // WoundEffects_ABC_h
