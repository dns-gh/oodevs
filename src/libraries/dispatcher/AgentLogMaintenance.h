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

namespace sword
{
    class Id;
    class LogMaintenanceState;
    class LogMaintenanceEquipmentAvailability;
}

namespace kernel
{
    class Agent_ABC;
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
             AgentLogMaintenance( const Model_ABC& model, const kernel::Agent_ABC& agent, const sword::LogMaintenanceState& asnMsg );
    virtual ~AgentLogMaintenance();
    //@}

    //! @name Main
    //@{
    void Update(  const sword::LogMaintenanceState& asnMsg );
    void Send  ( ClientPublisher_ABC& publisher ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AgentLogMaintenance( const AgentLogMaintenance& );            //!< Copy constructor
    AgentLogMaintenance& operator=( const AgentLogMaintenance& ); //!< Assignment operator
    //@}

private:
    const Model_ABC&                          model_;
    const kernel::Agent_ABC&                  agent_;
    bool                                      bSystemEnabled_;
    std::vector< sword::Id > priorities_;
    std::vector< sword::Id > tacticalPriorities_;

    std::vector< MaintenanceEquipmentAvailability > haulersAvailability_;
    std::vector< MaintenanceEquipmentAvailability > repairersAvailability_;
};

}

#endif // __AgentLogMaintenance_h_
