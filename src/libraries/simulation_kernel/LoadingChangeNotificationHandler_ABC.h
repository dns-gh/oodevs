// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __LoadingChangeNotificationHandler_ABC_H__
#define __LoadingChangeNotificationHandler_ABC_H__

namespace transport {

// =============================================================================
/** @class  LoadingChangeNotificationHandler_ABC
    @brief  LoadingChangeNotificationHandler_ABC
*/
// Created: MGD 2009-09-24
// =============================================================================
class LoadingChangeNotificationHandler_ABC {
public:
	         LoadingChangeNotificationHandler_ABC() {}
	virtual ~LoadingChangeNotificationHandler_ABC() {}

    virtual void NotifyIsLoadedInVab() = 0;
    virtual void NotifyIsUnLoadedInVab() = 0;
};

} // namespace transport

#endif /* __LoadingChangeNotificationHandler_ABC_H__ */
