 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentLogMaintenance_h_
#define __AgentLogMaintenance_h_

#include "EquipmentAvailability.h"

namespace Common
{
    class MsgEquipmentType;
}

namespace MsgsSimToClient
{
    class MsgLogMaintenanceState;
    class MsgLogMaintenanceEquipmentAvailability;
}

namespace kernel
{
    class Agent_ABC;
    class Automat_ABC;
}

namespace dispatcher
{
    class Model;
    class ClientPublisher_ABC;

// =============================================================================
/** @class  AgentLogMaintenance
    @brief  AgentLogMaintenance
*/
// Created: NLD 2006-09-19
// =============================================================================
class AgentLogMaintenance
{
public:
    //! @name Constructors/Destructor
    //@{
             AgentLogMaintenance( const Model& model, const kernel::Agent_ABC& agent, const MsgsSimToClient::MsgLogMaintenanceState& asnMsg );
    virtual ~AgentLogMaintenance();
    //@}

    //! @name Main
    //@{
    void Update(  const MsgsSimToClient::MsgLogMaintenanceState& asnMsg );
    void Send  ( ClientPublisher_ABC& publisher ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef EquipmentAvailability< MsgsSimToClient::MsgLogMaintenanceEquipmentAvailability > T_Availability;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AgentLogMaintenance( const AgentLogMaintenance& );            //!< Copy constructor
    AgentLogMaintenance& operator=( const AgentLogMaintenance& ); //!< Assignement operator
    //@}

private:
    const Model&             model_;
    const kernel::Agent_ABC& agent_;
    bool                     bSystemEnabled_;
    std::vector< Common::MsgEquipmentType >        priorities_;
    std::vector< const kernel::Automat_ABC* > tacticalPriorities_;

    std::vector< T_Availability > haulersAvailability_;
    std::vector< T_Availability > repairersAvailability_;
};

}

#endif // __AgentLogMaintenance_h_
