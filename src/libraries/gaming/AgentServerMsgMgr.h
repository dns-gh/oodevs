// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentServerMsgMgr_h_
#define __AgentServerMsgMgr_h_

#include "clients_kernel/Types.h"
#include "game_asn/Asn.h"
#include "Publisher_ABC.h"

namespace tools
{
    class MessageDispatcher_ABC;
    class MessageSender_ABC;
}

namespace kernel
{
    class Agent_ABC;
}

class MsgRecorder;
class Model;
class Simulation;
class Profile;

//=============================================================================
// Created: NLD 2002-07-12
//=============================================================================
class AgentServerMsgMgr : public Publisher_ABC
{
   
public:
    //! @name Constructor/Destructor
    //@{
             AgentServerMsgMgr( tools::MessageDispatcher_ABC& dispatcher, tools::MessageSender_ABC& sender, Simulation& simu, Profile& profile ); 
    virtual ~AgentServerMsgMgr();
    //@}

    //! @name Operations
    //@{
    void RegisterMessageRecorder( MsgRecorder& recorder );
    void UnregisterMessageRecorder( MsgRecorder& recorder );

    void Connect( const std::string& host );
    void Disconnect();

    virtual void Send( ASN1T_MsgsClientToSim& message );
    virtual void Send( ASN1T_MsgsClientToAuthentication& message );
    virtual void Send( ASN1T_MsgsClientToReplay& message );

    void SetModel( Model& model );
    //@}

private:
    //! @name Message callbacks
    //@{
    // ASN
    void OnReceiveMsgSimToClient           ( const std::string& from, const ASN1T_MsgsSimToClient& message );
    void OnReceiveMsgAuthenticationToClient( const std::string& from, const ASN1T_MsgsAuthenticationToClient& message );
    void OnReceiveMsgReplayToClient        ( const std::string& from, const ASN1T_MsgsReplayToClient& message );

    void OnReceiveMsgUnitVisionCones                       ( const ASN1T_MsgUnitVisionCones& message );
    void OnReceiveMsgUnitInterVisibility                   ( const ASN1T_MsgUnitDetection& message );
    void OnReceiveMsgObjectInterVisibility                 ( const ASN1T_MsgObjectDetection& message );
    void OnReceiveMsgPopulationConcentrationInterVisibility( const ASN1T_MsgPopulationConcentrationDetection& message );
    void OnReceiveMsgPopulationFlowInterVisibility         ( const ASN1T_MsgPopulationFlowDetection& message );
    void OnReceiveMsgDebugDrawPoints                       ( const ASN1T_MsgDebugPoints& message );

    // Control
    void OnReceiveMsgControlPauseAck             ( const ASN1T_MsgControlPauseAck&  asnMsg );
    void OnReceiveMsgControlResumeAck            ( const ASN1T_MsgControlResumeAck& asnMsg );
    void OnReceiveMsgControlChangeTimeFactorAck  ( const ASN1T_MsgControlChangeTimeFactorAck& asnMsg );
    void OnReceiveMsgControlSkipToTickAck        ( const ASN1T_MsgControlSkipToTickAck& asnMsg );
    void OnReceiveMsgControlInformation          ( const ASN1T_MsgControlInformation& asnMsg );
    void OnReceiveMsgProfilingValues             ( const ASN1T_MsgControlProfilingInformation& message );
    void OnReceiveMsgCtrReplayInfo            ( const ASN1T_MsgControlReplayInformation& asnMsg );
    void OnReceiveMsgControlMeteoGlobalAck       ();
    void OnReceiveMsgControlMeteoLocalAck        ();
    void OnReceiveMsgCheckPointSaveBegin      ();
    void OnReceiveMsgCheckPointSaveEnd        ();
    void OnReceiveMsgCheckPointSetFrequencyAck();
    void OnReceiveMsgCheckPointSaveNowAck     ();
    void OnReceiveMsgControlBeginTick            ( const ASN1T_MsgControlBeginTick& asnMsg );
    void OnReceiveMsgControlEndTick              ( const ASN1T_MsgControlEndTick& asnMsg );
    void OnReceiveMsgAutomatCreation         ( const ASN1T_MsgAutomatCreation& asnMsg );
    void OnReceiveMsgUnitCreation             ( const ASN1T_MsgUnitCreation& asnMsg );
    void OnReceiveMsgUnitDestruction          ( const ASN1T_MsgUnitDestruction& asnMsg );
    void OnReceiveMsgChangeDiplomacy         ( const ASN1T_MsgChangeDiplomacy& asnMsg );
    void OnReceiveMsgAuthenticationResponse   ( const ASN1T_MsgAuthenticationResponse& asnMsg );

