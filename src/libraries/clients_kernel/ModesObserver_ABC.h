// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ModesObserver_ABC_h_
#define __ModesObserver_ABC_h_

#include "ReadOnlyModable.h"

namespace kernel
{

// =============================================================================
/** @class  ModesObserver_ABC
    @brief  ModesObserver_ABC
*/
// Created: JSR 2012-05-24
// =============================================================================
class ModesObserver_ABC : public ReadOnlyModable
{
public:
    //! @name Constructors/Destructor
    //@{
             ModesObserver_ABC() : currentMode_( eModes_None ) {}
    virtual ~ModesObserver_ABC() {}
    //@}

    //! @name Abstract operations
    //@{
    virtual void NotifyModeChanged( E_Modes newMode ) { currentMode_ = newMode; }
    virtual E_Modes GetCurrentMode() const { return currentMode_; }
    //@}

private:
    //! @name Member data
    //@{
    E_Modes currentMode_;
    //@}
};

} //! namespace kernel

#endif // __ModesObserver_ABC_h_
