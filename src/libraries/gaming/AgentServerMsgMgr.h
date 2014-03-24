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

namespace sword
{
    class Tasker;
    class AarInformation;
    class AarToClient;
    class AuthenticationResponse;
    class AuthenticationToClient;
    class AutomatAttributes;
    class AutomatChangeKnowledgeGroup;
    class AutomatChangeKnowledgeGroupAck;
    class AutomatChangeSuperior;
    class AutomatChangeSuperiorAck;
    class AutomatCreation;
    class AutomatDestruction;
    class AutomatOrder;
    class BurningCellRequestAck;
    class ChangeDiplomacy;
    class ChangeDiplomacyAck;
    class ChangeLogisticLinks;
    class ChangeLogisticLinksAck;
    class ControlBeginTick;
    class ControlChangeTimeFactorAck;
    class ControlCheckPointSaveEnd;
    class ControlDateTimeChangeAck;
    class ControlEndTick;
    class ControlGlobalWeather;
    class ControlInformation;
    class ControlLocalWeatherCreation;
    class ControlLocalWeatherDestruction;
    class ControlPauseAck;
    class ControlProfilingInformation;
    class ControlReplayInformation;
    class ControlResumeAck;
    class ControlSendCurrentStateEnd;
    class ControlSkipToTickAck;
    class CrowdConcentrationCreation;
    class CrowdConcentrationDestruction;
    class CrowdConcentrationDetection;
    class CrowdConcentrationKnowledgeCreation;
    class CrowdConcentrationKnowledgeDestruction;
    class CrowdConcentrationKnowledgeUpdate;
    class CrowdConcentrationUpdate;
    class CrowdCreation;
    class CrowdDestruction;
    class CrowdFlowCreation;
    class CrowdFlowDestruction;
    class CrowdFlowDetection;
    class CrowdFlowKnowledgeCreation;
    class CrowdFlowKnowledgeDestruction;
    class CrowdFlowKnowledgeUpdate;
    class CrowdFlowUpdate;
    class CrowdKnowledgeCreation;
    class CrowdKnowledgeDestruction;
    class CrowdKnowledgeUpdate;
    class CrowdMagicActionAck;
    class CrowdOrder;
    class CrowdUpdate;
    class DebugPoints;
    class DecisionalState;
    class DispatcherToClient;
    class Explosion;
    class FolkCreation;
    class FolkGraphUpdate;
    class FormationChangeSuperior;
    class FormationCreation;
    class FormationDestruction;
    class FormationUpdate;
    class FragOrder;
    class FragOrderAck;
    class Indicator;
    class IndirectFirePerception;
    class InvalidateReport;
    class KnowledgeGroupCreation;
    class KnowledgeGroupCreationAck;
    class KnowledgeGroupDestruction;
    class KnowledgeGroupMagicActionAck;
    class KnowledgeGroupUpdate;
    class KnowledgeGroupUpdateAck;
    class LimitCreation;
    class LimitCreationRequestAck;
    class LimitDestruction;
    class LimitDestructionRequestAck;
    class LimitUpdate;
    class LimitUpdateRequestAck;
    class Listener;
    class LogisticHistoryAck;
    class LogMaintenanceHandlingCreation;
    class LogMaintenanceHandlingDestruction;
    class LogMaintenanceHandlingUpdate;
    class LogMaintenanceState;
    class LogMedicalHandlingCreation;
    class LogMedicalHandlingDestruction;
    class LogMedicalHandlingUpdate;
    class LogMedicalState;
    class LogFuneralHandlingCreation;
    class LogFuneralHandlingDestruction;
    class LogFuneralHandlingUpdate;
    class LogSupplyChangeQuotasAck;
    class LogSupplyHandlingCreation;
    class LogSupplyHandlingDestruction;
    class LogSupplyHandlingUpdate;
    class LogSupplyPullFlowAck;
    class LogSupplyPushFlowAck;
    class LogSupplyQuotas;
    class LogSupplyState;
    class MagicActionAck;
    class MarkerCreation;
    class MarkerDestruction;
    class MarkerUpdate;
    class MessengerToClient;
    class MessengerToClient;
    class NewDataChunkNotification;
    class ObjectCreation;
    class ObjectDestruction;
    class ObjectDetection;
    class ObjectKnowledgeCreation;
    class ObjectKnowledgeDestruction;
    class ObjectKnowledgeUpdate;
    class ObjectMagicActionAck;
    class ObjectUpdate;
    class PartyCreation;
    class PhaseLineCreation;
    class PhaseLineCreationAck;
    class PhaseLineDestruction;
    class PhaseLineDestructionRequestAck;
    class PhaseLineUpdate;
    class PhaseLineUpdateRequestAck;
    class PlotResult;
    class PopulationCreation;
    class PopulationUpdate;
    class ProfileCreation;
    class ProfileCreationRequestAck;
    class ProfileDestruction;
    class ProfileDestructionRequestAck;
    class ProfileUpdate;
    class ProfileUpdateRequestAck;
    class ReplayToClient;
    class Report;
    class SetAutomatModeAck;
    class ShapeCreation;
    class ShapeCreationRequestAck;
    class ShapeDestruction;
    class ShapeDestructionRequestAck;
    class ShapeUpdate;
    class ShapeUpdateRequestAck;
    class SimToClient;
    class StartCrowdFire;
    class StartFireEffect;
    class StartUnitFire;
    class StartUnitFireDetection;
    class StopCrowdFire;
    class StopFireEffect;
    class StopUnitFire;
    class StopUnitFireDetection;
    class TaskCreationRequestAck;
    class TextMessage;
    class TimeTableRequestAck;
    class TimeTable;
    class Trace;
    class UnitAttributes;
    class UnitChangeSuperior;
    class UnitChangeSuperiorAck;
    class UnitCreation;
    class UnitCreationRequestAck;
    class UnitDestruction;
    class UnitDetection;
    class UnitKnowledgeCreation;
    class UnitKnowledgeDestruction;
    class UnitKnowledgeUpdate;
    class UnitMagicActionAck;
    class UnitOrder;
    class UnitPathFind;
    class UnitVisionCones;
    class UrbanCreation;
    class UrbanDetection;
    class UrbanKnowledgeCreation;
    class UrbanKnowledgeDestruction;
    class UrbanKnowledgeUpdate;
    class UrbanUpdate;
    class ConnectedProfileList;
    class ListLogisticRequestsAck;
}

