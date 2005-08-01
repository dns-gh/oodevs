// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-03-10 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_AgentComposition.h $
// $Author: Age $
// $Modtime: 6/04/05 11:48 $
// $Revision: 9 $
// $Workfile: MOS_AgentComposition.h $
//
// *****************************************************************************

#ifndef __MOS_AgentComposition_h_
#define __MOS_AgentComposition_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_Types.h"
#include "MOS_Attr_Def.h"
#include "MOS_ASN_Types.h"

struct T_EquipmentState
{
    uint nNbrAvailable_;
    uint nNbrUnavailable_;
    uint nNbrReparable_;
    uint nNbrInMaintenance_;
};
    
struct T_TransportedEquipmentState
{
    uint nRessourceID_;
    uint nBreakdownID_;
    uint nNTI_;

    bool operator < ( const T_TransportedEquipmentState& rhs ) const
    {       
      return ( nRessourceID_ < rhs.nRessourceID_ ||
               ( !( rhs.nRessourceID_ < nRessourceID_ ) &&
                 ( nBreakdownID_ < rhs.nBreakdownID_ ||
                   ( !( rhs.nBreakdownID_ < nBreakdownID_ ) && nNTI_ < rhs.nNTI_ ) ) ) );
    }
};

typedef std::map< MIL_AgentID, T_EquipmentState >  T_EquipmentQty_Map;
typedef T_EquipmentQty_Map::iterator               IT_EquipmentQty_Map;
typedef T_EquipmentQty_Map::const_iterator         CIT_EquipmentQty_Map;

typedef std::map< MIL_AgentID, uint >       T_ResourceQty_Map;
typedef T_ResourceQty_Map::iterator         IT_ResourceQty_Map;
typedef T_ResourceQty_Map::const_iterator   CIT_ResourceQty_Map;

// =============================================================================
/** @class  MOS_AgentComposition
    @brief  Resource, equipement and transport data for an agent.
*/
// Created: APE 2004-03-10
// =============================================================================
class MOS_AgentComposition
{
    MT_COPYNOTALLOWED( MOS_AgentComposition )

public:
    //! @name Constructors/Destructor
    //@{
     MOS_AgentComposition();
    ~MOS_AgentComposition();
    //@}

    void SetComposition( const ASN1T_MsgUnitDotations& asnMsg );

private:
    void SetTroops        ( const ASN1T_MsgUnitDotations& asnMsg );
    void SetEquipment     ( const ASN1T_MsgUnitDotations& asnMsg );
    void SetResources     ( const ASN1T_MsgUnitDotations& asnMsg );
    void SetEquipmentLends( const ASN1T_MsgUnitDotations& asnMsg );

//$$$$ public for easy access.
public: 
    // Troops
    uint officiers_[eTroopHealthStateNbrStates];
    uint sousOfficiers_[eTroopHealthStateNbrStates];
    uint mdr_[eTroopHealthStateNbrStates];

    // Equipment
    T_EquipmentQty_Map equipment_;

    // Resources
    T_ResourceQty_Map resources_;

    // Carried troops
    uint nNbrHeliportedTroops_;

    typedef struct
    {
        uint nBorrowerId_;
        uint nEquipment_;
        int  nQuantity_;
    } T_Lend;
    std::vector< T_Lend > lends_;

    bool bEmptyGasTank_;
};


#ifdef MOS_USE_INLINE
#   include "MOS_AgentComposition.inl"
#endif

#endif // __MOS_AgentComposition_h_
