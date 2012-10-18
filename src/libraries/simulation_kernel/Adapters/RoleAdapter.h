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
#include "simulation_kernel/MIL.h"
#include <vector>

class MIL_AgentPion;

namespace core
{
    class Model;
}

namespace sword
{
    class ModelSender_ABC;
    class Sink;

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
             RoleAdapter( Sink& sink, MIL_AgentPion& pion, core::Model& entity );
    virtual ~RoleAdapter();
    //@}

    //! @name Operations
    //@{
    virtual void Finalize();
    virtual void SendChangedState() const;
    virtual void SendFullState( unsigned int context ) const;

    template< typename Archive >
    void serialize( Archive& a, const unsigned int );
    //@}

private:
    //! @name Serialization
    //@{
    INTERNAL_BOOST_SAVE_LOAD_CONSTRUCT_DATA_HEADER( sword::RoleAdapter );
    //@}

private:
    //! @name Member data
    //@{
    Sink& sink_;
    MIL_AgentPion& pion_;
    core::Model& entity_;
    std::vector< boost::shared_ptr< network::NetworkMessageSender_ABC > > senders_;
    //@}
};
}

BOOST_CLASS_EXPORT_KEY( sword::RoleAdapter )

#endif // SWORD_ROLE_ADAPTER_H
