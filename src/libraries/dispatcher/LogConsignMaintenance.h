 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __LogConsignMaintenance_h_
#define __LogConsignMaintenance_h_

#include "SimpleEntity.h"
#include "protocol/ClientSenders.h"

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
             LogConsignMaintenance( const Model& model, const sword::LogMaintenanceHandlingCreation& msg );
    virtual ~LogConsignMaintenance();
    //@}

    //! @name Operations
    //@{
    using kernel::Entity_ABC::Update;
    void Update( const sword::LogMaintenanceHandlingUpdate& msg );

    void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    void SendDestruction( ClientPublisher_ABC& publisher ) const;

    void Accept( kernel::ModelVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LogConsignMaintenance( const LogConsignMaintenance& );            //!< Copy constructor
    LogConsignMaintenance& operator=( const LogConsignMaintenance& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const Model& model_;
    const kernel::Agent_ABC& agent_;
    const unsigned long nTickCreation_;
    const unsigned int  nEquipmentType_; // XML reference - not resolved by dispatcher
    const unsigned int  nBreakdownType_; // XML reference - not resolved by dispatcher
    const kernel::Agent_ABC* pTreatingAgent_;
    sword::LogMaintenanceHandlingUpdate::EnumLogMaintenanceHandlingStatus nState_;
    unsigned long currentStateEndTick_;
    bool bDiagnosed_;
    //@}
};

}

#endif // __LogConsignMaintenance_h_
