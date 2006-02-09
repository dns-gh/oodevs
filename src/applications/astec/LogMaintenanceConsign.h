// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: NLD 2004-03-18 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/LogMaintenanceConsign.h $
// $Author: Age $
// $Modtime: 1/04/05 11:06 $
// $Revision: 1 $
// $Workfile: LogMaintenanceConsign.h $
//
// *****************************************************************************

#ifndef __LogMaintenanceConsign_h_
#define __LogMaintenanceConsign_h_

#include "Types.h"
#include "ASN_Types.h"

class Agent;

// =============================================================================
// Created: NLD 2004-03-18
// =============================================================================
class LogMaintenanceConsign
{
    MT_COPYNOTALLOWED( LogMaintenanceConsign );

public:
    //! @name Types
    //@{
    enum E_State
    {
        eGoingFrom,         // La composante se déplace seule vers le TC2
        eWaitingForCarrier,
        eCarrierGoingTo,
        eCarrierLoading,
        eCarrierGoingFrom,
        eCarrierUnloading,
        eDiagnosing,
        eSearchingForUpperLevel,
        eWaitingForParts,
        eWaitingForRepairer,
        eRepairing,
        eWaitingForGoingBackToWar,
        eGoingBackToWar,
        eFinished
    };
    //@}

public:
     LogMaintenanceConsign( const ASN1T_MsgLogMaintenanceTraitementEquipementCreation& asn );
    ~LogMaintenanceConsign();

    //! @name Accessors
    //@{
    uint        GetID() const;
    Agent&  GetPion() const;
    Agent*  GetPionLogHandling() const;
    uint        GetEquipmentTypeID() const;
    uint        GetBreakdownTypeID() const;
    E_State     GetState() const;
    std::string GetStateString() const;
    //@}

    //! @name Network events
    //@{
    void OnReceiveMsgUpdate( const ASN1T_MsgLogMaintenanceTraitementEquipementUpdate& asn );
    //@}

private:
    uint        nID_;
    Agent&  pion_;
    Agent*  pPionLogHandling_;
    uint        nEquipmentTypeID_;
    uint        nBreakdownTypeID_;
    E_State     nState_;
};

#include "LogMaintenanceConsign.inl"

#endif // __LogMaintenanceConsign_h_
