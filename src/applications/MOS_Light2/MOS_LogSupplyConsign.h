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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_LogSupplyConsign.h $
// $Author: Age $
// $Modtime: 1/04/05 11:07 $
// $Revision: 1 $
// $Workfile: MOS_LogSupplyConsign.h $
//
// *****************************************************************************

#ifndef __MOS_LogSupplyConsign_h_
#define __MOS_LogSupplyConsign_h_

#include "MOS_Types.h"
#include "MOS_ASN_Types.h"

class MOS_Agent;
class MOS_LogSupplyConsign_ListView_Item;

// =============================================================================
// Created: NLD 2004-03-18
// =============================================================================
class MOS_LogSupplyConsign
{
    MT_COPYNOTALLOWED( MOS_LogSupplyConsign );

public:
    //! @name Types
    //@{
    enum E_State
    {
        eConvoyWaitingForTransporters,
        eConvoyWaitingForCommander,
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
    MOS_LogSupplyConsign( const ASN1T_MsgLogRavitaillementTraitementCreation& asn );
    ~MOS_LogSupplyConsign();

    //! @name Accessors
    //@{
          uint           GetID              () const;
          MOS_Agent&     GetPion            () const;
          MOS_Agent*     GetAutomateLogHandling () const;
          MOS_Agent*     GetPionLogConvoying() const;
          MOS_Agent*     GetAutomateLogProvidingConvoyResources() const;
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
    MOS_Agent&        pion_;
    MOS_Agent*        pAutomateLogHandling_;
    MOS_Agent*        pPionLogConvoying_;
    MOS_Agent*        pAutomateLogProvidingConvoyResources_;
    E_State           nState_;
    T_DotationMap     dotations_;
};

#include "MOS_LogSupplyConsign.inl"

#endif // __MOS_LogSupplyConsign_h_
