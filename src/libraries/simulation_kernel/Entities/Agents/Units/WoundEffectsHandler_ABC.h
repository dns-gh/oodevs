// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef WoundEffectsHandler_ABC_h
#define WoundEffectsHandler_ABC_h

#include <boost/noncopyable.hpp>

class WoundEffects_ABC;

// =============================================================================
/** @class  WoundEffectsHandler_ABC
    @brief  Wound effects handler definition
*/
// Created: LGY 2012-12-13
// =============================================================================
class WoundEffectsHandler_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             WoundEffectsHandler_ABC() {}
    virtual ~WoundEffectsHandler_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void ApplyEffect( const WoundEffects_ABC& effect ) = 0;
    //@}
};

#endif // WoundEffectsHandler_ABC_h