    // Profiles
    void OnReceiveMsgProfileCreation             ( const ASN1T_MsgProfileCreation& message );
    void OnReceiveMsgProfileCreationRequestAck   ( const ASN1T_MsgProfileCreationRequestAck& message );
    void OnReceiveMsgProfileDestruction          ( const ASN1T_MsgProfileDestruction& message );
    void OnReceiveMsgProfileDestructionRequestAck( const ASN1T_MsgProfileDestructionRequestAck& message );
    void OnReceiveMsgProfileUpdate               ( const ASN1T_MsgProfileUpdate& message );
    void OnReceiveMsgProfileUpdateRequestAck     ( const ASN1T_MsgProfileUpdateRequestAck& message );

    // Side/Formation/Knowledge Group creation
    void OnReveiveMsgKnowledgeGroupCreation   ( const ASN1T_MsgKnowledgeGroupCreation& asnMsg );
    void OnReveiveMsgTeamCreation             ( const ASN1T_MsgTeamCreation& asnMsg );
    void OnReveiveMsgFormationCreation        ( const ASN1T_MsgFormationCreation& asnMsg );

    // Attributes
    void OnReceiveMsgUnitAttributes    ( const ASN1T_MsgUnitAttributes& asnMsg );
    void OnReceiveMsgUnitMagicActionAck( const ASN1T_MsgUnitMagicActionAck& asnMsg, unsigned long nCtx );
    void OnReceiveMsgUnitPathFind      ( const ASN1T_MsgUnitPathFind& asnMsg );
    void OnReceiveMsgAutomatAttributes( const ASN1T_MsgAutomatAttributes& asnMsg );

    // Log maintenance
    void OnReceiveMsgLogMaintenanceHandlingCreation   ( const ASN1T_MsgLogMaintenanceHandlingCreation& asnMsg );
    void OnReceiveMsgLogMaintenanceHandlingDestruction( const ASN1T_MsgLogMaintenanceHandlingDestruction& asnMsg );
    void OnReceiveMsgLogMaintenanceHandlingUpdate     ( const ASN1T_MsgLogMaintenanceHandlingUpdate& asnMsg );
    void OnReceiveMsgLogMaintenanceState                           ( const ASN1T_MsgLogMaintenanceState&                  asnMsg );

    // Log sante
    void OnReceiveMsgLogMedicalHandlingCreation   ( const ASN1T_MsgLogMedicalHandlingCreation& asnMsg );
    void OnReceiveMsgLogMedicalHandlingDestruction( const ASN1T_MsgLogMedicalHandlingDestruction& asnMsg );
    void OnReceiveMsgLogMedicalHandlingUpdate     ( const ASN1T_MsgLogMedicalHandlingUpdate& asnMsg );
    void OnReceiveMsgLogMedicalState                       ( const ASN1T_MsgLogMedicalState&                  asnMsg );

    // Log supply
    void OnReceiveMsgLogSupplyHandlingCreation   ( const ASN1T_MsgLogSupplyHandlingCreation& asnMsg );
    void OnReceiveMsgLogSupplyHandlingDestruction( const ASN1T_MsgLogSupplyHandlingDestruction& asnMsg );
    void OnReceiveMsgLogSupplyHandlingUpdate     ( const ASN1T_MsgLogSupplyHandlingUpdate& asnMsg );
    void OnReceiveMsgLogSupplyState                 ( const ASN1T_MsgLogSupplyState&  asnMsg );
    void OnReceiveMsgLogSupplyQuotas               ( const ASN1T_MsgLogSupplyQuotas& asnMsg );
    void OnReceiveMsgLogRavitaillementChangeQuotaAck       ( const ASN1T_MsgLogSupplyChangeQuotasAck& asnMsg, unsigned long nCtx );
    void OnReceiveMsgLogSupplyPushFlowAck       ( const ASN1T_MsgLogSupplyPushFlowAck& asnMsg, unsigned long nCtx );

