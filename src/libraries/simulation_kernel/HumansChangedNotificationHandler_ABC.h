// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __HumansChangedNotificationHandler_ABC_H__
#define __HumansChangedNotificationHandler_ABC_H__

namespace human {

// =============================================================================
/** @class  HumansChangedNotificationHandler_ABC
@brief  HumansChangedNotificationHandler_ABC
*/
// Created: MGD 2009-09-24
// =============================================================================
class HumansChangedNotificationHandler_ABC {
public:
             HumansChangedNotificationHandler_ABC() {}
    virtual ~HumansChangedNotificationHandler_ABC() {}

    virtual void NotifyHumanHasChanged() = 0;
};

} // namespace human

#endif /* __HumansChangedNotificationHandler_ABC_H__ */