namespace tools
{
    class MessageDispatcher_ABC;
    class MessageSender_ABC;
}

namespace kernel
{
    class Logger_ABC;
    class Entity_ABC;
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

    virtual void Send( const sword::ClientToSim& message );
    virtual void Register( T_SimHandler handler );
    virtual void Register( T_ReplayHandler handler );
    virtual void Send( const sword::ClientToAuthentication& message );
    virtual void Send( const sword::ClientToReplay& message );
    virtual void Send( const sword::ClientToAar& message );
    virtual void Send( const sword::ClientToMessenger& message );

    void SetElements( Model& model, Profile& profile );
    void Reconnect( const std::string& login, const std::string& password );
    //@}

    //! @name Statistics
    //@{
    unsigned long GetNbMessagesReceived() const;
    unsigned long GetNbMessagesSent    () const;
    //@}

private:
    //! @name Message callbacks
    //@{
    void OnReceiveSimToClient              ( const std::string& from, const sword::SimToClient& message );
    void OnReceiveSimToClient2             ( const std::string& from, const sword::SimToClient& message, unsigned int clientId );
    void OnReceiveMsgAuthenticationToClient( const std::string& from, const sword::AuthenticationToClient& message );
    void OnReceiveMsgReplayToClient        ( const std::string& from, const sword::ReplayToClient& message );
    void OnReceiveMsgAarToClient           ( const std::string& from, const sword::AarToClient& message );
    void OnReceiveMsgMessengerToClient     ( const std::string& from, const sword::MessengerToClient& message );
    void OnReceiveMsgDispatcherToClient    ( const std::string& from, const sword::DispatcherToClient& message );

