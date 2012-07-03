// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_LEGACY_NULL_ROLE_ADAPTER_H
#define SWORD_LEGACY_NULL_ROLE_ADAPTER_H

#include "Adapters/RoleAdapterInterface.h"

namespace sword
{
namespace legacy
{
// =============================================================================
/** @class  NullRoleAdapter
    @brief  Null role adapter
*/
// Created: MCO 2012-02-24
// =============================================================================
class NullRoleAdapter : public RoleAdapterInterface
{
public:
    //! @name Types
    //@{
    typedef RoleAdapterInterface RoleInterface;
    //@}

    //! @name Operations
    //@{
    template< typename Archive >
    void serialize( Archive& a, const unsigned int )
    {
        a & boost::serialization::base_object< RoleAdapterInterface >( *this );
    }
    //@}
};

}
}

BOOST_CLASS_EXPORT( sword::legacy::NullRoleAdapter )

#endif // SWORD_LEGACY_NULL_ROLE_ADAPTER_H
