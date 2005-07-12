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
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_LogMaintenanceConsign.h $
// $Author: Nld $
// $Modtime: 5/01/05 15:09 $
// $Revision: 1 $
// $Workfile: MOS_LogMaintenanceConsign.h $
//
// *****************************************************************************

#ifndef __MOS_LogMaintenanceConsign_h_
#define __MOS_LogMaintenanceConsign_h_

#include "MOS_Types.h"
#include "MOS_ASN_Types.h"

class MOS_Agent;
class MOS_LogMaintenanceConsign_ListView_Item;

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
    //@}

    //! @name Network events
    //@{
    void OnReceiveMsgUpdate( const ASN1T_MsgLogMaintenanceTraitementEquipementUpdate& asn );
    //@}

    //! @name GUI
    //@{
    void AddListViewItem   ( MOS_LogMaintenanceConsign_ListView_Item& item );
    void RemoveListViewItem( MOS_LogMaintenanceConsign_ListView_Item& item );
    void DeleteAllItems    ();
    //@}

private:
    //! @name Types
    //@{
    typedef std::set< MOS_LogMaintenanceConsign_ListView_Item* > T_ListViewItemSet;
    typedef T_ListViewItemSet::const_iterator                    CIT_ListViewItemSet;
    //@}

private:
    uint        nID_;
    MOS_Agent&  pion_;
    MOS_Agent*  pPionLogHandling_;
    uint        nEquipmentTypeID_;
    uint        nBreakdownTypeID_;
    E_State     nState_;

    T_ListViewItemSet items_;
};

#include "MOS_LogMaintenanceConsign.inl"

#endif // __MOS_LogMaintenanceConsign_h_