    void OnReceiveUnitVisionCones                  ( const sword::UnitVisionCones& message );
    void OnReceiveUnitInterVisibility              ( const sword::UnitDetection& message );
    void OnReceiveObjectInterVisibility            ( const sword::ObjectDetection& message );
    void OnReceiveCrowdConcentrationInterVisibility( const sword::CrowdConcentrationDetection& message );
    void OnReceiveCrowdFlowInterVisibility         ( const sword::CrowdFlowDetection& message );
    void OnReceiveMsgDebugDrawPoints               ( const sword::DebugPoints& message );

    // Control
    void OnReceiveControlPauseAck             ( const sword::ControlPauseAck& message, unsigned int messageClientId );
    void OnReceiveControlResumeAck            ( const sword::ControlResumeAck& message, unsigned int messageClientId );
    void OnReceiveControlChangeTimeFactorAck  ( const sword::ControlChangeTimeFactorAck& message );
    void OnReceiveControlDateTimeChangeAck    ( const sword::ControlDateTimeChangeAck& message );
    void OnReceiveControlSkipToTickAck        ( const sword::ControlSkipToTickAck& message );
    void OnReceiveControlInformation          ( const sword::ControlInformation& message );
    void OnReceiveMsgProfilingValues          ( const sword::ControlProfilingInformation& message );
    void OnReceiveMsgCtrReplayInfo            ( const sword::ControlReplayInformation& message );
    void OnReceiveControlMeteoGlobalAck       ();
    void OnReceiveControlMeteoLocalAck        ();
    void OnReceiveMsgCheckPointSaveBegin      ();
    void OnReceiveMsgCheckPointSaveEnd        ( const sword::ControlCheckPointSaveEnd& message );
    void OnReceiveMsgCheckPointSetFrequencyAck();
    void OnReceiveMsgCheckPointSaveNowAck     ();
    void OnReceiveControlBeginTick            ( const sword::ControlBeginTick& message );
    void OnReceiveControlEndTick              ( const sword::ControlEndTick& message );
    void OnReceiveAutomatCreation             ( const sword::AutomatCreation& message );
    void OnReceiveAutomatDestruction          ( const sword::AutomatDestruction& message );
    void OnReceiveUnitCreation                ( const sword::UnitCreation& message );
    void OnReceiveUnitDestruction             ( const sword::UnitDestruction& message );
    void OnReceiveChangeDiplomacy             ( const sword::ChangeDiplomacy& message );
    void OnReceiveAuthenticationResponse      ( const sword::AuthenticationToClient& wrapper );
    void OnReceiveConnectedProfiles           ( const sword::ConnectedProfileList& message );
    void OnReceiveControlMeteoGlobal          ( const sword::ControlGlobalWeather& message );
    void OnReceiveControlMeteoLocalCreation   ( const sword::ControlLocalWeatherCreation& message );
    void OnReceiveControlMeteoLocalDestruction( const sword::ControlLocalWeatherDestruction& message );
    void OnReceiveMsgSendCurrentStateEnd      ( const sword::ControlSendCurrentStateEnd& message );
    void OnReceiveNewDataChunkNotification    ( const sword::NewDataChunkNotification& message );
    void OnReceiveTimeTableRequestAck         ( const sword::TimeTableRequestAck& message );
    void OnReceiveTimeTable                   ( const sword::TimeTable& message );

    // Profiles
    void OnReceiveProfileCreation             ( const sword::ProfileCreation&                message );
    void OnReceiveProfileCreationRequestAck   ( const sword::ProfileCreationRequestAck&      message );
    void OnReceiveProfileDestruction          ( const sword::ProfileDestruction&             message );
    void OnReceiveProfileDestructionRequestAck( const sword::ProfileDestructionRequestAck&   message );
    void OnReceiveProfileUpdate               ( const sword::ProfileUpdate&                  message );
    void OnReceiveProfileUpdateRequestAck     ( const sword::ProfileUpdateRequestAck&        message );