    // Limas / Limits
    void OnReceiveMsgLimitCreationRequestAck   ( const ASN1T_MsgLimitCreationRequestAck&    asnMsg, unsigned long nCtx );
    void OnReceiveMsgLimitUpdateRequestAck     ( const ASN1T_MsgLimitUpdateRequestAck&      asnMsg, unsigned long nCtx );
    void OnReceiveMsgLimitDestructionRequestAck( const ASN1T_MsgLimitDestructionRequestAck& asnMsg, unsigned long nCtx );
    void OnReceiveMsgLimaCreationRequestAck    ( const ASN1T_MsgLimaCreationRequestAck&    asnMsg, unsigned long nCtx );
    void OnReceiveMsgLimaUpdateRequestAck      ( const ASN1T_MsgLimaUpdateRequestAck&      asnMsg, unsigned long nCtx );
    void OnReceiveMsgLimaDestructionRequestAck ( const ASN1T_MsgLimaDestructionRequestAck& asnMsg, unsigned long nCtx );
    void OnReceiveMsgLimitCreation      ( const ASN1T_MsgLimitCreation& asnMsg );
    void OnReceiveMsgLimitUpdate        ( const ASN1T_MsgLimitUpdate& asnMsg );
    void OnReceiveMsgLimitDestruction   ( const ASN1T_MsgLimitDestruction& asnMsg );
    void OnReceiveMsgLimaCreation       ( const ASN1T_MsgLimaCreation& asnMsg );
    void OnReceiveMsgLimaUpdate         ( const ASN1T_MsgLimaUpdate& asnMsg );
    void OnReceiveMsgLimaDestruction    ( const ASN1T_MsgLimaDestruction& asnMsg );

    // Object
    void OnReceiveMsgObjectCreation      ( const ASN1T_MsgObjectCreation& asnMsg );
    void OnReceiveMsgObjectUpdate        ( const ASN1T_MsgObjectUpdate& asnMsg );
    void OnReceiveMsgObjectDestruction   ( const ASN1T_MsgObjectDestruction& asnMsg );
    void OnReceiveMsgObjectMagicActionAck( const ASN1T_MsgObjectMagicActionAck& asnMsg, unsigned long nCtx );

    // Knowledge unit
    void OnReceiveMsgUnitKnowledgeCreation   ( const ASN1T_MsgUnitKnowledgeCreation&    asnMsg );
    void OnReceiveMsgUnitKnowledgeUpdate     ( const ASN1T_MsgUnitKnowledgeUpdate&      asnMsg );
    void OnReceiveMsgUnitKnowledgeDestruction( const ASN1T_MsgUnitKnowledgeDestruction& asnMsg );

    // Knowledge object
    void OnReceiveMsgObjectKnowledgeCreation   ( const ASN1T_MsgObjectKnowledgeCreation&    asnMsg );
    void OnReceiveMsgObjectKnowledgeUpdate     ( const ASN1T_MsgObjectKnowledgeUpdate&      asnMsg );
    void OnReceiveMsgObjectKnowledgeDestruction( const ASN1T_MsgObjectKnowledgeDestruction& asnMsg );

    // Knowledge population
    void OnReceiveMsgPopulationKnowledgeCreation                ( const ASN1T_MsgPopulationKnowledgeCreation&                 asnMsg );
    void OnReceiveMsgPopulationKnowledgeUpdate                  ( const ASN1T_MsgPopulationKnowledgeUpdate&                   asnMsg );
    void OnReceiveMsgPopulationKnowledgeDestruction             ( const ASN1T_MsgPopulationKnowledgeDestruction&              asnMsg );
    void OnReceiveMsgPopulationConcentrationKnowledgeCreation   ( const ASN1T_MsgPopulationConcentrationKnowledgeCreation&    asnMsg );
    void OnReceiveMsgPopulationConcentrationKnowledgeUpdate     ( const ASN1T_MsgPopulationConcentrationKnowledgeUpdate&      asnMsg );
    void OnReceiveMsgPopulationConcentrationKnowledgeDestruction( const ASN1T_MsgPopulationConcentrationKnowledgeDestruction& asnMsg );
    void OnReceiveMsgPopulationFlowKnowledgeCreation            ( const ASN1T_MsgPopulationFlowKnowledgeCreation&             asnMsg );
    void OnReceiveMsgPopulationFlowKnowledgeUpdate              ( const ASN1T_MsgPopulationFlowKnowledgeUpdate&               asnMsg );
    void OnReceiveMsgPopulationFlowKnowledgeDestruction         ( const ASN1T_MsgPopulationFlowKnowledgeDestruction&          asnMsg );

    // Orders
    void OnReceiveMsgAutomatOrder          ( const ASN1T_MsgAutomatOrder& asnMsg ); 
    void OnReceiveMsgAutomatOrderAck       ( const ASN1T_MsgAutomatOrderAck& asnMsg, unsigned long nCtx ); 
    void OnReceiveMsgUnitOrder             ( const ASN1T_MsgUnitOrder& asnMsg ); 
    void OnReceiveMsgUnitOrderAck          ( const ASN1T_MsgUnitOrderAck& asnMsg, unsigned long nCtx );
    void OnReceiveMsgFragOrderAck          ( const ASN1T_MsgFragOrderAck& asnMsg, unsigned long nCtx ); 
    void OnReceiveMsgUnitCreationRequestAck( const ASN1T_MsgUnitCreationRequestAck& asn );

