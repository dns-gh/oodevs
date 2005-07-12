//*****************************************************************************
//
// $Created: NLD 2002-07-12 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_AgentServerMsgMgr.h $
// $Author: Nld $
// $Modtime: 22/03/05 16:58 $
// $Revision: 69 $
// $Workfile: MOS_AgentServerMsgMgr.h $
//
//*****************************************************************************

#ifndef __MOS_AgentServerMsgMgr_h_
#define __MOS_AgentServerMsgMgr_h_

#include "MOS_Types.h"
#include "MOS_AgentServerMgr_ABC.h"

#include "MOS_ASN_Types.h"

class MOS_AgentServerController;
class MOS_Agent;

namespace DIN
{
	template < class T > class DIN_MessageServiceUserCbk;
}

//=============================================================================
// Created: NLD 2002-07-12
//=============================================================================
class MOS_AgentServerMsgMgr : public MOS_AgentServerMgr_ABC
{
    MT_COPYNOTALLOWED( MOS_AgentServerMsgMgr );

public:
    //-------------------------------------------------------------------------
    /** @name Messages */
    //-------------------------------------------------------------------------
    //@{
    //! SIM <- MOS
    enum 
    {
        eMsgMosSim                  = 0,
        eMsgMosSimWithContext       = 1,

        eMsgEnableUnitVisionCones   = 1000,
        eMsgDisableUnitVisionCones  = 1001,
        eMsgUnitMagicAction         = 1002,
        eMsgEnableProfiling         = 1003,
        eMsgDisableProfiling        = 1004
    };

    //! SIM -> MOS
    enum
    {
        eMsgSimMos                  = 0,
        eMsgSimMosWithContext       = 1,

        eMsgUnitVisionCones         = 1004,
        eMsgUnitTrace               = 1005,
        eMsgInit                    = 1007,
        eMsgProfilingValues         = 1008,
        eMsgUnitInterVisibility     = 1009,
        eMsgObjectInterVisibility   = 1010,
        eMsgKnowledgeGroup          = 1013,
        eMsgArmy                    = 1014,
        eMsgDebugDrawPoints         = 1015
    };
    //@}
    
public:
    MOS_AgentServerMsgMgr( MOS_AgentServerController& controller ); 
    virtual ~MOS_AgentServerMsgMgr();

    //-------------------------------------------------------------------------
    /** @name Accessors*/
    //-------------------------------------------------------------------------
    //@{
    bool IsPaused() const;
    //@}

    
    //-------------------------------------------------------------------------
    /** @name Service activation */
    //-------------------------------------------------------------------------
    //@{
    void Enable ( MOS_AgentServer& agentServer );
    void Disable( MOS_AgentServer& agentServer );
    //@}

    //-------------------------------------------------------------------------
    /** @name Messages */
    //-------------------------------------------------------------------------
    //@{
    DIN::DIN_BufferedMessage BuildMessage();

    void SendMsgMosSim           ( ASN1T_MsgsMosSim& asnMsg );
    void SendMsgMosSimWithContext( ASN1T_MsgsMosSimWithContext& asnMsg, MIL_MOSContextID nCtx );

    // Debug
    void SendMsgEnableUnitVisionCones ();
    void SendMsgDisableUnitVisionCones();
    void SendMsgEnableProfiling       ();
    void SendMsgDisableProfiling      ();
    void SendMsgUnitMagicActionDestroyComposante( const MOS_Agent& agent );
    //@}

private:
    //-------------------------------------------------------------------------
    /** @name Service callback */
    //-------------------------------------------------------------------------
    //@{
    bool OnError( DIN::DIN_Link &link, const DIN::DIN_ErrorDescription& info );
    //@}

    //-------------------------------------------------------------------------
    /** @name */
    //-------------------------------------------------------------------------
    //@{
    // Tests
    void OnReceiveMsgProfilingValues( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );

    // Debug
    void OnReceiveMsgInit                 ( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );
    void OnReceiveMsgUnitVisionCones      ( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );
    void OnReceiveMsgUnitTrace            ( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );
    void OnReceiveMsgUnitInterVisibility  ( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );
    void OnReceiveMsgObjectInterVisibility( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );
    void OnReceiveMsgKnowledgeGroup       ( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );
    void OnReceiveMsgArmy                 ( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );
    void OnReceiveMsgDebugDrawPoints      ( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );

