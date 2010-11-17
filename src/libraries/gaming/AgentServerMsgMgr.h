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
#include "protocol/ServerPublisher_ABC.h"

namespace MsgsDispatcherToClient
{
    class MsgDispatcherToClient;
}

namespace MsgsSimToClient
{
    class MsgSimToClient;

    class MsgUnitVisionCones;
    class MsgUnitDetection;
    class MsgObjectDetection;
    class MsgCrowdConcentrationDetection;
    class MsgCrowdFlowDetection;
    class MsgCrowdDestruction;
    class MsgDebugPoints;
    class MsgControlDatetimeChangeAck;
    class MsgControlInformation;
    class MsgControlProfilingInformation;
    class MsgControlBeginTick;
    class MsgControlEndTick;
    class MsgControlSendCurrentStateEnd;
    class MsgControlCheckPointSaveEnd;
    class MsgAutomatCreation;
    class MsgAutomatDestruction;
    class MsgFormationCreation;
    class MsgFormationDestruction;
    class MsgUnitCreation;
    class MsgUnitDestruction;
    class MsgPartyCreation;
    class MsgUnitAttributes;
    class MsgUnitMagicActionAck;
    class MsgUnitPathFind;
    class MsgAutomatAttributes;
    class MsgLogMaintenanceHandlingCreation;
    class MsgLogMaintenanceHandlingDestruction;
    class MsgLogMaintenanceHandlingUpdate;
    class MsgLogMaintenanceState;
    class MsgLogMedicalHandlingCreation;
    class MsgLogMedicalHandlingDestruction;
    class MsgLogMedicalHandlingUpdate;
    class MsgLogMedicalState;
    class MsgLogSupplyHandlingCreation;
    class MsgLogSupplyHandlingDestruction;
    class MsgLogSupplyHandlingUpdate;
    class MsgLogSupplyState;
    class MsgLogSupplyQuotas;
    class MsgLogSupplyChangeQuotasAck;
    class MsgLogSupplyPushFlowAck;
    class MsgLogSupplyPullFlowAck;
    class MsgObjectCreation;
    class MsgObjectUpdate;
    class MsgObjectDestruction;
    class MsgObjectMagicActionAck;
    class MsgMagicActionAck;
    class MsgUnitKnowledgeCreation;
    class MsgUnitKnowledgeUpdate;
    class MsgUnitKnowledgeDestruction;
    class MsgObjectKnowledgeCreation;
    class MsgObjectKnowledgeUpdate;
    class MsgObjectKnowledgeDestruction;
    class MsgCrowdKnowledgeCreation;
    class MsgCrowdKnowledgeUpdate;
    class MsgCrowdKnowledgeDestruction;
    class MsgCrowdConcentrationKnowledgeCreation;
    class MsgCrowdConcentrationKnowledgeUpdate;
    class MsgCrowdConcentrationKnowledgeDestruction;
    class MsgCrowdFlowKnowledgeCreation;
    class MsgCrowdFlowKnowledgeUpdate;
    class MsgCrowdFlowKnowledgeDestruction;
    class MsgAutomatOrderAck;
    class MsgUnitOrderAck;
    class MsgFragOrderAck;
    class MsgUnitCreationRequestAck;
    class MsgSetAutomatModeAck;
    class MsgUnitChangeSuperiorAck;
    class MsgChangeDiplomacyAck;
    class MsgAutomatChangeSuperiorAck;
    class MsgAutomatChangeKnowledgeGroupAck;
    class MsgChangeLogisticLinksAck;
    class MsgReport;
    class MsgInvalidateReport;
    class MsgTrace;
    class MsgDecisionalState;
    class MsgStartUnitFire;
    class MsgStopUnitFire;
    class MsgExplosion;
    class MsgStartFireEffect;
    class MsgStopFireEffect;
    class MsgStartCrowdFire;
    class MsgStopCrowdFire;
    class MsgCrowdCreation;
    class MsgCrowdUpdate;
    class MsgCrowdConcentrationCreation;
    class MsgCrowdConcentrationDestruction;
    class MsgCrowdConcentrationUpdate;
    class MsgCrowdFlowCreation;
    class MsgCrowdFlowDestruction;
    class MsgCrowdFlowUpdate;
    class MsgCrowdMagicActionAck;
    class MsgCrowdOrderAck;
    class MsgFolkCreation;
    class MsgFolkGraphUpdate;
    class MsgUrbanKnowledgeCreation;
    class MsgUrbanKnowledgeUpdate;
    class MsgUrbanKnowledgeDestruction;
    class MsgKnowledgeGroupMagicActionAck;
    class MsgKnowledgeGroupCreation;
    class MsgKnowledgeGroupCreationAck;
    class MsgKnowledgeGroupUpdate;
    class MsgKnowledgeGroupUpdateAck;
    class MsgKnowledgeGroupDestruction;
    class MsgUrbanCreation;
    class MsgUrbanUpdate;
    class MsgUrbanDetection;
    class MsgControlGlobalWeather;
    class MsgControlLocalWeatherCreation;
    class MsgControlLocalWeatherDestruction;
    class MsgControlPauseAck;
    class MsgControlResumeAck;
    class MsgControlChangeTimeFactorAck;
    class MsgActionCreateFireOrderAck;
    class Listener;
}

