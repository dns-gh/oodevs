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
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_LogMedicalConsign.h $
// $Author: Nld $
// $Modtime: 12/01/05 15:44 $
// $Revision: 1 $
// $Workfile: MOS_LogMedicalConsign.h $
//
// *****************************************************************************

#ifndef __MOS_LogMedicalConsign_h_
#define __MOS_LogMedicalConsign_h_

#include "MOS_Types.h"
#include "MOS_ASN_Types.h"

class MOS_Agent;
class MOS_LogMedicalConsign_ListView_Item;

// =============================================================================
// Created: NLD 2004-03-18
// =============================================================================
class MOS_LogMedicalConsign
{
    MT_COPYNOTALLOWED( MOS_LogMedicalConsign );

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
     MOS_LogMedicalConsign( const ASN1T_MsgLogSanteTraitementHumainCreation& asn );
    ~MOS_LogMedicalConsign();

    //! @name Accessors
    //@{
    uint        GetID() const;
    MOS_Agent&  GetPion() const;
    MOS_Agent*  GetPionLogHandling() const;
    bool        IsContaminated() const;
    bool        IsMentalDeceased() const;

    E_State     GetState() const;
    std::string GetWoundAsString() const;
    //@}

    //! @name Network events
    //@{
    void OnReceiveMsgUpdate( const ASN1T_MsgLogSanteTraitementHumainUpdate& asn );
    //@}

    //! @name GUI
    //@{
    void AddListViewItem   ( MOS_LogMedicalConsign_ListView_Item& item );
    void RemoveListViewItem( MOS_LogMedicalConsign_ListView_Item& item );
    void DeleteAllItems    ();
    //@}

private:
    //! @name Types
    //@{
    typedef std::set< MOS_LogMedicalConsign_ListView_Item* > T_ListViewItemSet;
    typedef T_ListViewItemSet::const_iterator                CIT_ListViewItemSet;
    //@}

private:
    uint        nID_;
    MOS_Agent&  pion_;
    MOS_Agent*  pPionLogHandling_;
    E_Wound     wound_;
    bool        bMentalDeceased_;
    bool        bContaminated_;
    E_State     nState_;

    T_ListViewItemSet items_;
};

#include "MOS_LogMedicalConsign.inl"

#endif // __MOS_LogMedicalConsign_h_