    // ASN
    void OnReceiveMsgSimMos           ( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );
    void OnReceiveMsgSimMosWithContext( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );

    // Control
    void OnReceiveMsgCtrlPauseAck             ( const ASN1T_MsgCtrlPauseAck&  asnMsg );
    void OnReceiveMsgCtrlResumeAck            ( const ASN1T_MsgCtrlResumeAck& asnMsg );
    void OnReceiveMsgCtrlChangeTimeFactorAck  ( const ASN1T_MsgCtrlChangeTimeFactorAck& asnMsg );
    void OnReceiveMsgCtrlInfo                 ( const ASN1T_MsgCtrlInfo& asnMsg );
    void OnReceiveMsgCtrlMeteoGlobalAck       ();
    void OnReceiveMsgCtrlMeteoLocalAck        ();
    void OnReceiveMsgCheckPointSaveBegin      ();
    void OnReceiveMsgCheckPointSaveEnd        ();
    void OnReceiveMsgCheckPointLoadBegin      ();
    void OnReceiveMsgCheckPointLoadEnd        ();
    void OnReceiveMsgCheckPointSetFrequencyAck();
    void OnReceiveMsgCheckPointSaveNowAck     ();
    void OnReceiveMsgCtrlSendCurrentStateBegin();
    void OnReceiveMsgCtrlSendCurrentStateEnd  ();
    void OnReceiveMsgCtrlBeginTick            ( const ASN1T_MsgCtrlBeginTick& asnMsg );
    void OnReceiveMsgCtrlEndTick              ( const ASN1T_MsgCtrlEndTick& asnMsg );
    void OnReceiveMsgAutomateCreation         ( const ASN1T_MsgAutomateCreation& asnMsg );
    void OnReceiveMsgPionCreation             ( const ASN1T_MsgPionCreation& asnMsg );
    void OnReceiveMsgChangeDiplomatie         ( const ASN1T_MsgChangeDiplomatie& asnMsg );

    // Attributes
    void OnReceiveMsgUnitAttributes    ( const ASN1T_MsgUnitAttributes& asnMsg );
    void OnReceiveMsgUnitDotations     ( const ASN1T_MsgUnitDotations&  asnMsg );
    void OnReceiveMsgUnitMagicActionAck( const ASN1T_MsgUnitMagicActionAck& asnMsg, MIL_MOSContextID nCtx );
    void OnReceiveMsgUnitPathFind      ( const ASN1T_MsgUnitPathFind& asnMsg );

    // Limas / Limits
    void OnReceiveMsgLimitCreationAck   ( const ASN1T_MsgLimitCreationAck&    asnMsg, MIL_MOSContextID nCtx );
    void OnReceiveMsgLimitUpdateAck     ( const ASN1T_MsgLimitUpdateAck&      asnMsg, MIL_MOSContextID nCtx );
    void OnReceiveMsgLimitDestructionAck( const ASN1T_MsgLimitDestructionAck& asnMsg, MIL_MOSContextID nCtx );
    void OnReceiveMsgLimaCreationAck    ( const ASN1T_MsgLimaCreationAck&    asnMsg, MIL_MOSContextID nCtx );
    void OnReceiveMsgLimaUpdateAck      ( const ASN1T_MsgLimaUpdateAck&      asnMsg, MIL_MOSContextID nCtx );
    void OnReceiveMsgLimaDestructionAck ( const ASN1T_MsgLimaDestructionAck& asnMsg, MIL_MOSContextID nCtx );
    void OnReceiveMsgLimitCreation      ( const ASN1T_MsgLimitCreation& asnMsg );
    void OnReceiveMsgLimitDestruction   ( const ASN1T_MsgLimitDestruction& asnMsg );
    void OnReceiveMsgLimaCreation       ( const ASN1T_MsgLimaCreation& asnMsg );
    void OnReceiveMsgLimaDestruction    ( const ASN1T_MsgLimaDestruction& asnMsg );

