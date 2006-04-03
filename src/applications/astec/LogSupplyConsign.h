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
// $Archive: /MVW_v10/Build/SDK/Light2/src/LogSupplyConsign.h $
// $Author: Age $
// $Modtime: 1/04/05 11:07 $
// $Revision: 1 $
// $Workfile: LogSupplyConsign.h $
//
// *****************************************************************************

#ifndef __LogSupplyConsign_h_
#define __LogSupplyConsign_h_

#include "Types.h"
#include "ASN_Types.h"

class Agent;
class LogSupplyConsign_ListView_Item;

// =============================================================================
// Created: NLD 2004-03-18
// =============================================================================
class LogSupplyConsign
{
    MT_COPYNOTALLOWED( LogSupplyConsign );

public:
    //! @name Types
    //@{
    enum E_State
    {
        eConvoyWaitingForTransporters,
        eConvoyForming,
        eConvoyGoingToLoadingPoint,
        eConvoyLoading,
        eConvoyGoingToUnloadingPoint,
        eConvoyUnloading,
        eConvoyGoingBackToFormingPoint,
        eFinished
    };

    struct sDotationData
    {
        uint nNbrRequested_;
        uint nNbrReserved_;
        uint nNbrConvoyed_;
    };

    typedef std::map< uint /*dot ID*/, sDotationData > T_DotationMap;
    typedef T_DotationMap::const_iterator              CIT_DotationMap;
    //@}

public:
    LogSupplyConsign( const ASN1T_MsgLogRavitaillementTraitementCreation& asn );
    ~LogSupplyConsign();

    //! @name Accessors
    //@{
          uint           GetID              () const;
          Agent&     GetPion            () const;
          Agent*     GetAutomateLogHandling () const;
          Agent*     GetPionLogConvoying() const;
          Agent*     GetAutomateLogProvidingConvoyResources() const;
          E_State        GetState           () const;
    const T_DotationMap& GetDotations       () const;
          std::string    GetStateString     () const;
    //@}

    //! @name Network events
    //@{
    void OnReceiveMsgUpdate( const ASN1T_MsgLogRavitaillementTraitementUpdate& asn );
    //@}

private:
    uint              nID_;
    Agent&        pion_;
    Agent*        pAutomateLogHandling_;
    Agent*        pPionLogConvoying_;
    Agent*        pAutomateLogProvidingConvoyResources_;
    E_State           nState_;
    T_DotationMap     dotations_;
};

#include "LogSupplyConsign.inl"

#endif // __LogSupplyConsign_h_
