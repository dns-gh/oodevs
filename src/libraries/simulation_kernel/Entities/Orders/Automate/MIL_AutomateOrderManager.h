//*****************************************************************************
//
// $Created: NLD 2003-04-08 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Orders/Automate/MIL_AutomateOrderManager.h $
// $Author: Nld $
// $Modtime: 11/03/05 16:27 $
// $Revision: 4 $
// $Workfile: MIL_AutomateOrderManager.h $
//
//*****************************************************************************

#ifndef __MIL_AutomateOrderManager_h_
#define __MIL_AutomateOrderManager_h_

#include "MIL.h"

#include "Network/NET_ASN_Types.h"

class MIL_AutomateMission_ABC;
class MIL_PionMission_ABC;
class MIL_AutomateMRT;
class MIL_Automate;
class MIL_AgentPion;
class MIL_PionMissionType;
class MIL_OrderConduite_ABC;
class MIL_Fuseau;
class MIL_LimaFunction;

//=============================================================================
// Created: NLD 2003-04-08
//=============================================================================
class MIL_AutomateOrderManager
{
    MT_COPYNOTALLOWED( MIL_AutomateOrderManager )

public:
     MIL_AutomateOrderManager( MIL_Automate& automate );
    ~MIL_AutomateOrderManager();

    //! @name Main
    //@{
    void Update();
    //@}

    //! @name Accessors
    //@{
          MIL_LimaOrder*   FindLima           ( const MIL_LimaFunction& function ) const;
          MIL_LimaOrder*   FindLima           ( uint nID ) const;
          MIL_AutomateMRT* GetMRT             () const;
    const MT_Vector2D&     GetDirDanger       () const;
    const MIL_Fuseau&      GetFuseau          () const;
          std::string      GetMissionName     () const;
          bool             IsNewMissionStarted() const;
    //@}

    //! @name Events
    //@{
    void                 OnReceiveRCMissionFinished();
    void                 OnReceiveMsgAutomateOrder ( const ASN1T_MsgAutomateOrder& asnMsg, MIL_MOSContextID nContext );
    void                 OnReceiveAutomateOrder    ( MIL_AutomateMission_ABC& mission );
    MIL_PionMission_ABC* MRT_CreatePionMission     ( MIL_AgentPion& pion, const MIL_PionMissionType& missionType );
    void                 MRT_Validate              ();
    MIL_PionMission_ABC* CDT_CreatePionMission     ( MIL_AgentPion& pion, const MIL_PionMissionType& missionType );
    void                 CDT_GivePionMission       ( MIL_PionMission_ABC& pionMission );
    //@}

    //! @name Conduite
    //@{
    void OnReceiveMsgOrderConduite( const ASN1T_MsgOrderConduite& asnMsg, MIL_MOSContextID nCtx );
    //@}

    //! @name Misc
    //@{
    void CancelAllOrders();
    void StopAllOrders  ();
    void Embraye        ();
    void Debraye        ();
    //@}

private:
    //! @name Tools
    //@{
           void SendMsgOrderManagement ( ASN1T_EnumOrderState nOrderState );
    static void SendMsgAutomateOrderAck( const ASN1T_MsgAutomateOrder& asnMsgAutomateOrder, ASN1T_EnumOrderErrorCode  nErrorCode, MIL_MOSContextID nContext );
           bool LaunchOrderConduite    ( MIL_OrderConduite_ABC& orderConduite );
    //@}
    
private:
    //! @name Types 
    //@{
    typedef std::set< MIL_PionMission_ABC* >    T_PionMissionSet;
    typedef T_PionMissionSet::iterator          IT_PionMissionSet;
    typedef T_PionMissionSet::const_iterator    CIT_PionMissionSet;
    //@}
    
private:
    MIL_Automate&            automate_;
    MIL_AutomateMission_ABC* pMission_;
    MIL_AutomateMission_ABC* pReplacementMission_;    
    bool                     bRCMissionFinishedReceived_;
    T_PionMissionSet         cdtPionMissionSet_; // Mission données par l'automate aux pions pendant la conduite
    bool                     bNewMissionStarted_;
};

#include "MIL_AutomateOrderManager.inl"
 
#endif // __MIL_AutomateOrderManager_h_
