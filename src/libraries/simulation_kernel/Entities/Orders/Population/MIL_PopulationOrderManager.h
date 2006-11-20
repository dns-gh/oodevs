//*****************************************************************************
//
// $Created: NLD 2003-04-08 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Orders/Population/MIL_PopulationOrderManager.h $
// $Author: Nld $
// $Modtime: 11/03/05 16:27 $
// $Revision: 4 $
// $Workfile: MIL_PopulationOrderManager.h $
//
//*****************************************************************************

#ifndef __MIL_PopulationOrderManager_h_
#define __MIL_PopulationOrderManager_h_

#include "MIL.h"

#include "Network/NET_ASN_Types.h"

class MIL_PopulationMission_ABC;
class MIL_Population;
class MIL_OrderConduite_ABC;

//=============================================================================
// Created: NLD 2003-04-08
//=============================================================================
class MIL_PopulationOrderManager
{
    MT_COPYNOTALLOWED( MIL_PopulationOrderManager )

public:
     MIL_PopulationOrderManager( MIL_Population& population );
    ~MIL_PopulationOrderManager();

    //! @name Main
    //@{
    void Update();
    //@}

    //! @name Accessors
    //@{
    std::string GetMissionName     () const;
    bool        IsNewMissionStarted() const;
    //@}

    //! @name Events
    //@{
    void OnReceiveRCMissionFinished ();
    void OnReceiveMsgPopulationOrder( const ASN1T_MsgPopulationOrder& asnMsg, MIL_MOSContextID nContext );    
    //@}

    //! @name Conduite
    //@{
    void OnReceiveMsgOrderConduite( const ASN1T_MsgOrderConduite& asnMsg, MIL_MOSContextID nCtx );
    //@}

    //! @name Misc
    //@{
    void CancelAllOrders();
    void StopAllOrders  ();
    //@}

private:
    //! @name Tools
    //@{
           void SendMsgOrderManagement   ( ASN1T_EnumOrderState nOrderState );
    static void SendMsgPopulationOrderAck( const ASN1T_MsgPopulationOrder& asnMsgPopulationOrder, ASN1T_EnumOrderErrorCode nErrorCode, MIL_MOSContextID nContext );
           bool LaunchOrderConduite      ( MIL_OrderConduite_ABC& orderConduite );
    //@}
    
private:
    MIL_Population&            population_;
    MIL_PopulationMission_ABC* pMission_;
    bool                       bRCMissionFinishedReceived_;
    bool                       bNewMissionStarted_;
};

#include "MIL_PopulationOrderManager.inl"
 
#endif // __MIL_PopulationOrderManager_h_