namespace Common
{
    class MsgChangeDiplomacy;
    class MsgAutomatOrder;
    class MsgUnitOrder;
    class MsgUnitChangeSuperior;
    class MsgAutomatChangeSuperior;
    class MsgAutomatChangeKnowledgeGroup;
    class MsgChangeLogisticLinks;
    class MsgCrowdOrder;
    class MsgTextMessage;
}

namespace MsgsReplayToClient
{
    class MsgReplayToClient;
    class MsgControlSkipToTickAck;
    class MsgControlReplayInformation;
}

namespace MsgsAuthenticationToClient
{
    class MsgAuthenticationToClient;
    class MsgAuthenticationResponse;
    class MsgProfileCreation;
    class MsgProfileCreationRequestAck;
    class MsgProfileDestruction;
    class MsgProfileDestructionRequestAck;
    class MsgProfileUpdate;
    class MsgProfileUpdateRequestAck;
}

namespace MsgsMessengerToClient
{
    class MsgMessengerToClient;
    class MsgLimitCreationRequestAck;
    class MsgLimitUpdateRequestAck;
    class MsgLimitDestructionRequestAck;
    class MsgLimaCreationRequestAck;
    class MsgLimaUpdateRequestAck;
    class MsgLimaDestructionRequestAck;
    class MsgLimitCreation;
    class MsgLimitUpdate;
    class MsgLimitDestruction;
    class MsgLimaCreation;
    class MsgLimaUpdate;
    class MsgLimaDestruction;
    class MsgMessengerToClient;
    class MsgIntelligenceCreation;
    class MsgIntelligenceUpdate;
    class MsgIntelligenceDestruction;
    class MsgIntelligenceCreationRequestAck;
    class MsgIntelligenceUpdateRequestAck;
    class MsgIntelligenceDestructionRequestAck;
    class MsgShapeCreation;
    class MsgShapeUpdate;
    class MsgShapeDestruction;
    class MsgShapeCreationRequestAck;
    class MsgShapeUpdateRequestAck;
    class MsgShapeDestructionRequestAck;
    class MsgMarkerCreation;
    class MsgMarkerUpdate;
    class MsgMarkerDestruction;
}

namespace MsgsAarToClient
{
    class MsgAarToClient;
    class MsgAarInformation;
    class MsgPlotResult;
    class MsgIndicator;
}

namespace tools
{
    class MessageDispatcher_ABC;
    class MessageSender_ABC;
}

namespace kernel
{
    class Agent_ABC;
    class Logger_ABC;
}

class Model;
class Services;
class Simulation;
class Profile;
class CommandHandler;

//=============================================================================
// Created: NLD 2002-07-12
//=============================================================================
class AgentServerMsgMgr : public Publisher_ABC
{
public:
    //! @name Constructor/Destructor
    //@{
             AgentServerMsgMgr( tools::MessageDispatcher_ABC& dispatcher, tools::MessageSender_ABC& sender,
             Services& services, Simulation& simu, kernel::Logger_ABC& logger, CommandHandler& commands );
    virtual ~AgentServerMsgMgr();
    //@}

    //! @name Operations
    //@{
    void Connect( const std::string& host );
    void Disconnect();