    // Side/Formation/Knowledge Group creation
    void OnReceiveKnowledgeGroupCreation   ( const sword::KnowledgeGroupCreation&    message );
    void OnReceiveKnowledgeGroupDestruction( const sword::KnowledgeGroupDestruction& message );
    void OnReceiveMsgPartyCreation         ( const sword::PartyCreation&             message );
    void OnReceiveFormationCreation        ( const sword::FormationCreation&         message );
    void OnReceiveFormationUpdate          ( const sword::FormationUpdate&           message );
    void OnReceiveFormationDestruction     ( const sword::FormationDestruction&      message );

    // LTO begin
    void OnReceiveKnowledgeGroupMagicActionAck( const sword::KnowledgeGroupMagicActionAck&   message, unsigned long nCtx );
    void OnReceiveKnowledgeGroupUpdate        ( const sword::KnowledgeGroupUpdate&           message );
    void OnReceiveKnowledgeGroupUpdateAck     ( const sword::KnowledgeGroupUpdateAck&        message, unsigned long nCtx );
    void OnReceiveKnowledgeGroupCreationAck   ( const sword::KnowledgeGroupCreationAck&      message, unsigned long nCtx );
    // LTO end

    // Attributes
    void OnReceiveUnitAttributes     ( const sword::UnitAttributes&         message );
    void OnReceiveUnitMagicActionAck ( const sword::UnitMagicActionAck&     message, unsigned int clientId );
    void OnReceiveUnitPathFind       ( const sword::UnitPathFind&           message );
    void OnReceiveAutomatAttributes  ( const sword::AutomatAttributes&      message );

    // Log maintenance
    void OnReceiveLogMaintenanceHandlingCreation   ( const sword::LogMaintenanceHandlingCreation&       message );
    void OnReceiveLogMaintenanceHandlingDestruction( const sword::LogMaintenanceHandlingDestruction&    message );
    void OnReceiveLogMaintenanceHandlingUpdate     ( const sword::LogMaintenanceHandlingUpdate&         message );
    void OnReceiveLogMaintenance              ( const sword::LogMaintenanceState&                  message );

    // Log sante
    void OnReceiveLogMedicalHandlingCreation   ( const sword::LogMedicalHandlingCreation&       message );
    void OnReceiveLogMedicalHandlingDestruction( const sword::LogMedicalHandlingDestruction&    message );
    void OnReceiveLogMedicalHandlingUpdate     ( const sword::LogMedicalHandlingUpdate&         message );
    void OnReceiveLogMedicalState              ( const sword::LogMedicalState&                  message );

    // Log supply
    void OnReceiveLogSupplyHandlingCreation          ( const sword::LogSupplyHandlingCreation&      message );
    void OnReceiveLogSupplyHandlingDestruction       ( const sword::LogSupplyHandlingDestruction&   message );
    void OnReceiveLogSupplyHandlingUpdate            ( const sword::LogSupplyHandlingUpdate&        message );
    void OnReceiveLogSupplyState                     ( const sword::LogSupplyState&                 message );
    void OnReceiveLogSupplyQuotas                    ( const sword::LogSupplyQuotas&                message );
    void OnReceiveMsgLogRavitaillementChangeQuotaAck ( const sword::LogSupplyChangeQuotasAck&       message, unsigned long nCtx );
    void OnReceiveLogSupplyPushFlowAck               ( const sword::LogSupplyPushFlowAck&           message, unsigned long nCtx );
    void OnReceiveLogSupplyPullFlowAck               ( const sword::LogSupplyPullFlowAck&           message, unsigned long nCtx);

    // Log funeral
    void OnReceiveLogFuneralHandlingCreation          ( const sword::LogFuneralHandlingCreation&      message );
    void OnReceiveLogFuneralHandlingDestruction       ( const sword::LogFuneralHandlingDestruction&   message );
    void OnReceiveLogFuneralHandlingUpdate            ( const sword::LogFuneralHandlingUpdate&        message );

