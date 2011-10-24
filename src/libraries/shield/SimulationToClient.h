// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef shield_SimulationToClient_h
#define shield_SimulationToClient_h

#include "Tools.h"
#include "protocol/Protocol.h"
#pragma warning( push, 0 )
#include "proto/SimToClient.pb.h"
#pragma warning( pop )

namespace shield
{
// =============================================================================
/** @class  SimulationToClient
    @brief  Simulation to client converter
*/
// Created: MCO 2010-11-09
// =============================================================================
class SimulationToClient
{
public:
    //! @name Operations
    //@{
    static void ConvertOrderAckToSpecificOrderAck( const sword::TaskCreationRequestAck& from, MsgsSimToClient::MsgSimToClient_Content* to );
    static void Convert( const sword::ActionCreateFireOrderAck& from, MsgsSimToClient::MsgActionCreateFireOrderAck* to );
    static void Convert( const sword::AutomatAttributes& from, MsgsSimToClient::MsgAutomatAttributes* to );
    static void Convert( const sword::AutomatChangeKnowledgeGroup& from, Common::MsgAutomatChangeKnowledgeGroup* to );
    static void Convert( const sword::AutomatChangeKnowledgeGroupAck& from, MsgsSimToClient::MsgAutomatChangeKnowledgeGroupAck* to );
    static void Convert( const sword::AutomatChangeSuperior& from, Common::MsgAutomatChangeSuperior* to );
    static void Convert( const sword::AutomatChangeSuperiorAck& from, MsgsSimToClient::MsgAutomatChangeSuperiorAck* to );
    static void Convert( const sword::AutomatCreation& from, MsgsSimToClient::MsgAutomatCreation* to );
    static void Convert( const sword::AutomatDestruction& from, MsgsSimToClient::MsgAutomatDestruction* to );
    static void Convert( const sword::AutomatOrder& from, Common::MsgAutomatOrder* to );
    static void Convert( const sword::ChangeDiplomacy& from, Common::MsgChangeDiplomacy* to );
    static void Convert( const sword::ChangeDiplomacyAck& from, MsgsSimToClient::MsgChangeDiplomacyAck* to );
    static void Convert( const sword::ChangeLogisticLinks& from, Common::MsgChangeLogisticLinks* to );
    static void Convert( const sword::ChangeLogisticLinksAck& from, MsgsSimToClient::MsgChangeLogisticLinksAck* to );
    static void Convert( const sword::ControlBeginTick& from, MsgsSimToClient::MsgControlBeginTick* to );
    static void Convert( const sword::ControlChangeTimeFactorAck& from, MsgsSimToClient::MsgControlChangeTimeFactorAck* to );
    static void Convert( const sword::ControlCheckPointDeleteAck& from, MsgsSimToClient::MsgControlCheckPointDeleteAck* to );
    static void Convert( const sword::ControlCheckPointList& from, MsgsSimToClient::MsgControlCheckPointList* to );
    static void Convert( const sword::ControlCheckPointListAck& from, MsgsSimToClient::MsgControlCheckPointListAck* to );
    static void Convert( const sword::ControlCheckPointSaveBegin& from, MsgsSimToClient::MsgControlCheckPointSaveBegin* to );
    static void Convert( const sword::ControlCheckPointSaveEnd& from, MsgsSimToClient::MsgControlCheckPointSaveEnd* to );
    static void Convert( const sword::ControlCheckPointSaveNowAck& from, MsgsSimToClient::MsgControlCheckPointSaveNowAck* to );
    static void Convert( const sword::ControlCheckPointSetFrequencyAck& from, MsgsSimToClient::MsgControlCheckPointSetFrequencyAck* to );
    static void Convert( const sword::ControlDateTimeChangeAck& from, MsgsSimToClient::MsgControlDatetimeChangeAck* to );
    static void Convert( const sword::ControlEndTick& from, MsgsSimToClient::MsgControlEndTick* to );
    static void Convert( const sword::ControlGlobalWeather& from, MsgsSimToClient::MsgControlGlobalWeather* to );
    static void Convert( const sword::ControlGlobalWeatherAck& from, MsgsSimToClient::MsgControlGlobalWeatherAck* to );
    static void Convert( const sword::ControlInformation& from, MsgsSimToClient::MsgControlInformation* to );
    static void Convert( const sword::ControlLocalWeatherAck& from, MsgsSimToClient::MsgControlLocalWeatherAck* to );
    static void Convert( const sword::ControlLocalWeatherCreation& from, MsgsSimToClient::MsgControlLocalWeatherCreation* to );
    static void Convert( const sword::ControlLocalWeatherDestruction& from, MsgsSimToClient::MsgControlLocalWeatherDestruction* to );
    static void Convert( const sword::ControlPauseAck& from, MsgsSimToClient::MsgControlPauseAck* to );
    static void Convert( const sword::ControlProfilingInformation& from, MsgsSimToClient::MsgControlProfilingInformation* to );
    static void Convert( const sword::ControlResumeAck& from, MsgsSimToClient::MsgControlResumeAck* to );
    static void Convert( const sword::ControlSendCurrentStateBegin& from, MsgsSimToClient::MsgControlSendCurrentStateBegin* to );
    static void Convert( const sword::ControlSendCurrentStateEnd& from, MsgsSimToClient::MsgControlSendCurrentStateEnd* to );
    static void Convert( const sword::ControlStopAck& from, MsgsSimToClient::MsgControlStopAck* to );
    static void Convert( const sword::CrowdConcentrationCreation& from, MsgsSimToClient::MsgCrowdConcentrationCreation* to );
    static void Convert( const sword::CrowdConcentrationDestruction& from, MsgsSimToClient::MsgCrowdConcentrationDestruction* to );
    static void Convert( const sword::CrowdConcentrationKnowledgeCreation& from, MsgsSimToClient::MsgCrowdConcentrationKnowledgeCreation* to );
    static void Convert( const sword::CrowdConcentrationKnowledgeDestruction& from, MsgsSimToClient::MsgCrowdConcentrationKnowledgeDestruction* to );
    static void Convert( const sword::CrowdConcentrationKnowledgeUpdate& from, MsgsSimToClient::MsgCrowdConcentrationKnowledgeUpdate* to );
    static void Convert( const sword::CrowdConcentrationUpdate& from, MsgsSimToClient::MsgCrowdConcentrationUpdate* to );
    static void Convert( const sword::CrowdCreation& from, MsgsSimToClient::MsgCrowdCreation* to );
    static void Convert( const sword::CrowdDestruction& from, MsgsSimToClient::MsgCrowdDestruction* to );
    static void Convert( const sword::CrowdFlowCreation& from, MsgsSimToClient::MsgCrowdFlowCreation* to );
    static void Convert( const sword::CrowdFlowDestruction& from, MsgsSimToClient::MsgCrowdFlowDestruction* to );
    static void Convert( const sword::CrowdFlowKnowledgeCreation& from, MsgsSimToClient::MsgCrowdFlowKnowledgeCreation* to );
    static void Convert( const sword::CrowdFlowKnowledgeDestruction& from, MsgsSimToClient::MsgCrowdFlowKnowledgeDestruction* to );
    static void Convert( const sword::CrowdFlowKnowledgeUpdate& from, MsgsSimToClient::MsgCrowdFlowKnowledgeUpdate* to );
    static void Convert( const sword::CrowdFlowUpdate& from, MsgsSimToClient::MsgCrowdFlowUpdate* to );
    static void Convert( const sword::CrowdKnowledgeCreation& from, MsgsSimToClient::MsgCrowdKnowledgeCreation* to );
    static void Convert( const sword::CrowdKnowledgeDestruction& from, MsgsSimToClient::MsgCrowdKnowledgeDestruction* to );
    static void Convert( const sword::CrowdKnowledgeUpdate& from, MsgsSimToClient::MsgCrowdKnowledgeUpdate* to );
    static void Convert( const sword::CrowdMagicActionAck& from, MsgsSimToClient::MsgCrowdMagicActionAck* to );
    static void Convert( const sword::CrowdOrder& from, Common::MsgCrowdOrder* to );
    static void Convert( const sword::CrowdUpdate& from, MsgsSimToClient::MsgCrowdUpdate* to );
    static void Convert( const sword::DebugPoints& from, MsgsSimToClient::MsgDebugPoints* to );
    static void Convert( const sword::DecisionalState& from, MsgsSimToClient::MsgDecisionalState* to );
    static void Convert( const sword::Explosion& from, MsgsSimToClient::MsgExplosion* to );
    static void Convert( const sword::FormationCreation& from, MsgsSimToClient::MsgFormationCreation* to );
    static void Convert( const sword::FormationDestruction& from, MsgsSimToClient::MsgFormationDestruction* to );
    static void Convert( const sword::FormationUpdate& from, MsgsSimToClient::MsgFormationUpdate* to );
    static void Convert( const sword::FragOrderAck& from, MsgsSimToClient::MsgFragOrderAck* to );
    static void Convert( const sword::InvalidateReport& from, MsgsSimToClient::MsgInvalidateReport* to );
    static void Convert( const sword::KnowledgeGroupCreation& from, MsgsSimToClient::MsgKnowledgeGroupCreation* to );
    static void Convert( const sword::KnowledgeGroupCreationAck& from, MsgsSimToClient::MsgKnowledgeGroupCreationAck* to );
    static void Convert( const sword::KnowledgeGroupDestruction& from, MsgsSimToClient::MsgKnowledgeGroupDestruction* to );
    static void Convert( const sword::KnowledgeGroupMagicActionAck& from, MsgsSimToClient::MsgKnowledgeGroupMagicActionAck* to );
    static void Convert( const sword::KnowledgeGroupUpdate& from, MsgsSimToClient::MsgKnowledgeGroupUpdate* to );
    static void Convert( const sword::KnowledgeGroupUpdateAck& from, MsgsSimToClient::MsgKnowledgeGroupMagicActionAck* to );
    static void Convert( const sword::LogMaintenanceHandlingCreation& from, MsgsSimToClient::MsgLogMaintenanceHandlingCreation* to );
    static void Convert( const sword::LogMaintenanceHandlingDestruction& from, MsgsSimToClient::MsgLogMaintenanceHandlingDestruction* to );
    static void Convert( const sword::LogMaintenanceHandlingUpdate& from, MsgsSimToClient::MsgLogMaintenanceHandlingUpdate* to );
    static void Convert( const sword::LogMaintenanceState& from, MsgsSimToClient::MsgLogMaintenanceState* to );
    static void Convert( const sword::LogMedicalHandlingCreation& from, MsgsSimToClient::MsgLogMedicalHandlingCreation* to );
    static void Convert( const sword::LogMedicalHandlingDestruction& from, MsgsSimToClient::MsgLogMedicalHandlingDestruction* to );
    static void Convert( const sword::LogMedicalHandlingUpdate& from, MsgsSimToClient::MsgLogMedicalHandlingUpdate* to );
    static void Convert( const sword::LogMedicalState& from, MsgsSimToClient::MsgLogMedicalState* to );
    static void Convert( const sword::LogSupplyChangeQuotasAck& from, MsgsSimToClient::MsgLogSupplyChangeQuotasAck* to );
    static void Convert( const sword::LogSupplyHandlingCreation& from, MsgsSimToClient::MsgLogSupplyHandlingCreation* to );
    static void Convert( const sword::LogSupplyHandlingDestruction& from, MsgsSimToClient::MsgLogSupplyHandlingDestruction* to );
    static void Convert( const sword::LogSupplyHandlingUpdate& from, MsgsSimToClient::MsgLogSupplyHandlingUpdate* to );
    static void Convert( const sword::LogSupplyPullFlowAck& from, MsgsSimToClient::MsgLogSupplyPullFlowAck* to );
    static void Convert( const sword::LogSupplyPushFlowAck& from, MsgsSimToClient::MsgLogSupplyPushFlowAck* to );
    static void Convert( const sword::LogSupplyQuotas& from, MsgsSimToClient::MsgLogSupplyQuotas* to );
    static void Convert( const sword::LogSupplyState& from, MsgsSimToClient::MsgLogSupplyState* to );
    static void Convert( const sword::LogFuneralHandlingCreation& from, MsgsSimToClient::MsgLogFuneralHandlingCreation* to );
    static void Convert( const sword::LogFuneralHandlingDestruction& from, MsgsSimToClient::MsgLogFuneralHandlingDestruction* to );
    static void Convert( const sword::LogFuneralHandlingUpdate& from, MsgsSimToClient::MsgLogFuneralHandlingUpdate* to );
    static void Convert( const sword::MagicActionAck& from, MsgsSimToClient::MsgMagicActionAck* to );
    static void Convert( const sword::ObjectCreation& from, MsgsSimToClient::MsgObjectCreation* to );
    static void Convert( const sword::ObjectDestruction& from, MsgsSimToClient::MsgObjectDestruction* to );
    static void Convert( const sword::ObjectKnowledgeCreation& from, MsgsSimToClient::MsgObjectKnowledgeCreation* to );
    static void Convert( const sword::ObjectKnowledgeDestruction& from, MsgsSimToClient::MsgObjectKnowledgeDestruction* to );
    static void Convert( const sword::ObjectKnowledgeUpdate& from, MsgsSimToClient::MsgObjectKnowledgeUpdate* to );
    static void Convert( const sword::ObjectMagicActionAck& from, MsgsSimToClient::MsgObjectMagicActionAck* to );
    static void Convert( const sword::ObjectUpdate& from, MsgsSimToClient::MsgObjectUpdate* to );
    static void Convert( const sword::PartyCreation& from, MsgsSimToClient::MsgPartyCreation* to );
    static void Convert( const sword::PopulationCreation& from, MsgsSimToClient::MsgPopulationCreation* to );
    static void Convert( const sword::PopulationUpdate& from, MsgsSimToClient::MsgPopulationUpdate* to );
    static void Convert( const sword::Report& from, MsgsSimToClient::MsgReport* to );
    static void Convert( const sword::SetAutomatModeAck& from, MsgsSimToClient::MsgSetAutomatModeAck* to );
    static void Convert( const sword::StartCrowdFire& from, MsgsSimToClient::MsgStartCrowdFire* to );
    static void Convert( const sword::StartFireEffect& from, MsgsSimToClient::MsgStartFireEffect* to );
    static void Convert( const sword::StartUnitFire& from, MsgsSimToClient::MsgStartUnitFire* to );
    static void Convert( const sword::StockResource& from, Common::StockResource* to );
    static void Convert( const sword::StopCrowdFire& from, MsgsSimToClient::MsgStopCrowdFire* to );
    static void Convert( const sword::StopFireEffect& from, MsgsSimToClient::MsgStopFireEffect* to );
    static void Convert( const sword::StopUnitFire& from, MsgsSimToClient::MsgStopUnitFire* to );
    static void Convert( const sword::UnitAttributes& from, MsgsSimToClient::MsgUnitAttributes* to );
    static void Convert( const sword::UnitChangeSuperior& from, Common::MsgUnitChangeSuperior* to );
    static void Convert( const sword::UnitChangeSuperiorAck& from, MsgsSimToClient::MsgUnitChangeSuperiorAck* to );
    static void Convert( const sword::UnitCreation& from, MsgsSimToClient::MsgUnitCreation* to );
    static void Convert( const sword::UnitCreationRequestAck& from, MsgsSimToClient::MsgUnitCreationRequestAck* to );
    static void Convert( const sword::UnitDestruction& from, MsgsSimToClient::MsgUnitDestruction* to );
    static void Convert( const sword::UnitEnvironmentType& from, MsgsSimToClient::MsgUnitEnvironmentType* to );
    static void Convert( const sword::UnitKnowledgeCreation& from, MsgsSimToClient::MsgUnitKnowledgeCreation* to );
    static void Convert( const sword::UnitKnowledgeDestruction& from, MsgsSimToClient::MsgUnitKnowledgeDestruction* to );
    static void Convert( const sword::UnitKnowledgeUpdate& from, MsgsSimToClient::MsgUnitKnowledgeUpdate* to );
    static void Convert( const sword::UnitMagicActionAck& from, MsgsSimToClient::MsgUnitMagicActionAck* to );
    static void Convert( const sword::UnitOrder& from, Common::MsgUnitOrder* to );
    static void Convert( const sword::UnitPathFind& from, MsgsSimToClient::MsgUnitPathFind* to );
    static void Convert( const sword::UnitVisionCones& from, MsgsSimToClient::MsgUnitVisionCones* to );
    static void Convert( const sword::UrbanCreation& from, MsgsSimToClient::MsgUrbanCreation* to );
    static void Convert( const sword::UrbanKnowledgeCreation& from, MsgsSimToClient::MsgUrbanKnowledgeCreation* to );
    static void Convert( const sword::UrbanKnowledgeUpdate& from, MsgsSimToClient::MsgUrbanKnowledgeUpdate* to );
    static void Convert( const sword::UrbanUpdate& from, MsgsSimToClient::MsgUrbanUpdate* to );
    //@}
};

}

#endif // shield_SimulationToClient_h