    virtual void Send( const MsgsClientToSim::MsgClientToSim&                       wrapper );
    virtual void Send( const MsgsClientToAuthentication::MsgClientToAuthentication& wrapper );
    virtual void Send( const MsgsClientToReplay::MsgClientToReplay&                 wrapper );
    virtual void Send( const MsgsClientToAar::MsgClientToAar&                       wrapper );
    virtual void Send( const MsgsClientToMessenger::MsgClientToMessenger&           wrapper ) ;

    void SetElements( Model& model, Profile& profile );
    //@}

    //! @name Callbacks/listeners
    //@{
    void RegisterListener( boost::shared_ptr< MsgsSimToClient::Listener >& listener );
    //@}

    //! @name Statistics
    //@{
    unsigned long GetNbMessagesReceived() const;
    unsigned long GetNbMessagesSent    () const;
    //@}

private:
    //! @name Message callbacks
    //@{
    void OnReceiveMsgSimToClient           ( const std::string& from, const MsgsSimToClient::MsgSimToClient&                        wrapper );
    void OnReceiveMsgSimToClient2          ( const std::string& from, const MsgsSimToClient::MsgSimToClient&                        wrapper );
    void OnReceiveMsgAuthenticationToClient( const std::string& from, const MsgsAuthenticationToClient::MsgAuthenticationToClient&  wrapper );
    void OnReceiveMsgReplayToClient        ( const std::string& from, const MsgsReplayToClient::MsgReplayToClient&                  wrapper );
    void OnReceiveMsgAarToClient           ( const std::string& from, const MsgsAarToClient::MsgAarToClient&                        wrapper );
    void OnReceiveMsgMessengerToClient     ( const std::string& from, const MsgsMessengerToClient::MsgMessengerToClient&            wrapper );
    void OnReceiveMsgDispatcherToClient    ( const std::string& from, const MsgsDispatcherToClient::MsgDispatcherToClient&          wrapper );

    void OnReceiveMsgUnitVisionCones                       ( const MsgsSimToClient::MsgUnitVisionCones&                  message );
    void OnReceiveMsgUnitInterVisibility                   ( const MsgsSimToClient::MsgUnitDetection&                    message );
    void OnReceiveMsgObjectInterVisibility                 ( const MsgsSimToClient::MsgObjectDetection&                  message );
    void OnReceiveMsgCrowdConcentrationInterVisibility( const MsgsSimToClient::MsgCrowdConcentrationDetection& message );
    void OnReceiveMsgCrowdFlowInterVisibility         ( const MsgsSimToClient::MsgCrowdFlowDetection&          message );
    void OnReceiveMsgDebugDrawPoints                       ( const MsgsSimToClient::MsgDebugPoints&                      message );

    // Control
    void OnReceiveMsgControlPauseAck             ( const MsgsSimToClient::MsgControlPauseAck&                   message );
    void OnReceiveMsgControlResumeAck            ( const MsgsSimToClient::MsgControlResumeAck&                  message );
    void OnReceiveMsgControlChangeTimeFactorAck  ( const MsgsSimToClient::MsgControlChangeTimeFactorAck&        message );
    void OnReceiveMsgControlDatetimeChangeAck    ( const MsgsSimToClient::MsgControlDatetimeChangeAck&          message );
    void OnReceiveMsgControlSkipToTickAck        ( const MsgsReplayToClient::MsgControlSkipToTickAck&           message );
    void OnReceiveMsgControlInformation          ( const MsgsSimToClient::MsgControlInformation&                message );
    void OnReceiveMsgProfilingValues             ( const MsgsSimToClient::MsgControlProfilingInformation&       message );
    void OnReceiveMsgCtrReplayInfo               ( const MsgsReplayToClient::MsgControlReplayInformation&       message );
    void OnReceiveMsgControlMeteoGlobalAck       ();
    void OnReceiveMsgControlMeteoLocalAck        ();
    void OnReceiveMsgCheckPointSaveBegin         ();
    void OnReceiveMsgCheckPointSaveEnd           ( const MsgsSimToClient::MsgControlCheckPointSaveEnd&          message );
    void OnReceiveMsgCheckPointSetFrequencyAck   ();
    void OnReceiveMsgCheckPointSaveNowAck        ();
    void OnReceiveMsgControlBeginTick            ( const MsgsSimToClient::MsgControlBeginTick&                  message );
    void OnReceiveMsgControlEndTick              ( const MsgsSimToClient::MsgControlEndTick&                    message );
    void OnReceiveMsgAutomatCreation             ( const MsgsSimToClient::MsgAutomatCreation&                   message );
    void OnReceiveMsgAutomatDestruction          ( const MsgsSimToClient::MsgAutomatDestruction&                message );
    void OnReceiveMsgUnitCreation                ( const MsgsSimToClient::MsgUnitCreation&                      message );
    void OnReceiveMsgUnitDestruction             ( const MsgsSimToClient::MsgUnitDestruction&                   message );
    void OnReceiveMsgChangeDiplomacy             ( const Common::MsgChangeDiplomacy&                            message );
    void OnReceiveMsgAuthenticationResponse      ( const MsgsAuthenticationToClient::MsgAuthenticationResponse& message );
    void OnReceiveMsgControlMeteoGlobal          ( const MsgsSimToClient::MsgControlGlobalWeather&                message );
    void OnReceiveMsgControlMeteoLocalCreation   ( const MsgsSimToClient::MsgControlLocalWeatherCreation&         message );
    void OnReceiveMsgControlMeteoLocalDestruction( const MsgsSimToClient::MsgControlLocalWeatherDestruction&      message );
    void OnReceiveMsgSendCurrentStateEnd         ( const MsgsSimToClient::MsgControlSendCurrentStateEnd&        message );

