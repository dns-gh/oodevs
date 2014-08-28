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
    class AarToClient;
    class AuthenticationToClient;
    class ClientToAar;
    class ClientToAuthentication;
    class ClientToMessenger;
    class ClientToReplay;
    class ClientToSim;
    class DispatcherToClient;
    class MessengerToClient;
    class ReplayToClient;
    class SimToClient;
    class Tasker;
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
    void OnReceiveMsgAuthenticationToClient( const std::string& from, const sword::AuthenticationToClient& message );
    void OnReceiveMsgReplayToClient        ( const std::string& from, const sword::ReplayToClient& message );
    void OnReceiveMsgAarToClient           ( const std::string& from, const sword::AarToClient& message );
    void OnReceiveMsgMessengerToClient     ( const std::string& from, const sword::MessengerToClient& message );
    void OnReceiveMsgDispatcherToClient    ( const std::string& from, const sword::DispatcherToClient& message );

    void OnReceiveUnitVisionCones                  ( const sword::SimToClient& msg );
    void OnReceiveUnitInterVisibility              ( const sword::SimToClient& msg );
    void OnReceiveObjectInterVisibility            ( const sword::SimToClient& msg );
    void OnReceiveCrowdConcentrationInterVisibility( const sword::SimToClient& msg );
    void OnReceiveCrowdFlowInterVisibility         ( const sword::SimToClient& msg );
    void OnReceiveMsgDebugDrawPoints               ( const sword::SimToClient& msg );

    // Control
    template< typename T >
    void OnReceiveControlPauseAck             ( const T& msg );
    template< typename T >
    void OnReceiveControlResumeAck            ( const T& msg );
    template< typename T >
    void OnReceiveControlChangeTimeFactorAck  ( const T& msg );
    void OnReceiveControlDateTimeChangeAck    ( const sword::SimToClient& msg );
    void OnReceiveControlSkipToTickAck        ( const sword::ReplayToClient& msg );
    void OnReceiveControlInformation          ( const sword::SimToClient& msg );
    void OnReceiveMsgProfilingValues          ( const sword::SimToClient& msg );
    void OnReceiveMsgCtrReplayInfo            ( const sword::ReplayToClient& msg );
    void OnReceiveControlMeteoGlobalAck       ( const sword::SimToClient& msg );
    void OnReceiveControlMeteoLocalAck        ( const sword::SimToClient& msg );
    void OnReceiveControlEnableVisionCones    ( const sword::SimToClient& msg );
    void OnReceiveMsgCheckPointSaveBegin      ( const sword::SimToClient& msg );
    void OnReceiveMsgCheckPointSaveEnd        ( const sword::SimToClient& msg );
    void OnReceiveMsgCheckPointSetFrequencyAck( const sword::SimToClient& msg );
    void OnReceiveMsgCheckPointSaveNowAck     ( const sword::SimToClient& msg );
    void OnReceiveControlBeginTick            ( const sword::SimToClient& msg );
    void OnReceiveControlEndTick              ( const sword::SimToClient& msg );
    void OnReceiveAutomatCreation             ( const sword::SimToClient& msg );
    void OnReceiveAutomatDestruction          ( const sword::SimToClient& msg );
    void OnReceiveUnitCreation                ( const sword::SimToClient& msg );
    void OnReceiveUnitDestruction             ( const sword::SimToClient& msg );
    void OnReceiveChangeDiplomacy             ( const sword::SimToClient& msg );
    void OnReceiveAuthenticationResponse      ( const sword::AuthenticationToClient& msg );
    void OnReceiveConnectedProfiles           ( const sword::AuthenticationToClient& msg );
    void OnReceiveControlMeteoGlobal          ( const sword::SimToClient& msg );
    void OnReceiveControlMeteoLocalCreation   ( const sword::SimToClient& msg );
    void OnReceiveControlMeteoLocalDestruction( const sword::SimToClient& msg );
    void OnReceiveMsgSendCurrentStateEnd      ( const sword::SimToClient& msg );
    void OnReceiveNewDataChunkNotification    ( const sword::ReplayToClient& msg );
    void OnReceiveTimeskip                    ( const sword::ReplayToClient& msg );
    void OnReceiveTimeTableRequestAck         ( const sword::ReplayToClient& msg );
    void OnReceiveTimeTable                   ( const sword::ReplayToClient& msg );

    // Profiles
    void OnReceiveProfileCreation             ( const sword::AuthenticationToClient& msg );
    void OnReceiveProfileCreationRequestAck   ( const sword::AuthenticationToClient& msg );
    void OnReceiveProfileDestruction          ( const sword::AuthenticationToClient& msg );
    void OnReceiveProfileDestructionRequestAck( const sword::AuthenticationToClient& msg );
    void OnReceiveProfileUpdate               ( const sword::AuthenticationToClient& msg );
    void OnReceiveProfileUpdateRequestAck     ( const sword::AuthenticationToClient& msg );

    // Side/Formation/Knowledge Group creation
    void OnReceiveKnowledgeGroupCreation   ( const sword::SimToClient& msg );
    void OnReceiveKnowledgeGroupDestruction( const sword::SimToClient& msg );
    void OnReceiveMsgPartyCreation         ( const sword::SimToClient& msg );
    void OnReceiveMsgPartyUpdate           ( const sword::SimToClient& msg );
    void OnReceiveFormationCreation        ( const sword::SimToClient& msg );
    void OnReceiveFormationUpdate          ( const sword::SimToClient& msg );
    void OnReceiveFormationDestruction     ( const sword::SimToClient& msg );

    // LTO begin
    void OnReceiveKnowledgeGroupMagicActionAck( const sword::SimToClient& msg );
    void OnReceiveKnowledgeGroupUpdate        ( const sword::SimToClient& msg );
    void OnReceiveKnowledgeGroupUpdateAck     ( const sword::SimToClient& msg );
    void OnReceiveKnowledgeGroupCreationAck   ( const sword::SimToClient& msg );
    // LTO end

    // Attributes
    void OnReceiveUnitAttributes    ( const sword::SimToClient& msg );
    void OnReceiveUnitMagicActionAck( const sword::SimToClient& msg );
    void OnReceiveUnitPathFind      ( const sword::SimToClient& msg );
    void OnReceiveAutomatAttributes ( const sword::SimToClient& msg );

    // Log maintenance
    void OnReceiveLogMaintenanceHandlingCreation   ( const sword::SimToClient& msg );
    void OnReceiveLogMaintenanceHandlingDestruction( const sword::SimToClient& msg );
    void OnReceiveLogMaintenanceHandlingUpdate     ( const sword::SimToClient& msg );
    void OnReceiveLogMaintenance                   ( const sword::SimToClient& msg );

    // Log sante
    void OnReceiveLogMedicalHandlingCreation   ( const sword::SimToClient& msg );
    void OnReceiveLogMedicalHandlingDestruction( const sword::SimToClient& msg );
    void OnReceiveLogMedicalHandlingUpdate     ( const sword::SimToClient& msg );
    void OnReceiveLogMedicalState              ( const sword::SimToClient& msg );

    // Log supply
    void OnReceiveLogSupplyHandlingCreation          ( const sword::SimToClient& msg );
    void OnReceiveLogSupplyHandlingDestruction       ( const sword::SimToClient& msg );
    void OnReceiveLogSupplyHandlingUpdate            ( const sword::SimToClient& msg );
    void OnReceiveLogSupplyState                     ( const sword::SimToClient& msg );
    void OnReceiveLogSupplyQuotas                    ( const sword::SimToClient& msg );
    void OnReceiveMsgLogRavitaillementChangeQuotaAck ( const sword::SimToClient& msg );
    void OnReceiveLogSupplyPushFlowAck               ( const sword::SimToClient& msg );
    void OnReceiveLogSupplyPullFlowAck               ( const sword::SimToClient& msg );

    // Log funeral
    void OnReceiveLogFuneralHandlingCreation   ( const sword::SimToClient& msg );
    void OnReceiveLogFuneralHandlingDestruction( const sword::SimToClient& msg );
    void OnReceiveLogFuneralHandlingUpdate     ( const sword::SimToClient& msg );

    // Log
    template< typename T >
    void OnReceiveLogisticHistoryAck         ( const T& msg );
    template< typename T >
    void OnReceiveListLogisticRequestsAck    ( const T& msg );

    // Limas / Limits
    void OnReceiveLimitCreationRequestAck    ( const sword::MessengerToClient& msg );
    void OnReceiveLimitUpdateRequestAck      ( const sword::MessengerToClient& msg );
    void OnReceiveLimitDestructionRequestAck ( const sword::MessengerToClient& msg );
    void OnReceiveLimaCreationRequestAck     ( const sword::MessengerToClient& msg );
    void OnReceiveLimaUpdateRequestAck       ( const sword::MessengerToClient& msg );
    void OnReceiveLimaDestructionRequestAck  ( const sword::MessengerToClient& msg );
    void OnReceiveLimitCreation              ( const sword::MessengerToClient& msg );
    void OnReceiveLimitUpdate                ( const sword::MessengerToClient& msg );
    void OnReceiveLimitDestruction           ( const sword::MessengerToClient& msg );
    void OnReceiveLimaCreation               ( const sword::MessengerToClient& msg );
    void OnReceiveLimaUpdate                 ( const sword::MessengerToClient& msg );
    void OnReceiveLimaDestruction            ( const sword::MessengerToClient& msg );

    // Magic
    void OnReceiveMagicActionAck( const sword::SimToClient& msg );

    // Object
    void OnReceiveObjectCreation      ( const sword::SimToClient& msg );
    void OnReceiveObjectUpdate        ( const sword::SimToClient& msg );
    void OnReceiveObjectDestruction   ( const sword::SimToClient& msg );
    void OnReceiveObjectMagicActionAck( const sword::SimToClient& msg );

    // Knowledge unit
    void OnReceiveUnitKnowledgeCreation   ( const sword::SimToClient& msg );
    void OnReceiveUnitKnowledgeUpdate     ( const sword::SimToClient& msg );
    void OnReceiveUnitKnowledgeDestruction( const sword::SimToClient& msg );

    // Knowledge object
    void OnReceiveObjectKnowledgeCreation   ( const sword::SimToClient& msg );
    void OnReceiveObjectKnowledgeUpdate     ( const sword::SimToClient& msg );
    void OnReceiveObjectKnowledgeDestruction( const sword::SimToClient& msg );

    // Knowledge population
    void OnReceiveCrowdKnowledgeCreation                ( const sword::SimToClient& msg );
    void OnReceiveCrowdKnowledgeUpdate                  ( const sword::SimToClient& msg );
    void OnReceiveCrowdKnowledgeDestruction             ( const sword::SimToClient& msg );
    void OnReceiveCrowdConcentrationKnowledgeCreation   ( const sword::SimToClient& msg );
    void OnReceiveCrowdConcentrationKnowledgeUpdate     ( const sword::SimToClient& msg );
    void OnReceiveCrowdConcentrationKnowledgeDestruction( const sword::SimToClient& msg );
    void OnReceiveCrowdFlowKnowledgeCreation            ( const sword::SimToClient& msg );
    void OnReceiveCrowdFlowKnowledgeUpdate              ( const sword::SimToClient& msg );
    void OnReceiveCrowdFlowKnowledgeDestruction         ( const sword::SimToClient& msg );

    // Orders
    void OnReceiveAutomatOrder          ( const sword::SimToClient& msg );
    void OnReceiveUnitOrder             ( const sword::SimToClient& msg );
    void OnReceiveOrderAck              ( const sword::SimToClient& msg );
    void OnReceiveFragOrder             ( const sword::SimToClient& msg );
    void OnReceiveFragOrderAck          ( const sword::SimToClient& msg );
    void OnReceiveUnitCreationRequestAck( const sword::SimToClient& msg );
    void OnReceiveCrowdOrder            ( const sword::SimToClient& msg );

    // Automate mode
    void OnReceiveSetAutomatModeAck( const sword::SimToClient& msg );

    // Hierachy changes;
    void OnReceiveUnitChangeSuperiorAck         ( const sword::SimToClient& msg );
    void OnReceiveUnitChangeSuperior            ( const sword::SimToClient& msg );
    void OnReceiveChangeDiplomacyAck            ( const sword::SimToClient& msg );
    void OnReceiveAutomatChangeSuperior         ( const sword::SimToClient& msg );
    void OnReceiveAutomatChangeSuperiorAck      ( const sword::SimToClient& msg );
    void OnReceiveAutomatChangeKnowledgeGroup   ( const sword::SimToClient& msg );
    void OnReceiveAutomatChangeKnowledgeGroupAck( const sword::SimToClient& msg );
    void OnReceiveAutomatChangeLogisticLinks    ( const sword::SimToClient& msg );
    void OnReceiveAutomatChangeLogisticLinksAck ( const sword::SimToClient& msg );
    void OnReceiveFormationChangeSuperior       ( const sword::SimToClient& msg );

    // RC
    void OnReceiveMsgCR              ( const sword::SimToClient& msg );
    void OnReceiveMsgInvalidateReport( const sword::SimToClient& msg );
    void OnReceiveTrace              ( const sword::SimToClient& msg );
    void OnReceiveMsgDecisionalState ( const sword::SimToClient& msg );

    // Tirs
    void OnReceiveStartUnitFire         ( const sword::SimToClient& msg );
    void OnReceiveStopUnitFire          ( const sword::SimToClient& msg );
    void OnReceiveStartUnitFireDetection( const sword::SimToClient& msg );
    void OnReceiveStopUnitFireDetection ( const sword::SimToClient& msg );
    void OnReceiveExplosion             ( const sword::SimToClient& msg );
    void OnReceiveStartFireEffect       ( const sword::SimToClient& msg );
    void OnReceiveStopFireEffect        ( const sword::SimToClient& msg );
    void OnReceiveIndirectFirePerception( const sword::SimToClient& msg );
    void OnReceiveStartCrowdFire        ( const sword::SimToClient& msg );
    void OnReceiveStopCrowdFire         ( const sword::SimToClient& msg );

    // Population
    void OnPopulationCreation( const sword::SimToClient& msg );
    void OnPopulationUpdate  ( const sword::SimToClient& msg );

    // Crowd
    void OnCrowdCreation                ( const sword::SimToClient& msg );
    void OnCrowdDestruction             ( const sword::SimToClient& msg );
    void OnCrowdUpdate                  ( const sword::SimToClient& msg );
    void OnCrowdConcentrationCreation   ( const sword::SimToClient& msg );
    void OnCrowdConcentrationDestruction( const sword::SimToClient& msg );
    void OnCrowdConcentrationUpdate     ( const sword::SimToClient& msg );
    void OnCrowdFlowCreation            ( const sword::SimToClient& msg );
    void OnCrowdFlowDestruction         ( const sword::SimToClient& msg );
    void OnCrowdFlowUpdate              ( const sword::SimToClient& msg );
    void OnReceiveCrowdMagicActionAck   ( const sword::SimToClient& msg );

    // Folk
    void OnReceiveFolkCreation          ( const sword::SimToClient& msg );
    void OnReceiveFolkGraphUpdate       ( const sword::SimToClient& msg );

    // Drawings
    void OnReceiveShapeCreation             ( const sword::MessengerToClient& msg );
    void OnReceiveShapeUpdate               ( const sword::MessengerToClient& msg );
    void OnReceiveShapeDestruction          ( const sword::MessengerToClient& msg );
    void OnReceiveShapeCreationRequestAck   ( const sword::MessengerToClient& msg );
    void OnReceiveShapeUpdateRequestAck     ( const sword::MessengerToClient& msg );
    void OnReceiveShapeDestructionRequestAck( const sword::MessengerToClient& msg );

    // Notes
    void OnReceiveNoteCreation              ( const sword::MessengerToClient& msg );
    void OnReceiveNoteUpdate                ( const sword::MessengerToClient& msg );
    void OnReceiveNoteDestruction           ( const sword::MessengerToClient& msg );

    // Chat
    void OnReceiveMsgTextMessage            ( const sword::MessengerToClient& msg );

    // 3a
    void OnReceiveMsgAarInformation( const sword::AarToClient& msg );
    void OnReceiveMsgAarResult     ( const sword::AarToClient& msg );
    void OnReceiveMsgAarIndicator  ( const sword::AarToClient& msg );

    // Urban
    void OnReceiveUrbanCreation            ( const sword::SimToClient& msg );
    void OnReceiveUrbanUpdate              ( const sword::SimToClient& msg );
    void OnReceiveUrbanDetection           ( const sword::SimToClient& msg );
    void OnReceiveUrbanKnowledgeCreation   ( const sword::SimToClient& msg );
    void OnReceiveUrbanKnowledgeUpdate     ( const sword::SimToClient& msg );
    void OnReceiveUrbanKnowledgeDestruction( const sword::SimToClient& msg );

    // Handlers
    void UpdateHandlers( const sword::SimToClient& message );
    void UpdateHandlers( const sword::ReplayToClient& message );

    // Burn surface
    void OnReceiveBurningCellRequestAck( const sword::SimToClient& msg );
    //@}

    // Pathfinds
    void OnReceivePathfindCreation   ( const sword::SimToClient& msg );
    void OnReceivePathfindUpdate     ( const sword::SimToClient& msg );
    void OnReceivePathfindDestruction( const sword::SimToClient& msg );
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

