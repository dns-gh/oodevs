// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ControllerObserver_ABC_h_
#define __ControllerObserver_ABC_h_

#include "Observer_ABC.h"

namespace kernel
{

// =============================================================================
/** @class  ControllerObserver_ABC
    @brief  Controller observer definition
*/
// Created: SBO 2006-11-21
// =============================================================================
class ControllerObserver_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ControllerObserver_ABC() {};
    virtual ~ControllerObserver_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual void NotifyCreated() {};
    virtual void NotifyUpdated() {};
    virtual void NotifyDeleted() {};
    //@}
};

}

#endif // __ControllerObserver_ABC_h_
