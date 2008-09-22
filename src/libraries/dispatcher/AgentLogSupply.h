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

#include "game_asn/Simulation.h"
#include "EquipmentAvailability.h"
#include "clients_kernel/Resolver.h"

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
             AgentLogSupply( const kernel::Agent_ABC& agent, const ASN1T_MsgLogSupplyState& asnMsg );
    virtual ~AgentLogSupply();
    //@}

    //! @name Main
    //@{
    void Update(  const ASN1T_MsgLogSupplyState& asnMsg );
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
    typedef EquipmentAvailability< ASN1T_LogSupplyEquimentAvailability > T_Availability;
    //@}

private:
    const kernel::Agent_ABC&      agent_;
    bool                          bSystemEnabled_;
    kernel::Resolver< Dotation >  stocks_;
    std::vector< T_Availability > convoyersAvailability_;
};

}

#endif // __AgentLogSupply_h_
