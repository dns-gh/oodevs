// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __NetworkMessageSender_ABC_H__
#define __NetworkMessageSender_ABC_H__

namespace network
{
// =============================================================================
/** @class  NetworkMessageSender_ABC
    @brief  NetworkMessageSender_ABC
*/
// Created: NLD 2010-12-27
// =============================================================================
class NetworkMessageSender_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             NetworkMessageSender_ABC() {}
    virtual ~NetworkMessageSender_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void SendChangedState() const = 0;
    virtual void SendFullState   ( unsigned int context ) const = 0;
    //@}
};

} // namespace network

#endif /* __NetworkUnitAttributesMessageSender_ABC_H__ */
