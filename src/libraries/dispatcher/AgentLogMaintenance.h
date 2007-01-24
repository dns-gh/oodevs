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

#include "network/Simulation_Asn.h"
#include "ModelRefsContainer.h"
#include "ModelsContainer.h"
#include "EquipmentAvailability.h"

namespace dispatcher
{
class Model;
class Automat;
class Agent;
class Publisher_ABC;

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
     AgentLogMaintenance( Model& model, const Agent& agent, const ASN1T_MsgLogMaintenanceEtat& asnMsg );
    ~AgentLogMaintenance();
    //@}

    //! @name Main
    //@{
    void Update(  const ASN1T_MsgLogMaintenanceEtat& asnMsg );
    void Send  ( Publisher_ABC& publisher ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< ASN1T_TypeEquipement > T_EquipmentTypeVector;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AgentLogMaintenance( const AgentLogMaintenance& );            //!< Copy constructor
    AgentLogMaintenance& operator=( const AgentLogMaintenance& ); //!< Assignement operator
    //@}

private:
    const Agent&                          agent_;
          Model&                          model_;
          bool                            bSystemEnabled_;
          T_EquipmentTypeVector           priorities_;
          ModelRefsContainer< Automat >   tacticalPriorities_;

    ModelsContainer< EquipmentAvailability< ASN1T_MaintenanceDisponibiliteMoyens > > haulersAvailability_;
    ModelsContainer< EquipmentAvailability< ASN1T_MaintenanceDisponibiliteMoyens > > repairersAvailability_;
};

}

#endif // __AgentLogMaintenance_h_
