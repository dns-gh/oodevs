// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __ComponentsChangedNotificationHandler_ABC_H__
#define __ComponentsChangedNotificationHandler_ABC_H__

namespace component {

// =============================================================================
/** @class  ComponentsChangedNotificationHandler_ABC
    @brief  ComponentsChangedNotificationHandler_ABC
*/
// Created: MGD 2009-09-24
// =============================================================================
class ComponentsChangedNotificationHandler_ABC {
public:
             ComponentsChangedNotificationHandler_ABC() {}
    virtual ~ComponentsChangedNotificationHandler_ABC() {}

	virtual void NotifyComponentHasChanged() = 0;
};

} // namespace component

#endif /* __ComponentsChangedNotificationHandler_ABC_H__ */