    // Log
    void OnReceiveLogisticHistoryAck         ( const sword::LogisticHistoryAck& message, unsigned int clientId );
    void OnReceiveListLogisticRequestsAck    ( const sword::ListLogisticRequestsAck& message, unsigned int clientId );

    // Limas / Limits
    void OnReceiveLimitCreationRequestAck    ( const sword::LimitCreationRequestAck&    message);
    void OnReceiveLimitUpdateRequestAck      ( const sword::LimitUpdateRequestAck&      message);
    void OnReceiveLimitDestructionRequestAck ( const sword::LimitDestructionRequestAck& message);
    void OnReceiveLimaCreationRequestAck     ( const sword::PhaseLineCreationAck&     message);
    void OnReceiveLimaUpdateRequestAck       ( const sword::PhaseLineUpdateRequestAck&       message);
    void OnReceiveLimaDestructionRequestAck  ( const sword::PhaseLineDestructionRequestAck&  message);
    void OnReceiveLimitCreation              ( const sword::LimitCreation&              message );
    void OnReceiveLimitUpdate                ( const sword::LimitUpdate&                message );
    void OnReceiveLimitDestruction           ( const sword::LimitDestruction&           message );
    void OnReceiveLimaCreation               ( const sword::PhaseLineCreation&               message );
    void OnReceiveLimaUpdate                 ( const sword::PhaseLineUpdate&                 message );
    void OnReceiveLimaDestruction            ( const sword::PhaseLineDestruction&            message );

    // Magic
    void OnReceiveMagicActionAck( const sword::MagicActionAck& message, unsigned long nCtx );

    // Object
    void OnReceiveObjectCreation      ( const sword::ObjectCreation&       message );
    void OnReceiveObjectUpdate        ( const sword::ObjectUpdate&         message );
    void OnReceiveObjectDestruction   ( const sword::ObjectDestruction&    message );
    void OnReceiveObjectMagicActionAck( const sword::ObjectMagicActionAck& message, unsigned long nCtx );

    // Knowledge unit
    void OnReceiveUnitKnowledgeCreation   ( const sword::UnitKnowledgeCreation&    message );
    void OnReceiveUnitKnowledgeUpdate     ( const sword::UnitKnowledgeUpdate&      message );
    void OnReceiveUnitKnowledgeDestruction( const sword::UnitKnowledgeDestruction& message );

    // Knowledge object
    void OnReceiveObjectKnowledgeCreation   ( const sword::ObjectKnowledgeCreation&    v );
    void OnReceiveObjectKnowledgeUpdate     ( const sword::ObjectKnowledgeUpdate&      message );
    void OnReceiveObjectKnowledgeDestruction( const sword::ObjectKnowledgeDestruction& message );

    // Knowledge population
    void OnReceiveCrowdKnowledgeCreation                ( const sword::CrowdKnowledgeCreation&                 message );
    void OnReceiveCrowdKnowledgeUpdate                  ( const sword::CrowdKnowledgeUpdate&                   message );
    void OnReceiveCrowdKnowledgeDestruction             ( const sword::CrowdKnowledgeDestruction&              message );
    void OnReceiveCrowdConcentrationKnowledgeCreation   ( const sword::CrowdConcentrationKnowledgeCreation&    message );
    void OnReceiveCrowdConcentrationKnowledgeUpdate     ( const sword::CrowdConcentrationKnowledgeUpdate&      message );
    void OnReceiveCrowdConcentrationKnowledgeDestruction( const sword::CrowdConcentrationKnowledgeDestruction& message );
    void OnReceiveCrowdFlowKnowledgeCreation            ( const sword::CrowdFlowKnowledgeCreation&             message );
    void OnReceiveCrowdFlowKnowledgeUpdate              ( const sword::CrowdFlowKnowledgeUpdate&               message );
    void OnReceiveCrowdFlowKnowledgeDestruction         ( const sword::CrowdFlowKnowledgeDestruction&          message );

