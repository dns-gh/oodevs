 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __AgentLogMaintenance_h_
#define __AgentLogMaintenance_h_

#include "EquipmentAvailability.h"

namespace Common
{
    class EquipmentType;
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
    class Model_ABC;
    class ClientPublisher_ABC;
    class MaintenanceEquipmentAvailability;

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
             AgentLogMaintenance( const Model_ABC& model, const kernel::Agent_ABC& agent, const MsgsSimToClient::MsgLogMaintenanceState& asnMsg );
    virtual ~AgentLogMaintenance();
    //@}

    //! @name Main
    //@{
    void Update(  const MsgsSimToClient::MsgLogMaintenanceState& asnMsg );
    void Send  ( ClientPublisher_ABC& publisher ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AgentLogMaintenance( const AgentLogMaintenance& );            //!< Copy constructor
    AgentLogMaintenance& operator=( const AgentLogMaintenance& ); //!< Assignment operator
    //@}

private:
    const Model_ABC&         model_;
    const kernel::Agent_ABC& agent_;
    bool                     bSystemEnabled_;
    std::vector< Common::EquipmentType >        priorities_;
    std::vector< const kernel::Automat_ABC* > tacticalPriorities_;

    std::vector< MaintenanceEquipmentAvailability > haulersAvailability_;
    std::vector< MaintenanceEquipmentAvailability > repairersAvailability_;
};

}

#endif // __AgentLogMaintenance_h_
