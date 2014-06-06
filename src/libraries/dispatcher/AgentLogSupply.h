 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentLogSupply_h_
#define __AgentLogSupply_h_

#include "EquipmentAvailability.h"
#include <tools/Resolver.h>
#include <boost/noncopyable.hpp>

namespace sword
{
    class LogSupplyEquimentAvailability;
    class LogSupplyState;
}

namespace kernel
{
    class Agent_ABC;
}

namespace dispatcher
{
    class ClientPublisher_ABC;
    class Dotation;

// =============================================================================
/** @class  AgentLogSupply
    @brief  AgentLogSupply
*/
// Created: NLD 2006-09-19
// =============================================================================
class AgentLogSupply : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             AgentLogSupply( const kernel::Agent_ABC& agent, const sword::LogSupplyState& asnMsg );
    virtual ~AgentLogSupply();
    //@}

    //! @name Main
    //@{
    void Update(  const sword::LogSupplyState& asnMsg );
    void Send  ( ClientPublisher_ABC& publisher ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef EquipmentAvailability< sword::LogSupplyEquimentAvailability > T_Availability;
    //@}

private:
    const kernel::Agent_ABC&      agent_;
    bool                          bSystemEnabled_;
    tools::Resolver< Dotation >   stocks_;
    std::vector< T_Availability > convoyersAvailability_;
    std::unique_ptr< Dotation >   resourceNetworkStock_;
};

}

#endif // __AgentLogSupply_h_
