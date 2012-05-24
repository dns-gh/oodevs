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

namespace kernel
{

// =============================================================================
/** @class  ModesObserver_ABC
    @brief  ModesObserver_ABC
*/
// Created: JSR 2012-05-24
// =============================================================================
class ModesObserver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ModesObserver_ABC() : currentMode_( 0 ) {}
    virtual ~ModesObserver_ABC() {}
    //@}

    //! @name Abstract operations
    //@{
    virtual void NotifyModeChanged( int newMode ) { currentMode_ = newMode; }
    int GetCurrentMode() const { return currentMode_; }
    //@}

private:
    //! @name Member data
    //@{
    int currentMode_;
    //@}
};

} //! namespace kernel

#endif // __ModesObserver_ABC_h_