    // Profiles
    void OnReceiveMsgProfileCreation             ( const MsgsAuthenticationToClient::MsgProfileCreation&                message );
    void OnReceiveMsgProfileCreationRequestAck   ( const MsgsAuthenticationToClient::MsgProfileCreationRequestAck&      message );
    void OnReceiveMsgProfileDestruction          ( const MsgsAuthenticationToClient::MsgProfileDestruction&             message );
    void OnReceiveMsgProfileDestructionRequestAck( const MsgsAuthenticationToClient::MsgProfileDestructionRequestAck&   message );
    void OnReceiveMsgProfileUpdate               ( const MsgsAuthenticationToClient::MsgProfileUpdate&                  message );
    void OnReceiveMsgProfileUpdateRequestAck     ( const MsgsAuthenticationToClient::MsgProfileUpdateRequestAck&        message );

    // Side/Formation/Knowledge Group creation
    void OnReceiveMsgKnowledgeGroupCreation   ( const MsgsSimToClient::MsgKnowledgeGroupCreation&    message );
    void OnReceiveMsgKnowledgeGroupDestruction( const MsgsSimToClient::MsgKnowledgeGroupDestruction& message );
    void OnReceiveMsgPartyCreation            ( const MsgsSimToClient::MsgPartyCreation&             message );
    void OnReceiveMsgFormationCreation        ( const MsgsSimToClient::MsgFormationCreation&         message );
    void OnReceiveMsgFormationDestruction     ( const MsgsSimToClient::MsgFormationDestruction&      message );

    // LTO begin
    void OnReceiveMsgKnowledgeGroupMagicActionAck( const MsgsSimToClient::MsgKnowledgeGroupMagicActionAck&   message, unsigned long nCtx );
    void OnReceiveMsgKnowledgeGroupUpdate        ( const MsgsSimToClient::MsgKnowledgeGroupUpdate&           message );
    void OnReceiveMsgKnowledgeGroupUpdateAck     ( const MsgsSimToClient::MsgKnowledgeGroupUpdateAck&        message, unsigned long nCtx );
    void OnReceiveMsgKnowledgeGroupCreationAck   ( const MsgsSimToClient::MsgKnowledgeGroupCreationAck&      message, unsigned long nCtx );
    // LTO end

    // Attributes
    void OnReceiveMsgUnitAttributes     ( const MsgsSimToClient::MsgUnitAttributes&         message );
    void OnReceiveMsgUnitMagicActionAck ( const MsgsSimToClient::MsgUnitMagicActionAck&     message, unsigned long nCtx );
    void OnReceiveMsgUnitPathFind       ( const MsgsSimToClient::MsgUnitPathFind&           message );
    void OnReceiveMsgAutomatAttributes  ( const MsgsSimToClient::MsgAutomatAttributes&      message );

