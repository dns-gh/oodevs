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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_LogMaintenanceConsign.h $
// $Author: Age $
// $Modtime: 1/04/05 11:06 $
// $Revision: 1 $
// $Workfile: MOS_LogMaintenanceConsign.h $
//
// *****************************************************************************

#ifndef __MOS_LogMaintenanceConsign_h_
#define __MOS_LogMaintenanceConsign_h_

#include "MOS_Types.h"
#include "MOS_ASN_Types.h"

class MOS_Agent;

// =============================================================================
// Created: NLD 2004-03-18
// =============================================================================
class MOS_LogMaintenanceConsign
{
    MT_COPYNOTALLOWED( MOS_LogMaintenanceConsign );

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
     MOS_LogMaintenanceConsign( const ASN1T_MsgLogMaintenanceTraitementEquipementCreation& asn );
    ~MOS_LogMaintenanceConsign();

    //! @name Accessors
    //@{
    uint        GetID() const;
    MOS_Agent&  GetPion() const;
    MOS_Agent*  GetPionLogHandling() const;
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
    MOS_Agent&  pion_;
    MOS_Agent*  pPionLogHandling_;
    uint        nEquipmentTypeID_;
    uint        nBreakdownTypeID_;
    E_State     nState_;
};

#include "MOS_LogMaintenanceConsign.inl"

#endif // __MOS_LogMaintenanceConsign_h_