    // Object
    void OnReceiveMsgObjectCreation      ( const ASN1T_MsgObjectCreation& asnMsg );
    void OnReceiveMsgObjectUpdate        ( const ASN1T_MsgObjectUpdate& asnMsg );
    void OnReceiveMsgObjectDestruction   ( const ASN1T_MsgObjectDestruction& asnMsg );
    void OnReceiveMsgObjectMagicActionAck( const ASN1T_MsgObjectMagicActionAck& asnMsg, MIL_MOSContextID nCtx );

    // Knowledge unit
    void OnReceiveMsgUnitKnowledgeCreation   ( const ASN1T_MsgUnitKnowledgeCreation&    asnMsg );
    void OnReceiveMsgUnitKnowledgeUpdate     ( const ASN1T_MsgUnitKnowledgeUpdate&      asnMsg );
    void OnReceiveMsgUnitKnowledgeDestruction( const ASN1T_MsgUnitKnowledgeDestruction& asnMsg );

    // Knowledge object
    void OnReceiveMsgObjectKnowledgeCreation   ( const ASN1T_MsgObjectKnowledgeCreation& asnMsg );
    void OnReceiveMsgObjectKnowledgeUpdate     ( const ASN1T_MsgObjectKnowledgeUpdate& asnMsg );
    void OnReceiveMsgObjectKnowledgeDestruction( const ASN1T_MsgObjectKnowledgeDestruction& asnMsg );

    // Log maintenance
    void OnReceiveMsgLogMaintenanceTraitementEquipementCreation   ( const ASN1T_MsgLogMaintenanceTraitementEquipementCreation& asnMsg );
    void OnReceiveMsgLogMaintenanceTraitementEquipementDestruction( const ASN1T_MsgLogMaintenanceTraitementEquipementDestruction& asnMsg );
    void OnReceiveMsgLogMaintenanceTraitementEquipementUpdate     ( const ASN1T_MsgLogMaintenanceTraitementEquipementUpdate& asnMsg );
    void OnReceiveMsgLogMaintenanceEtat                           ( const ASN1T_MsgLogMaintenanceEtat&                  asnMsg );

    // Log sante
    void OnReceiveMsgLogSanteTraitementHumainCreation   ( const ASN1T_MsgLogSanteTraitementHumainCreation& asnMsg );
    void OnReceiveMsgLogSanteTraitementHumainDestruction( const ASN1T_MsgLogSanteTraitementHumainDestruction& asnMsg );
    void OnReceiveMsgLogSanteTraitementHumainUpdate     ( const ASN1T_MsgLogSanteTraitementHumainUpdate& asnMsg );
    void OnReceiveMsgLogSanteEtat                       ( const ASN1T_MsgLogSanteEtat&                  asnMsg );

    // Log supply
    void OnReceiveMsgLogRavitaillementTraitementCreation   ( const ASN1T_MsgLogRavitaillementTraitementCreation& asnMsg );
    void OnReceiveMsgLogRavitaillementTraitementDestruction( const ASN1T_MsgLogRavitaillementTraitementDestruction& asnMsg );
    void OnReceiveMsgLogRavitaillementTraitementUpdate     ( const ASN1T_MsgLogRavitaillementTraitementUpdate& asnMsg );
    void OnReceiveMsgLogRavitaillementEtat                 ( const ASN1T_MsgLogRavitaillementEtat&  asnMsg );
    void OnReceiveMsgLogRavitaillementQuotas               ( const ASN1T_MsgLogRavitaillementQuotas& asnMsg );
    void OnReceiveMsgLogRavitaillementChangeQuotaAck       ( const ASN1T_MsgLogRavitaillementChangeQuotasAck& asnMsg, MIL_MOSContextID nCtx );
    void OnReceiveMsgLogRavitaillementPousserFlux          ( const ASN1T_MsgLogRavitaillementPousserFluxAck& asnMsg, MIL_MOSContextID nCtx );


