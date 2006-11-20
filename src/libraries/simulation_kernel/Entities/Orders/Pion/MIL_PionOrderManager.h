//*****************************************************************************
//
// $Created: NLD 2003-01-10 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Orders/Pion/MIL_PionOrderManager.h $
// $Author: Nld $
// $Modtime: 4/03/05 17:29 $
// $Revision: 4 $
// $Workfile: MIL_PionOrderManager.h $
//
//*****************************************************************************

#ifndef __MIL_PionOrder_h_
#define __MIL_PionOrder_h_

#include "MIL.h"

#include "Network/NET_ASN_Types.h"

class MIL_AgentPion;
class MIL_PionMission_ABC;
class MIL_OrderConduite_ABC;
class MIL_Fuseau;
class MIL_LimaFunction;

//=============================================================================
// Created: NLD 2003-01-10
//=============================================================================
class MIL_PionOrderManager
{
    MT_COPYNOTALLOWED( MIL_PionOrderManager );

public:
     MIL_PionOrderManager( MIL_AgentPion& pion );
    ~MIL_PionOrderManager();

    // @name Main
    void Update( bool bIsDead );
    //@}
 
    // @name Events 
    //@{
    void OnReceiveRCMissionFinished();
    void OnReceivePionOrder        ( MIL_PionMission_ABC& pionMission ); // Ordre de l'automate
    void OnReceiveMsgPionOrder     ( const ASN1T_MsgPionOrder& asnMsg, MIL_MOSContextID nContext );
    //@}

    //! @name Conduite
    //@{
    void OnReceiveMsgOrderConduite( const ASN1T_MsgOrderConduite& asnMsg, MIL_MOSContextID nCtx );
    void OnReceiveOrderConduite   ( DIA_Parameters& diaParams );
    //@}

    // @name Misc 
    //@{
    void StopAllOrders  ();
    void CancelAllOrders();
    bool RelievePion    ( const MIL_AgentPion& pion ); 
    bool CanRelievePion ( const MIL_AgentPion& pion ) const;
    //@}

    // @name Accessors 
    //@{
          MIL_LimaOrder* FindLima           ( const MIL_LimaFunction& function ) const;
          MIL_LimaOrder* FindLima           ( uint nID ) const;
    const T_LimaVector&  GetLimas           () const;
    const MT_Vector2D&   GetDirDanger       () const;
    const MIL_Fuseau&    GetFuseau          () const;
          std::string    GetMissionName     () const;
          bool           IsNewMissionStarted() const;
    //@}

private:
    // @name Tools
    //@{
           void SendMsgOrderManagement( ASN1T_EnumOrderState nOrderState );
    static void SendMsgPionOrderAck   ( const ASN1T_MsgPionOrder& asnMsgPionOrder, ASN1T_EnumOrderErrorCode nErrorCode, MIL_MOSContextID nContext );
           bool LaunchOrderConduite   ( MIL_OrderConduite_ABC& orderConduite );
    //@}
   
private:
    MIL_AgentPion&       pion_;        
    MIL_PionMission_ABC* pMission_;
    MIL_PionMission_ABC* pReplacementMission_;
    bool                 bRCMissionFinishedReceived_;
    bool                 bNewMissionStarted_;
};

#include "MIL_PionOrderManager.inl"

#endif // __MIL_PionOrder_h_
