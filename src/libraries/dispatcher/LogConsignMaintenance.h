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

#include "network/Simulation_Asn.h"

namespace dispatcher
{
class Dispatcher;
class Agent;
class Model;
class Publisher_ABC;

// =============================================================================
/** @class  LogConsignMaintenance
    @brief  LogConsignMaintenance
*/
// Created: NLD 2006-09-19
// =============================================================================
class LogConsignMaintenance
{
public:
    //! @name Constructors/Destructor
    //@{
     LogConsignMaintenance( Model& model, const ASN1T_MsgLogMaintenanceTraitementEquipementCreation& msg );
    ~LogConsignMaintenance();
    //@}

    //! @name Accessors
    //@{
    unsigned long GetID() const;
    //@}

    //! @name Operations
    //@{
    void Update        ( const ASN1T_MsgLogMaintenanceTraitementEquipementUpdate& msg );
    void SendCreation  ( Publisher_ABC& publisher ) const;
    void SendFullUpdate( Publisher_ABC& publisher ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    LogConsignMaintenance( const LogConsignMaintenance& );            //!< Copy constructor
    LogConsignMaintenance& operator=( const LogConsignMaintenance& ); //!< Assignement operator
    //@}

private:
          Model&        model_;
    const unsigned long nID_;
    const Agent&        agent_;
    const unsigned int  nEquipmentType_; // XML reference - no resolved by dispatcher
    const unsigned int  nBreakdownType_; // XML reference - no resolved by dispatcher

    Agent*                                 pTreatingAgent_;
    ASN1T_EnumLogMaintenanceTraitementEtat nState_;
    bool                                   bDiagnosed_;
};

}

#include "LogConsignMaintenance.inl"

#endif // __LogConsignMaintenance_h_