    // Log maintenance
    void OnReceiveMsgLogMaintenanceHandlingCreation   ( const MsgsSimToClient::MsgLogMaintenanceHandlingCreation&       message );
    void OnReceiveMsgLogMaintenanceHandlingDestruction( const MsgsSimToClient::MsgLogMaintenanceHandlingDestruction&    message );
    void OnReceiveMsgLogMaintenanceHandlingUpdate     ( const MsgsSimToClient::MsgLogMaintenanceHandlingUpdate&         message );
    void OnReceiveMsgLogMaintenanceState              ( const MsgsSimToClient::MsgLogMaintenanceState&                  message );

    // Log sante
    void OnReceiveMsgLogMedicalHandlingCreation   ( const MsgsSimToClient::MsgLogMedicalHandlingCreation&       message );
    void OnReceiveMsgLogMedicalHandlingDestruction( const MsgsSimToClient::MsgLogMedicalHandlingDestruction&    message );
    void OnReceiveMsgLogMedicalHandlingUpdate     ( const MsgsSimToClient::MsgLogMedicalHandlingUpdate&         message );
    void OnReceiveMsgLogMedicalState              ( const MsgsSimToClient::MsgLogMedicalState&                  message );

    // Log supply
    void OnReceiveMsgLogSupplyHandlingCreation          ( const MsgsSimToClient::MsgLogSupplyHandlingCreation&      message );
    void OnReceiveMsgLogSupplyHandlingDestruction       ( const MsgsSimToClient::MsgLogSupplyHandlingDestruction&   message );
    void OnReceiveMsgLogSupplyHandlingUpdate            ( const MsgsSimToClient::MsgLogSupplyHandlingUpdate&        message );
    void OnReceiveMsgLogSupplyState                     ( const MsgsSimToClient::MsgLogSupplyState&                 message );
    void OnReceiveMsgLogSupplyQuotas                    ( const MsgsSimToClient::MsgLogSupplyQuotas&                message );
    void OnReceiveMsgLogRavitaillementChangeQuotaAck    ( const MsgsSimToClient::MsgLogSupplyChangeQuotasAck&       message, unsigned long nCtx );
    void OnReceiveMsgLogSupplyPushFlowAck               ( const MsgsSimToClient::MsgLogSupplyPushFlowAck&           message, unsigned long nCtx );
    void OnReceiveMsgLogSupplyPullFlowAck               ( const MsgsSimToClient::MsgLogSupplyPullFlowAck&           message, unsigned long nCtx);

    // Limas / Limits
    void OnReceiveMsgLimitCreationRequestAck    ( const MsgsMessengerToClient::MsgLimitCreationRequestAck&    message);
    void OnReceiveMsgLimitUpdateRequestAck      ( const MsgsMessengerToClient::MsgLimitUpdateRequestAck&      message);
    void OnReceiveMsgLimitDestructionRequestAck ( const MsgsMessengerToClient::MsgLimitDestructionRequestAck& message);
    void OnReceiveMsgLimaCreationRequestAck     ( const MsgsMessengerToClient::MsgLimaCreationRequestAck&     message);
    void OnReceiveMsgLimaUpdateRequestAck       ( const MsgsMessengerToClient::MsgLimaUpdateRequestAck&       message);
    void OnReceiveMsgLimaDestructionRequestAck  ( const MsgsMessengerToClient::MsgLimaDestructionRequestAck&  message);
    void OnReceiveMsgLimitCreation              ( const MsgsMessengerToClient::MsgLimitCreation&              message );
    void OnReceiveMsgLimitUpdate                ( const MsgsMessengerToClient::MsgLimitUpdate&                message );
    void OnReceiveMsgLimitDestruction           ( const MsgsMessengerToClient::MsgLimitDestruction&           message );
    void OnReceiveMsgLimaCreation               ( const MsgsMessengerToClient::MsgLimaCreation&               message );
    void OnReceiveMsgLimaUpdate                 ( const MsgsMessengerToClient::MsgLimaUpdate&                 message );
    void OnReceiveMsgLimaDestruction            ( const MsgsMessengerToClient::MsgLimaDestruction&            message );

    // Magic
    void OnReceiveMsgMagicActionAck( const MsgsSimToClient::MsgMagicActionAck& message, unsigned long nCtx );