    // Orders
    void OnReceiveAutomatOrder          ( const sword::AutomatOrder&    message );
    void OnReceiveUnitOrder             ( const sword::UnitOrder&       message );
    void OnReceiveOrderAck              ( const sword::TaskCreationRequestAck& message, unsigned int clientId );
    void OnReceiveFragOrder             ( const sword::FragOrder&       message );
    void OnReceiveFragOrderAck          ( const sword::FragOrderAck&    message, unsigned int clientId );
    void OnReceiveUnitCreationRequestAck( const sword::UnitCreationRequestAck& message );
    void OnReceiveCrowdOrder            ( const sword::CrowdOrder&             message );

    // Automate mode
    void OnReceiveSetAutomatModeAck( const sword::SetAutomatModeAck& message, unsigned int clientId );

    // Hierachy changes
    void OnReceiveUnitChangeSuperiorAck          ( const sword::UnitChangeSuperiorAck&          message, unsigned long nCtx );
    void OnReceiveUnitChangeSuperior             ( const sword::UnitChangeSuperior&                      message );
    void OnReceiveChangeDiplomacyAck             ( const sword::ChangeDiplomacyAck&             message, unsigned long nCtx );
    void OnReceiveAutomatChangeSuperior          ( const sword::AutomatChangeSuperior&                   message );
    void OnReceiveAutomatChangeSuperiorAck       ( const sword::AutomatChangeSuperiorAck&       message, unsigned long nCtx );
    void OnReceiveAutomatChangeKnowledgeGroup    ( const sword::AutomatChangeKnowledgeGroup&             message );
    void OnReceiveAutomatChangeKnowledgeGroupAck ( const sword::AutomatChangeKnowledgeGroupAck& message, unsigned long nCtx );
    void OnReceiveAutomatChangeLogisticLinks     ( const sword::ChangeLogisticLinks&                     message );
    void OnReceiveAutomatChangeLogisticLinksAck  ( const sword::ChangeLogisticLinksAck&         message, unsigned long nCtx );
    void OnReceiveFormationChangeSuperior        ( const sword::FormationChangeSuperior&                 message );

    // RC
    void OnReceiveMsgCR              ( const sword::Report&           message );
    void OnReceiveMsgInvalidateReport( const sword::InvalidateReport& message );
    void OnReceiveTrace           ( const sword::Trace&            message );
    void OnReceiveMsgDecisionalState ( const sword::DecisionalState&  message );

    // Tirs
    void OnReceiveStartUnitFire         ( const sword::StartUnitFire& message );
    void OnReceiveStopUnitFire          ( const sword::StopUnitFire& message );
    void OnReceiveStartUnitFireDetection( const sword::StartUnitFireDetection& message );
    void OnReceiveStopUnitFireDetection ( const sword::StopUnitFireDetection& message );
    void OnReceiveExplosion             ( const sword::Explosion& message );
    void OnReceiveStartFireEffect       ( const sword::StartFireEffect& message );
    void OnReceiveStopFireEffect        ( const sword::StopFireEffect& message );
    void OnReceiveIndirectFirePerception( const sword::IndirectFirePerception&  message );
    void OnReceiveStartCrowdFire   ( const sword::StartCrowdFire& message );
    void OnReceiveStopCrowdFire    ( const sword::StopCrowdFire&  message );

    // Population
    void OnPopulationCreation                   ( const sword::PopulationCreation&              message );
    void OnPopulationUpdate                     ( const sword::PopulationUpdate&                message );

