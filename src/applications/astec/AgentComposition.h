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
// $Archive: /MVW_v10/Build/SDK/Light2/src/AgentComposition.h $
// $Author: Age $
// $Modtime: 6/04/05 11:48 $
// $Revision: 9 $
// $Workfile: AgentComposition.h $
//
// *****************************************************************************

#ifndef __AgentComposition_h_
#define __AgentComposition_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "Types.h"
#include "Attr_Def.h"
#include "ASN_Types.h"

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
/** @class  AgentComposition
    @brief  Resource, equipement and transport data for an agent.
*/
// Created: APE 2004-03-10
// =============================================================================
class AgentComposition
{
    MT_COPYNOTALLOWED( AgentComposition )

public:
    //! @name Constructors/Destructor
    //@{
     AgentComposition();
    ~AgentComposition();
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
    } T_LendGiven;

    typedef struct
    {
        uint nOwnerId_;
        uint nEquipment_;
        int  nQuantity_;
    } T_LendReceived;

    std::vector< T_LendGiven    > lendsGiven_;
    std::vector< T_LendReceived > lendsReceived_;

    bool bEmptyGasTank_;
};

#endif // __AgentComposition_h_
