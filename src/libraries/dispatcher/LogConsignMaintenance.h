 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogConsignMaintenance_h_
#define __LogConsignMaintenance_h_

#include "game_asn/Simulation.h"
#include "SimpleEntity.h"

namespace kernel
{
    class Agent_ABC;
}

namespace dispatcher
{
    class Model;
    class ModelVisitor_ABC;
    class ClientPublisher_ABC;

// =============================================================================
/** @class  LogConsignMaintenance
    @brief  LogConsignMaintenance
*/
// Created: NLD 2006-09-19
// =============================================================================
class LogConsignMaintenance : public SimpleEntity< >
{
public:
    //! @name Constructors/Destructor
    //@{
             LogConsignMaintenance( const Model& model, const ASN1T_MsgLogMaintenanceHandlingCreation& msg );
    virtual ~LogConsignMaintenance();
    //@}

    //! @name Operations
    //@{
    using kernel::Entity_ABC::Update;
    void Update( const ASN1T_MsgLogMaintenanceHandlingUpdate& msg );

    void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    void SendDestruction( ClientPublisher_ABC& publisher ) const;

    void Accept( ModelVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    LogConsignMaintenance( const LogConsignMaintenance& );            //!< Copy constructor
    LogConsignMaintenance& operator=( const LogConsignMaintenance& ); //!< Assignement operator
    //@}

private:
    const Model&        model_;
    const kernel::Agent_ABC&        agent_;
    const unsigned long nTickCreation_;
    const unsigned int  nEquipmentType_; // XML reference - not resolved by dispatcher
    const unsigned int  nBreakdownType_; // XML reference - not resolved by dispatcher

    const kernel::Agent_ABC*               pTreatingAgent_;
    ASN1T_EnumLogMaintenanceHandlingStatus nState_;
    bool                                   bDiagnosed_;
};

}

#endif // __LogConsignMaintenance_h_