    // Crowd
    void OnCrowdCreation                ( const sword::CrowdCreation&                 message );
    void OnCrowdDestruction             ( const sword::CrowdDestruction&              message );
    void OnCrowdUpdate                  ( const sword::CrowdUpdate&                   message );
    void OnCrowdConcentrationCreation   ( const sword::CrowdConcentrationCreation&    message );
    void OnCrowdConcentrationDestruction( const sword::CrowdConcentrationDestruction& message );
    void OnCrowdConcentrationUpdate     ( const sword::CrowdConcentrationUpdate&      message );
    void OnCrowdFlowCreation            ( const sword::CrowdFlowCreation&             message );
    void OnCrowdFlowDestruction         ( const sword::CrowdFlowDestruction&          message );
    void OnCrowdFlowUpdate              ( const sword::CrowdFlowUpdate&               message );
    void OnReceiveCrowdMagicActionAck   ( const sword::CrowdMagicActionAck&           message, unsigned long nCtx );

    // Folk
    void OnReceiveFolkCreation               ( const sword::FolkCreation&    message );
    void OnReceiveFolkGraphUpdate            ( const sword::FolkGraphUpdate& message );

    // Drawings
    void OnReceiveShapeCreation             ( const sword::ShapeCreation&              message );
    void OnReceiveShapeUpdate               ( const sword::ShapeUpdate&                message );
    void OnReceiveShapeDestruction          ( const sword::ShapeDestruction&           message );
    void OnReceiveShapeCreationRequestAck   ( const sword::ShapeCreationRequestAck&    message );
    void OnReceiveShapeUpdateRequestAck     ( const sword::ShapeUpdateRequestAck&      message );
    void OnReceiveShapeDestructionRequestAck( const sword::ShapeDestructionRequestAck& message );

    // Notes
    void OnReceiveNoteCreation             ( const sword::MarkerCreation&              message );
    void OnReceiveNoteUpdate               ( const sword::MarkerUpdate&                message );
    void OnReceiveNoteDestruction          ( const sword::MarkerDestruction&           message );

    // Chat
    void OnReceiveMsgTextMessage( const sword::TextMessage& message );

    // 3a
    void OnReceiveMsgAarInformation( const sword::AarInformation& message );
    void OnReceiveMsgAarResult     ( const sword::PlotResult&     message );
    void OnReceiveMsgAarIndicator  ( const sword::Indicator&      message );

    // Urban
    void OnReceiveUrbanCreation            ( const sword::UrbanCreation&                    message );
    void OnReceiveUrbanUpdate              ( const sword::UrbanUpdate&                      message );
    void OnReceiveUrbanDetection           ( const sword::UrbanDetection&                   message );
    void OnReceiveUrbanKnowledgeCreation   ( const sword::UrbanKnowledgeCreation&           message );
    void OnReceiveUrbanKnowledgeUpdate     ( const sword::UrbanKnowledgeUpdate&             message );
    void OnReceiveUrbanKnowledgeDestruction( const sword::UrbanKnowledgeDestruction&        message );

    // Handlers
    void UpdateHandlers( const sword::SimToClient& message );
    void UpdateHandlers( const sword::ReplayToClient& message );

    // Burn surface
    void OnReceiveBurningCellRequestAck( const sword::BurningCellRequestAck& message, unsigned long nCtx );
    //@}

    //! @name Helpers
    //@{
    Model& GetModel() const;
    Profile& GetProfile() const;
    kernel::Entity_ABC& GetTasker( const sword::Tasker& tasker ) const;
    //@}

    //! @name Copy / Assignment
    //@{
    AgentServerMsgMgr( const AgentServerMsgMgr& );
    AgentServerMsgMgr& operator=( const AgentServerMsgMgr& );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< T_SimHandler >    T_SimHandlers;
    typedef std::vector< T_ReplayHandler > T_ReplayHandlers;
    //@}

    //! @name Member data
    //@{
    tools::MessageDispatcher_ABC& dispatcher_;
    tools::MessageSender_ABC& sender_;
    std::string               host_;
    Model*                    model_;
    Profile*                  profile_;
    Services&                 services_;
    Simulation&               simulation_;
    kernel::Logger_ABC&       logger_;
    CommandHandler&           commands_;
    T_SimHandlers             simHandlers_;
    T_ReplayHandlers          replayHandlers_;
    //@}
};

#endif // __AgentServerMsgMgr_h_

