// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __surrender_SurrenderNotificationHandler_ABC_H__
#define __surrender_SurrenderNotificationHandler_ABC_H__

namespace surrender {

// =============================================================================
/** @class  SurrenderNotificationHandler_ABC
    @brief  SurrenderNotificationHandler_ABC
*/
// Created: AHC 2009-10-01
// =============================================================================
class SurrenderNotificationHandler_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             SurrenderNotificationHandler_ABC() {}
    virtual ~SurrenderNotificationHandler_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void NotifyCaptured() = 0;
    virtual void NotifyReleased() = 0;
    //@}
};

}

#endif /* __surrender_SurrenderNotificationHandler_ABC_H__ */
