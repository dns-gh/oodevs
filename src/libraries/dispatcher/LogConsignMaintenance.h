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

#include "SimpleEntity.h"

namespace Common
{
    enum EnumLogMaintenanceHandlingStatus;
}

namespace MsgsSimToClient
{
    class MsgLogMaintenanceHandlingCreation;
    class MsgLogMaintenanceHandlingUpdate;
}

namespace kernel
{
    class Agent_ABC;
    class ModelVisitor_ABC;
}

namespace dispatcher
{
    class Model;
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
             LogConsignMaintenance( const Model& model, const MsgsSimToClient::MsgLogMaintenanceHandlingCreation& msg );
    virtual ~LogConsignMaintenance();
    //@}

    //! @name Operations
    //@{
    using kernel::Entity_ABC::Update;
    void Update( const MsgsSimToClient::MsgLogMaintenanceHandlingUpdate& msg );

    void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    void SendDestruction( ClientPublisher_ABC& publisher ) const;

    void Accept( kernel::ModelVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    LogConsignMaintenance( const LogConsignMaintenance& );            //!< Copy constructor
    LogConsignMaintenance& operator=( const LogConsignMaintenance& ); //!< Assignment operator
    //@}

private:
    const Model&        model_;
    const kernel::Agent_ABC&        agent_;
    const unsigned long nTickCreation_;
    const unsigned int  nEquipmentType_; // XML reference - not resolved by dispatcher
    const unsigned int  nBreakdownType_; // XML reference - not resolved by dispatcher

    const kernel::Agent_ABC*               pTreatingAgent_;
    Common::EnumLogMaintenanceHandlingStatus nState_;
    bool                                   bDiagnosed_;
};

}

#endif // __LogConsignMaintenance_h_
