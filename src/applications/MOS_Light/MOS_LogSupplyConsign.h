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
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_LogSupplyConsign.h $
// $Author: Nld $
// $Modtime: 10/02/05 16:05 $
// $Revision: 2 $
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
        eConvoyForming,
        eConvoyLoading,
        eConvoyGoingTo,
        eConvoyUnloading,
        eConvoyGoingFrom,
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
          MOS_Agent*     GetPionLogHandling () const;
          MOS_Agent*     GetPionLogConvoying() const;
          E_State        GetState           () const;
    const T_DotationMap& GetDotations       () const;
    //@}

    //! @name Network events
    //@{
    void OnReceiveMsgUpdate( const ASN1T_MsgLogRavitaillementTraitementUpdate& asn );
    //@}

    //! @name GUI
    //@{
    void AddListViewItem   ( MOS_LogSupplyConsign_ListView_Item& item );
    void RemoveListViewItem( MOS_LogSupplyConsign_ListView_Item& item );
    void DeleteAllItems    ();
    //@}

private:
    //! @name Types
    //@{
    typedef std::set< MOS_LogSupplyConsign_ListView_Item* > T_ListViewItemSet;
    typedef T_ListViewItemSet::const_iterator               CIT_ListViewItemSet;
    //@}

private:
    uint              nID_;
    MOS_Agent&        pion_;
    MOS_Agent*        pPionLogHandling_;
    MOS_Agent*        pPionLogConvoying_;
    E_State           nState_;
    T_DotationMap     dotations_;
    T_ListViewItemSet items_;
};

#include "MOS_LogSupplyConsign.inl"

#endif // __MOS_LogSupplyConsign_h_
