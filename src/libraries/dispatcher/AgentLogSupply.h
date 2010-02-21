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
#include "tools/Resolver.h"

namespace MsgsSimToClient
{
    class MsgLogSupplyEquimentAvailability;
    class MsgLogSupplyState;
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
class AgentLogSupply
{
public:
    //! @name Constructors/Destructor
    //@{
             AgentLogSupply( const kernel::Agent_ABC& agent, const MsgsSimToClient::MsgLogSupplyState& asnMsg );
    virtual ~AgentLogSupply();
    //@}

    //! @name Main
    //@{
    void Update(  const MsgsSimToClient::MsgLogSupplyState& asnMsg );
    void Send  ( ClientPublisher_ABC& publisher ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AgentLogSupply( const AgentLogSupply& );            //!< Copy constructor
    AgentLogSupply& operator=( const AgentLogSupply& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef EquipmentAvailability< MsgsSimToClient::MsgLogSupplyEquimentAvailability > T_Availability;
    //@}

private:
    const kernel::Agent_ABC&      agent_;
    bool                          bSystemEnabled_;
    tools::Resolver< Dotation >  stocks_;
    std::vector< T_Availability > convoyersAvailability_;
};

}

#endif // __AgentLogSupply_h_