    // Object
    void OnReceiveMsgObjectCreation      ( const MsgsSimToClient::MsgObjectCreation&       message );
    void OnReceiveMsgObjectUpdate        ( const MsgsSimToClient::MsgObjectUpdate&         message );
    void OnReceiveMsgObjectDestruction   ( const MsgsSimToClient::MsgObjectDestruction&    message );
    void OnReceiveMsgObjectMagicActionAck( const MsgsSimToClient::MsgObjectMagicActionAck& message, unsigned long nCtx );

    // Knowledge unit
    void OnReceiveMsgUnitKnowledgeCreation   ( const MsgsSimToClient::MsgUnitKnowledgeCreation&    message );
    void OnReceiveMsgUnitKnowledgeUpdate     ( const MsgsSimToClient::MsgUnitKnowledgeUpdate&      message );
    void OnReceiveMsgUnitKnowledgeDestruction( const MsgsSimToClient::MsgUnitKnowledgeDestruction& message );

    // Knowledge object
    void OnReceiveMsgObjectKnowledgeCreation   ( const MsgsSimToClient::MsgObjectKnowledgeCreation&    v );
    void OnReceiveMsgObjectKnowledgeUpdate     ( const MsgsSimToClient::MsgObjectKnowledgeUpdate&      message );
    void OnReceiveMsgObjectKnowledgeDestruction( const MsgsSimToClient::MsgObjectKnowledgeDestruction& message );

    // Knowledge population
    void OnReceiveMsgCrowdKnowledgeCreation                ( const MsgsSimToClient::MsgCrowdKnowledgeCreation&                 message );
    void OnReceiveMsgCrowdKnowledgeUpdate                  ( const MsgsSimToClient::MsgCrowdKnowledgeUpdate&                   message );
    void OnReceiveMsgCrowdKnowledgeDestruction             ( const MsgsSimToClient::MsgCrowdKnowledgeDestruction&              message );
    void OnReceiveMsgCrowdConcentrationKnowledgeCreation   ( const MsgsSimToClient::MsgCrowdConcentrationKnowledgeCreation&    message );
    void OnReceiveMsgCrowdConcentrationKnowledgeUpdate     ( const MsgsSimToClient::MsgCrowdConcentrationKnowledgeUpdate&      message );
    void OnReceiveMsgCrowdConcentrationKnowledgeDestruction( const MsgsSimToClient::MsgCrowdConcentrationKnowledgeDestruction& message );
    void OnReceiveMsgCrowdFlowKnowledgeCreation            ( const MsgsSimToClient::MsgCrowdFlowKnowledgeCreation&             message );
    void OnReceiveMsgCrowdFlowKnowledgeUpdate              ( const MsgsSimToClient::MsgCrowdFlowKnowledgeUpdate&               message );
    void OnReceiveMsgCrowdFlowKnowledgeDestruction         ( const MsgsSimToClient::MsgCrowdFlowKnowledgeDestruction&          message );

    // Orders
    void OnReceiveMsgAutomatOrder          ( const Common::MsgAutomatOrder&                    message );
    void OnReceiveMsgAutomatOrderAck       ( const MsgsSimToClient::MsgAutomatOrderAck&        message, unsigned long nCtx );
    void OnReceiveMsgUnitOrder             ( const Common::MsgUnitOrder&                       message );
    void OnReceiveMsgUnitOrderAck          ( const MsgsSimToClient::MsgUnitOrderAck&           message, unsigned long nCtx );
    void OnReceiveMsgFragOrderAck          ( const MsgsSimToClient::MsgFragOrderAck&           message, unsigned long nCtx );
    void OnReceiveMsgUnitCreationRequestAck( const MsgsSimToClient::MsgUnitCreationRequestAck& message );

    // Automate mode
    void OnReceiveMsgSetAutomatModeAck( const MsgsSimToClient::MsgSetAutomatModeAck& message, unsigned long nCtx );

