// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __NetworkNotificationHandler_ABC_H__
#define __NetworkNotificationHandler_ABC_H__

namespace network {

// =============================================================================
/** @class  NetworkNotificationHandler_ABC
@brief  NetworkNotificationHandler_ABC
*/
// Created: MGD 2009-09-24
// =============================================================================
class NetworkNotificationHandler_ABC {
public:
             NetworkNotificationHandler_ABC() {}
    virtual ~NetworkNotificationHandler_ABC() {}

    virtual void NotifyDataHasChanged() = 0;
};

} // namespace network

#endif /* __NetworkNotificationHandler_ABC_H__ */
