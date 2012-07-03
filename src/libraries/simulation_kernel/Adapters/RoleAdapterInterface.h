// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_ROLE_ADAPTER_INTERFACE_H
#define SWORD_ROLE_ADAPTER_INTERFACE_H

#include "MT_Tools/Role_ABC.h"

namespace sword
{
// =============================================================================
/** @class  RoleAdapterInterface
    @brief  Role adapter interface

    The only purpose of this role interface is to help with the serialization
    of a sword specific role.

    This is likely to disappear once sword legacy code goes away.
*/
// Created: MCO 2012-02-24
// =============================================================================
class RoleAdapterInterface : public tools::Role_ABC
{
public:
    //! @name Types
    //@{
    typedef RoleAdapterInterface RoleInterface;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             RoleAdapterInterface() {}
    virtual ~RoleAdapterInterface() {}
    //@}

    //! @name Serialization
    //@{
    template< typename Archive > void serialize( Archive& ar, const unsigned int )
    {
        ar & boost::serialization::base_object< tools::Role_ABC >( *this );
    }
    //@}

};

}

#endif // SWORD_ROLE_ADAPTER_INTERFACE_H
