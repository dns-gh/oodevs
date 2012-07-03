// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_ROLE_ADAPTER_H
#define SWORD_ROLE_ADAPTER_H

#include "RoleAdapterInterface.h"
#include "NetworkMessageSender_ABC.h"
#include <vector>

class MIL_AgentPion;

namespace core
{
    class Model;
    class ModelListener_ABC;
}

namespace sword
{
    class ModelSender_ABC;

// =============================================================================
/** @class  RoleAdapter
    @brief  Role adapter
*/
// Created: SLI 2012-01-16
// =============================================================================
class RoleAdapter : public RoleAdapterInterface
                  , public network::NetworkMessageSender_ABC
{
public:
    //! @name Types
    //@{
    typedef RoleAdapterInterface RoleInterface;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             RoleAdapter( MIL_AgentPion& pion, core::Model& entity );
    virtual ~RoleAdapter();
    //@}

    //! @name Operations
    //@{
    virtual void SendChangedState() const;
    virtual void SendFullState( unsigned int context ) const;

    template< typename Archive >
    void serialize( Archive& a, const unsigned int );
    //@}

private:
    //! @name Serialization
    //@{
    template< typename Archive >
    friend void save_construct_data( Archive& archive, const RoleAdapter* role, const unsigned int /*version*/ )
    {
        const MIL_AgentPion* const pion = &role->pion_;
        const core::Model* const entity = &role->entity_;
        archive << pion << entity;
    }
    template< typename Archive >
    friend void load_construct_data( Archive& archive, RoleAdapter* role, const unsigned int /*version*/ )
    {
        MIL_AgentPion* pion;
        core::Model* entity;
        archive >> pion >> entity;
        ::new( role )RoleAdapter( *pion, *entity );
    }
    //@}

private:
    //! @name Member data
    //@{
    MIL_AgentPion& pion_;
    core::Model& entity_;
    std::vector< boost::shared_ptr< network::NetworkMessageSender_ABC > > senders_;
    //@}
};

}

BOOST_CLASS_EXPORT_KEY( sword::RoleAdapter )

#endif // SWORD_ROLE_ADAPTER_H