    // Hierachy changes
    void OnReceiveMsgUnitChangeSuperiorAck          ( const MsgsSimToClient::MsgUnitChangeSuperiorAck&          message, unsigned long nCtx );
    void OnReceiveMsgUnitChangeSuperior             ( const Common::MsgUnitChangeSuperior&                      message );
    void OnReceiveMsgChangeDiplomacyAck             ( const MsgsSimToClient::MsgChangeDiplomacyAck&             message, unsigned long nCtx );
    void OnReceiveMsgAutomatChangeSuperior          ( const Common::MsgAutomatChangeSuperior&                   message );
    void OnReceiveMsgAutomatChangeSuperiorAck       ( const MsgsSimToClient::MsgAutomatChangeSuperiorAck&       message, unsigned long nCtx );
    void OnReceiveMsgAutomatChangeKnowledgeGroup    ( const Common::MsgAutomatChangeKnowledgeGroup&             message );
    void OnReceiveMsgAutomatChangeKnowledgeGroupAck ( const MsgsSimToClient::MsgAutomatChangeKnowledgeGroupAck& message, unsigned long nCtx );
    void OnReceiveMsgAutomatChangeLogisticLinks     ( const Common::MsgChangeLogisticLinks&                     message );
    void OnReceiveMsgAutomatChangeLogisticLinksAck  ( const MsgsSimToClient::MsgChangeLogisticLinksAck&         message, unsigned long nCtx );

    // RC
    void OnReceiveMsgCR              ( const MsgsSimToClient::MsgReport&           message );
    void OnReceiveMsgInvalidateReport( const MsgsSimToClient::MsgInvalidateReport& message );
    void OnReceiveMsgTrace           ( const MsgsSimToClient::MsgTrace&            message );
    void OnReceiveMsgDecisionalState ( const MsgsSimToClient::MsgDecisionalState&  message );

    // Tirs
    void OnReceiveMsgStartUnitFire         ( const MsgsSimToClient::MsgStartUnitFire&       message );
    void OnReceiveMsgStopUnitFire          ( const MsgsSimToClient::MsgStopUnitFire&        message );
    void OnReceiveMsgExplosion             ( const MsgsSimToClient::MsgExplosion&           message );
    void OnReceiveMsgStartFireEffect       ( const MsgsSimToClient::MsgStartFireEffect&     message );
    void OnReceiveMsgStopFireEffect        ( const MsgsSimToClient::MsgStopFireEffect&      message );
    void OnReceiveMsgStartCrowdFire   ( const MsgsSimToClient::MsgStartCrowdFire& message );
    void OnReceiveMsgStopCrowdFire    ( const MsgsSimToClient::MsgStopCrowdFire&  message );

    // Population
    void OnMsgCrowdCreation                ( const MsgsSimToClient::MsgCrowdCreation&                 message );
    void OnMsgCrowdDestruction             ( const MsgsSimToClient::MsgCrowdDestruction&              message );
    void OnMsgCrowdUpdate                  ( const MsgsSimToClient::MsgCrowdUpdate&                   message );
    void OnMsgCrowdConcentrationCreation   ( const MsgsSimToClient::MsgCrowdConcentrationCreation&    message );
    void OnMsgCrowdConcentrationDestruction( const MsgsSimToClient::MsgCrowdConcentrationDestruction& message );
    void OnMsgCrowdConcentrationUpdate     ( const MsgsSimToClient::MsgCrowdConcentrationUpdate&      message );
    void OnMsgCrowdFlowCreation            ( const MsgsSimToClient::MsgCrowdFlowCreation&             message );
    void OnMsgCrowdFlowDestruction         ( const MsgsSimToClient::MsgCrowdFlowDestruction&          message );
    void OnMsgCrowdFlowUpdate              ( const MsgsSimToClient::MsgCrowdFlowUpdate&               message );
    void OnReceiveMsgCrowdMagicActionAck   ( const MsgsSimToClient::MsgCrowdMagicActionAck&           message, unsigned long nCtx );
    void OnReceiveMsgCrowdOrderAck         ( const MsgsSimToClient::MsgCrowdOrderAck&                 message, unsigned long nCtx );
    void OnReceiveMsgCrowdOrder            ( const Common::MsgCrowdOrder&                             message );

    // Folk
    void OnReceiveMsgFolkCreation               ( const MsgsSimToClient::MsgFolkCreation&    message );
    void OnReceiveMsgFolkGraphUpdate            ( const MsgsSimToClient::MsgFolkGraphUpdate& message );