    // Orders
    void OnReceiveMsgAutomateOrderAck    ( const ASN1T_MsgAutomateOrderAck& asnMsg    , MIL_MOSContextID nCtx ); 
    void OnReceiveMsgPionOrderAck        ( const ASN1T_MsgPionOrderAck& asnMsg        , MIL_MOSContextID nCtx ); 
    void OnReceiveMsgOrderConduiteAck    ( const ASN1T_MsgOrderConduiteAck& asnMsg, MIL_MOSContextID nCtx ); 
    void OnReceiveMsgWaitForOrderConduite( const ASN1T_MsgAttenteOrdreConduite& asnMsg );
    void OnReceiveMsgOrderManagement     ( const ASN1T_MsgOrderManagement& asnMsg );
    void OnReceiveMsgAutomateMRT         ( const ASN1T_MsgAutomateMRT& asnMsg );
    void OnReceiveMsgPionOrder           ( const ASN1T_MsgPionOrder& asnMsg ); 
    void OnReceiveMsgAutomateOrder       ( const ASN1T_MsgAutomateOrder& asnMsg );

    // Automate mode
    void OnReceiveMsgSetAutomateModeAck( const ASN1T_MsgSetAutomateModeAck& asnMsg, MIL_MOSContextID nCtx );
    
    // Automate
    void OnReceiveMsgChangeAutomateAck          ( const ASN1T_MsgChangeAutomateAck& asnMsg, MIL_MOSContextID nCtx );
    void OnReceiveMsgChangeAutomate             ( const ASN1T_MsgChangeAutomate& asnMsg );
    void OnReceiveMsgChangeDiplomacyAck         ( const ASN1T_MsgChangeDiplomatieAck& asnMsg, MIL_MOSContextID nCtx );
    void OnReceiveMsgChangeGroupeConnaissanceAck( const ASN1T_MsgChangeGroupeConnaissanceAck& asnMsg, MIL_MOSContextID nCtx );
    void OnReceiveMsgChangeLiensLogistiquesAck  ( const ASN1T_MsgChangeLiensLogistiquesAck& asnMsg, MIL_MOSContextID nCtx );

    // RC
    void OnReceiveMsgCR( const ASN1T_MsgCR& asnMsg );

    // Tirs
    void OnReceiveMsgStartDirectFire       ( const ASN1T_MsgStartDirectFire& asnMsg   );
    void OnReceiveMsgStartIndirectFire     ( const ASN1T_MsgStartIndirectFire& asnMsg );
    void OnReceiveMsgStopDirectFire        ( const ASN1T_MsgStopDirectFire& asnMsg    );
    void OnReceiveMsgStopIndirectFire      ( const ASN1T_MsgStopIndirectFire& asnMsg  );
    void OnReceiveMsgExplosion             ( const ASN1T_MsgExplosion& asnMsg );
    void OnReceiveMsgStartFireEffect       ( const ASN1T_MsgStartFireEffect& asnMsg );
    void OnReceiveMsgStopFireEffect        ( const ASN1T_MsgStopFireEffect& asnMsg );
    //@}

    //-------------------------------------------------------------------------
    /** @name Tools */
    //-------------------------------------------------------------------------
    //@{
    void PrintCtrlMsgErrorCode       ( std::stringstream& msg, const ASN1T_EnumCtrlErrorCode& nCode );
    void PrintInfoContextMsgErrorCode( std::stringstream& msg, const ASN1T_EnumInfoContextErrorCode& nCode );
    void PrintOrderMsgErrorCode      ( std::stringstream& msg, const ASN1T_EnumOrderErrorCode& nCode );
    void PrintUnitAttrMsgErrorCode   ( std::stringstream& msg, const ASN1T_EnumUnitAttrErrorCode& nCode );
    void PrintObjectMsgErrorCode     ( std::stringstream& msg, const ASN1T_EnumObjectErrorCode& nCode );
    //@}
    
private:
    DIN::DIN_MessageServiceUserCbk<MOS_AgentServerMsgMgr>*  pMessageService_;
    uint32                                                  nCurrentServerTime_; //$$$$ tmp
    bool                                                    bPaused_;
    uint                                                    nTickDuration_;

    // ASN
    ASN1OCTET           aASNEncodeBuffer_[100000];
    ASN1OCTET           aASNDecodeBuffer_[100000];
};

#ifdef MOS_USE_INLINE
#   include "MOS_AgentServerMsgMgr.inl"
#endif

#endif // __MOS_AgentServerMsgMgr_h_

