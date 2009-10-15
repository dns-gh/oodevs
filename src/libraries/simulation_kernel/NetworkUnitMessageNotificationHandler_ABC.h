// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __NetworkUnitMessageNotificationHandler_ABC_H__
#define __NetworkUnitMessageNotificationHandler_ABC_H__

class NET_ASN_MsgUnitAttributes;

namespace network {

// =============================================================================
/** @class  NetworkUnitMessageNotificationHandler_ABC
@brief  NetworkUnitMessageNotificationHandler_ABC
*/
// Created: MGD 2009-10-14
// =============================================================================
class NetworkUnitMessageNotificationHandler_ABC {
public:
    NetworkUnitMessageNotificationHandler_ABC();
    virtual ~NetworkUnitMessageNotificationHandler_ABC();

    virtual void SendChangedState( NET_ASN_MsgUnitAttributes& asnMsg ) const = 0;
    virtual void SendFullState   ( NET_ASN_MsgUnitAttributes& asnMsg ) const = 0;
};

} // namespace network

#endif /* __NetworkUnitMessageNotificationHandler_ABC_H__ */