    // Intelligence
    void OnReceiveMsgIntelligenceCreation             ( const MsgsMessengerToClient::MsgIntelligenceCreation&              message );
    void OnReceiveMsgIntelligenceUpdate               ( const MsgsMessengerToClient::MsgIntelligenceUpdate&                message );
    void OnReceiveMsgIntelligenceDestruction          ( const MsgsMessengerToClient::MsgIntelligenceDestruction&           message );
    void OnReceiveMsgIntelligenceCreationRequestAck   ( const MsgsMessengerToClient::MsgIntelligenceCreationRequestAck&    message );
    void OnReceiveMsgIntelligenceUpdateRequestAck     ( const MsgsMessengerToClient::MsgIntelligenceUpdateRequestAck&      message );
    void OnReceiveMsgIntelligenceDestructionRequestAck( const MsgsMessengerToClient::MsgIntelligenceDestructionRequestAck& message );

    // Drawings
    void OnReceiveMsgShapeCreation             ( const MsgsMessengerToClient::MsgShapeCreation&              message );
    void OnReceiveMsgShapeUpdate               ( const MsgsMessengerToClient::MsgShapeUpdate&                message );
    void OnReceiveMsgShapeDestruction          ( const MsgsMessengerToClient::MsgShapeDestruction&           message );
    void OnReceiveMsgShapeCreationRequestAck   ( const MsgsMessengerToClient::MsgShapeCreationRequestAck&    message );
    void OnReceiveMsgShapeUpdateRequestAck     ( const MsgsMessengerToClient::MsgShapeUpdateRequestAck&      message );
    void OnReceiveMsgShapeDestructionRequestAck( const MsgsMessengerToClient::MsgShapeDestructionRequestAck& message );

    // Notes
    void OnReceiveMsgNoteCreation             ( const MsgsMessengerToClient::MsgMarkerCreation&              message );
    void OnReceiveMsgNoteUpdate               ( const MsgsMessengerToClient::MsgMarkerUpdate&                message );
    void OnReceiveMsgNoteDestruction          ( const MsgsMessengerToClient::MsgMarkerDestruction&           message );


    // Chat
    void OnReceiveMsgTextMessage( const Common::MsgTextMessage& message );

    // 3a
    void OnReceiveMsgAarInformation( const MsgsAarToClient::MsgAarInformation& message );
    void OnReceiveMsgAarResult     ( const MsgsAarToClient::MsgPlotResult&     message );
    void OnReceiveMsgAarIndicator  ( const MsgsAarToClient::MsgIndicator&      message );

    // Urban
    void OnReceiveMsgUrbanCreation            ( const MsgsSimToClient::MsgUrbanCreation&                    message );
    void OnReceiveMsgUrbanUpdate              ( const MsgsSimToClient::MsgUrbanUpdate&                      message );
    void OnReceiveMsgUrbanDetection           ( const MsgsSimToClient::MsgUrbanDetection&                   message );
    void OnReceiveMsgUrbanKnowledgeCreation   ( const MsgsSimToClient::MsgUrbanKnowledgeCreation&           message );
    void OnReceiveMsgUrbanKnowledgeUpdate     ( const MsgsSimToClient::MsgUrbanKnowledgeUpdate&             message );
    void OnReceiveMsgUrbanKnowledgeDestruction( const MsgsSimToClient::MsgUrbanKnowledgeDestruction&        message );
    //@}

    // LTO begin
    // CreateFireOrder
    void OnReceiveMsgActionCreateFireOrderAck( const MsgsSimToClient::MsgActionCreateFireOrderAck& message, unsigned long nCtx );
    // LTO end

    //! @name Helpers
    //@{
    Model& GetModel() const;
    Profile& GetProfile() const;
    //@}

    //! @name Copy / Assignment
    //@{
    AgentServerMsgMgr( const AgentServerMsgMgr& );
    AgentServerMsgMgr& operator=( const AgentServerMsgMgr& );
    //@}

private:
    //! @name Types
    //@{
    typedef std::set< boost::shared_ptr< MsgsSimToClient::Listener > > T_Listeners;
    //@}

    //! @name Member data
    //@{
    tools::MessageDispatcher_ABC& dispatcher_;
    tools::MessageSender_ABC& sender_;

    std::string          host_;
    Model*               model_;
    Profile*             profile_;
    Services&            services_;
    Simulation&          simulation_;
    kernel::Logger_ABC&  logger_;
    CommandHandler&      commands_;
    T_Listeners          listeners_;
    //@}
};

#endif // __AgentServerMsgMgr_h_

