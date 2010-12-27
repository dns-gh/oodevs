// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __NetworkUnitAttributesMessageSender_ABC_H__
#define __NetworkUnitAttributesMessageSender_ABC_H__

namespace client
{
    class UnitAttributes;
}

namespace network
{
// =============================================================================
/** @class  NetworkUnitAttributesMessageSender_ABC
    @brief  NetworkUnitAttributesMessageSender_ABC
*/
// Created: MGD 2009-10-14
// =============================================================================
class NetworkUnitAttributesMessageSender_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             NetworkUnitAttributesMessageSender_ABC() {}
    virtual ~NetworkUnitAttributesMessageSender_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void SendChangedState( client::UnitAttributes& asnMsg ) const = 0;
    virtual void SendFullState( client::UnitAttributes& asnMsg ) const = 0;
    //@}
};

} // namespace network

#endif /* __NetworkUnitAttributesMessageSender_ABC_H__ */
