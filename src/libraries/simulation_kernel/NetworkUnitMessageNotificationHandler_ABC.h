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

namespace client
{
    class UnitAttributes;
}

namespace network
{
// =============================================================================
/** @class  NetworkUnitMessageNotificationHandler_ABC
    @brief  NetworkUnitMessageNotificationHandler_ABC
*/
// Created: MGD 2009-10-14
// =============================================================================
class NetworkUnitMessageNotificationHandler_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             NetworkUnitMessageNotificationHandler_ABC() {}
    virtual ~NetworkUnitMessageNotificationHandler_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void SendChangedState( client::UnitAttributes& asnMsg ) const = 0;
    virtual void SendFullState( client::UnitAttributes& asnMsg ) const = 0;
    //@}
};

} // namespace network

#endif /* __NetworkUnitMessageNotificationHandler_ABC_H__ */
