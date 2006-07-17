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
// $Archive: /MVW_v10/Build/SDK/Light2/src/LogMedicalConsign.h $
// $Author: Age $
// $Modtime: 1/04/05 11:06 $
// $Revision: 1 $
// $Workfile: LogMedicalConsign.h $
//
// *****************************************************************************

#ifndef __LogMedicalConsign_h_
#define __LogMedicalConsign_h_

#include "Types.h"
#include "ASN_Types.h"

class Agent;
class LogMedicalConsign_ListView_Item;

// =============================================================================
// Created: NLD 2004-03-18
// =============================================================================
class LogMedicalConsign
{
    MT_COPYNOTALLOWED( LogMedicalConsign );

public:
    //! @name Types
    //@{
    enum E_State
    {
       eWaitingForEvacuation,
       eEvacuationGoingTo,
       eEvacuationLoading,
       eEvacuationWaitingForFullLoading,
       eEvacuationGoingFrom,
       eEvacuationUnloading,
       eWaitingForDiagnostic,
       eDiagnosing,
       eSearchingForSortingArea,
       eWaitingForSorting,
       eSorting,
       eSearchingForHealingArea,
       eWaitingForHealing,
       eHealing,
       eResting,
       eWaitingForCollection,
       eCollectionLoading,
       eCollectionWaitingForFullLoading,
       eCollectionGoingTo,
       eCollectionUnloading,
       eFinished
    };

    enum E_Wound
    {
        eNotWounded, 
        eKilled,
        eWoundedU1,
        eWoundedU2,
        eWoundedU3,
        eWoundedUE
    };
    //@}

public:
     LogMedicalConsign( const ASN1T_MsgLogSanteTraitementHumainCreation& asn );
    ~LogMedicalConsign();

    //! @name Accessors
    //@{
    uint        GetID() const;
    Agent&      GetPion() const;
    Agent*      GetPionLogHandling() const;
    bool        IsContaminated() const;
    bool        IsMentalDeceased() const;
    bool        IsDiagnosed     () const;

    E_State     GetState() const;
    std::string GetWoundAsString() const;
    std::string GetStateString() const;
    //@}

    //! @name Network events
    //@{
    void OnReceiveMsgUpdate( const ASN1T_MsgLogSanteTraitementHumainUpdate& asn );
    //@}

private:
    uint        nID_;
    Agent&      pion_;
    Agent*      pPionLogHandling_;
    E_Wound     wound_;
    bool        bMentalDeceased_;
    bool        bContaminated_;
    E_State     nState_;
    bool        bDiagnosed_;
};

#include "LogMedicalConsign.inl"

#endif // __LogMedicalConsign_h_