    // Automate mode
    void OnReceiveMsgSetAutomatModeAck( const ASN1T_MsgSetAutomatModeAck& asnMsg, unsigned long nCtx );
    
    // Hierachy changes
    void OnReceiveMsgUnitChangeSuperiorAck              ( const ASN1T_MsgUnitChangeSuperiorAck& asnMsg, unsigned long nCtx );
    void OnReceiveMsgUnitChangeSuperior                 ( const ASN1T_MsgUnitChangeSuperior& asnMsg );
    void OnReceiveMsgChangeDiplomacyAck                 ( const ASN1T_MsgChangeDiplomacyAck& asnMsg, unsigned long nCtx );
    void OnReceiveMsgAutomatChangeKnowledgeGroupAck( const ASN1T_MsgAutomatChangeKnowledgeGroupAck& asnMsg, unsigned long nCtx );
    void OnReceiveMsgAutomatChangeLogisticLinks     ( const ASN1T_MsgAutomatChangeLogisticLinks& asnMsg );
    void OnReceiveMsgAutomatChangeLogisticLinksAck  ( const ASN1T_MsgAutomatChangeLogisticLinksAck& asnMsg, unsigned long nCtx );

    // RC
    void OnReceiveMsgCR             ( const ASN1T_MsgReport& asnMsg );
    void OnReceiveMsgTrace          ( const ASN1T_MsgTrace& asnMsg );
    void OnReceiveMsgDecisionalState( const ASN1T_MsgDecisionalState& asnMsg );

    // Tirs
    void OnReceiveMsgStartUnitFire         ( const ASN1T_MsgStartUnitFire&       asnMsg );
    void OnReceiveMsgStopUnitFire          ( const ASN1T_MsgStopUnitFire&        asnMsg );
    void OnReceiveMsgExplosion             ( const ASN1T_MsgExplosion&           asnMsg );
    void OnReceiveMsgStartFireEffect       ( const ASN1T_MsgStartFireEffect&     asnMsg );
    void OnReceiveMsgStopFireEffect        ( const ASN1T_MsgStopFireEffect&      asnMsg );
    void OnReceiveMsgStartPopulationFire   ( const ASN1T_MsgStartPopulationFire& asnMsg );
    void OnReceiveMsgStopPopulationFire    ( const ASN1T_MsgStopPopulationFire&  asnMsg );
	
	// Population
	void OnMsgPopulationCreation				( const ASN1T_MsgPopulationCreation& asnMsg );
	void OnMsgPopulationUpdate					( const ASN1T_MsgPopulationUpdate& asnMsg ); 
	void OnMsgPopulationConcentrationCreation	( const ASN1T_MsgPopulationConcentrationCreation& asnMsg ); 
	void OnMsgPopulationConcentrationDestruction( const ASN1T_MsgPopulationConcentrationDestruction& asnMsg );
	void OnMsgPopulationConcentrationUpdate		( const ASN1T_MsgPopulationConcentrationUpdate& asnMsg );
	void OnMsgPopulationFlowCreation			( const ASN1T_MsgPopulationFlowCreation& asnMsg );
	void OnMsgPopulationFlowDestruction			( const ASN1T_MsgPopulationFlowDestruction& asnMsg ); 
	void OnMsgPopulationFlowUpdate				( const ASN1T_MsgPopulationFlowUpdate& asnMsg ); 
    void OnReceiveMsgPopulationMagicActionAck   ( const ASN1T_MsgPopulationMagicActionAck& asnMsg, unsigned long nCtx );
    void OnReceiveMsgPopulationOrderAck         ( const ASN1T_MsgPopulationOrderAck& asnMsg, unsigned long nCtx );
    void OnReceiveMsgPopulationOrder            ( const ASN1T_MsgPopulationOrder& asnMsg ); 

    // Folk
    void OnReceiveMsgFolkCreation               ( const ASN1T_MsgFolkCreation& asnMsg );
    void OnReceiveMsgFolkGraphUpdate            ( const ASN1T_MsgFolkGraphUpdate& asnMsg );
    //@}

    //! @name Helpers
    //@{
    Model& GetModel() const;
    //@}

    //! @name Copy / Assignment
    //@{
    AgentServerMsgMgr( const AgentServerMsgMgr& );
    AgentServerMsgMgr& operator=( const AgentServerMsgMgr& );
    //@}

private:
    //! @name Member data
    //@{
    tools::MessageDispatcher_ABC& dispatcher_;
    tools::MessageSender_ABC& sender_;

    std::string          host_;
    Model*               model_;
    Simulation&          simulation_;
    Profile&             profile_;
    MsgRecorder*         msgRecorder_;
    //@}
};

#endif // __AgentServerMsgMgr_h_

