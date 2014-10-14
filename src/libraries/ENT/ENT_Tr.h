// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ENT_Tr_h_
#define __ENT_Tr_h_

#include "ENT_Enums.h"
#include <string>

namespace sword
{
    enum DotationType;
    enum EnumLogisticLevel;
    enum KnowledgeMagicAction_Type;
    enum LogFuneralHandlingUpdate_EnumLogFuneralHandlingStatus;
    enum LogMaintenanceHandlingUpdate_EnumLogMaintenanceHandlingStatus;
    enum LogMedicalHandlingUpdate_EnumLogMedicalHandlingStatus;
    enum LogSupplyHandlingUpdate_EnumLogSupplyHandlingStatus;
    enum MagicAction_Type;
    enum ObjectMagicAction_Type;
    enum UnitMagicAction_Type;
}

namespace ENT_Tr
{
    enum E_Conversion
    {
        eToSim,
        eToApp,
        eToTr
    };

    void InitTranslations();

        // ConvertFrom functions
    const std::string& ConvertFromAgentNbcSuit( E_AgentNbcSuit, E_Conversion = eToSim );
    const std::string& ConvertFromAmmunitionType( E_AmmunitionType, E_Conversion = eToSim );
    const std::string& ConvertFromAviationRange( E_AviationRange, E_Conversion = eToSim );
    const std::string& ConvertFromBreakdownNTI( E_BreakdownNTI, ENT_Tr::E_Conversion = ENT_Tr::eToSim );
    const std::string& ConvertFromBreakdownType( E_BreakdownType, ENT_Tr::E_Conversion = ENT_Tr::eToSim );
    const std::string& ConvertFromCoordinateSystem( E_CoordinateSystem, E_Conversion = eToTr );
    const std::string& ConvertFromCrossingType( E_CrossingType, E_Conversion = eToSim );
    const std::string& ConvertFromDiplomacy( E_Diplomacy, E_Conversion = eToSim );
    const std::string& ConvertFromEquipmentState( E_EquipmentState, E_Conversion = eToSim );
    const std::string& ConvertFromEventDockModes( E_EventDockModes, E_Conversion = eToTr );
    const std::string& ConvertFromEventTypes( E_EventTypes, E_Conversion = eToTr );
    const std::string& ConvertFromFireAvailability( E_FireAvailability, E_Conversion = eToSim );
    const std::string& ConvertFromForceRatioStatus( E_ForceRatioStatus, E_Conversion = eToSim );
    const std::string& ConvertFromGhostType( E_GhostType, E_Conversion = eToSim );
    const std::string& ConvertFromHumanLocation( E_HumanLocation, E_Conversion = eToSim );
    const std::string& ConvertFromHumanRank( E_HumanRank, E_Conversion = eToSim );
    const std::string& ConvertFromHumanState( E_HumanState, E_Conversion = eToSim );
    const std::string& ConvertFromHumanWound( E_HumanWound, E_Conversion = eToSim );
    const std::string& ConvertFromInjuriesSeriousness( E_InjuriesSeriousness, E_Conversion = eToSim );
    const std::string& ConvertFromLayerTypes( E_LayerTypes, E_Conversion = eToTr );
    const std::string& ConvertFromLightingType( E_LightingType, E_Conversion = eToSim );
    const std::string& ConvertFromLocation( E_Location, E_Conversion = eToSim );
    const std::string& ConvertFromLocationCategory( E_LocationCategory, E_Conversion = eToSim );
    const std::string& ConvertFromLocationType( E_LocationType, E_Conversion = eToSim );
    const std::string& ConvertFromMeetingEngagementStatus( E_MeetingEngagementStatus, E_Conversion = eToSim );
    const std::string& ConvertFromMissionType( E_MissionType, E_Conversion = eToTr );
    const std::string& ConvertFromModes( E_Modes, E_Conversion = eToSim );
    const std::string& ConvertFromNatureLevel( E_NatureLevel, E_Conversion = eToSim );
    const std::string& ConvertFromNbcState( E_NbcState, E_Conversion = eToSim );
    const std::string& ConvertFromObstacleActivation( E_ObstacleActivation, E_Conversion = eToSim );
    const std::string& ConvertFromOperationalStatus( E_OperationalStatus, E_Conversion = eToSim );
    const std::string& ConvertFromPerceptionLevel( E_PerceptionLevel, E_Conversion = eToSim );
    const std::string& ConvertFromPerceptionType( E_PerceptionType, E_Conversion = eToSim );
    const std::string& ConvertFromPopulationAttitude( E_PopulationAttitude, E_Conversion = eToSim );
    const std::string& ConvertFromPopulationRoe( E_PopulationRoe, E_Conversion = eToSim );
    const std::string& ConvertFromRoe( E_Roe, E_Conversion = eToSim );
    const std::string& ConvertFromUnitExperience( E_UnitExperience, E_Conversion = eToSim );
    const std::string& ConvertFromUnitMorale( E_UnitMorale, E_Conversion = eToSim );
    const std::string& ConvertFromUnitPosture( E_UnitPosture, E_Conversion = eToSim );
    const std::string& ConvertFromUnitStress( E_UnitStress, E_Conversion = eToSim );
    const std::string& ConvertFromUnitTiredness( E_UnitTiredness, E_Conversion = eToSim );
    const std::string& ConvertFromVisualisationScale( E_VisualisationScale, E_Conversion = eToSim );
    const std::string& ConvertFromWeatherType( E_WeatherType, E_Conversion = eToSim );

    const std::string& ConvertFromDotationType( sword::DotationType, E_Conversion );
    const std::string& ConvertFromKnowledgeMagicActionType( sword::KnowledgeMagicAction_Type, E_Conversion = eToTr );
    const std::string& ConvertFromLogFuneralHandlingStatus( sword::LogFuneralHandlingUpdate_EnumLogFuneralHandlingStatus, E_Conversion = eToTr );
    const std::string& ConvertFromLogisticLevel( sword::EnumLogisticLevel, E_Conversion = eToTr );
    const std::string& ConvertFromLogMaintenanceHandlingStatus( sword::LogMaintenanceHandlingUpdate_EnumLogMaintenanceHandlingStatus, E_Conversion = eToTr );
    const std::string& ConvertFromLogMedicalHandlingStatus( sword::LogMedicalHandlingUpdate_EnumLogMedicalHandlingStatus, E_Conversion = eToTr );
    const std::string& ConvertFromLogSupplyHandlingStatus( sword::LogSupplyHandlingUpdate_EnumLogSupplyHandlingStatus, E_Conversion = eToTr );
    const std::string& ConvertFromMagicActionType( sword::MagicAction_Type, E_Conversion = eToTr );
    const std::string& ConvertFromObjectMagicActionType( sword::ObjectMagicAction_Type, E_Conversion = eToTr );
    const std::string& ConvertFromUnitMagicActionType( sword::UnitMagicAction_Type, E_Conversion = eToTr );

        // ConvertTo functions
    E_AgentNbcSuit ConvertToAgentNbcSuit( const std::string&, E_Conversion = eToSim );
    E_AmmunitionType ConvertToAmmunitionType( const std::string&, E_Conversion = eToSim );
    E_AviationRange ConvertToAviationRange( const std::string&, E_Conversion = eToSim );
    E_BreakdownNTI ConvertToBreakdownNTI( const std::string&, E_Conversion = eToSim );
    E_BreakdownType ConvertToBreakdownType( const std::string&, E_Conversion = eToSim );
    E_CoordinateSystem ConvertToCoordinateSystem( const std::string&, E_Conversion = eToTr );
    E_CrossingType ConvertToCrossingType( const std::string&, E_Conversion = eToSim );
    E_Diplomacy ConvertToDiplomacy( const std::string&, E_Conversion = eToSim );
    E_EquipmentState ConvertToEquipmentState( const std::string&, E_Conversion = eToSim );
    E_EventDockModes ConvertToEventDockModes( const std::string&, E_Conversion = eToTr );
    E_EventTypes ConvertToEventTypes( const std::string&, E_Conversion = eToTr );
    E_FireAvailability ConvertToFireAvailability( const std::string&, E_Conversion = eToSim );
    E_ForceRatioStatus ConvertToForceRatioStatus( const std::string&, E_Conversion = eToSim );
    E_GhostType ConvertToGhostType( const std::string&, E_Conversion = eToSim );
    E_HumanLocation ConvertToHumanLocation( const std::string&, E_Conversion = eToSim );
    E_HumanRank ConvertToHumanRank( const std::string&, E_Conversion = eToSim );
    E_HumanState ConvertToHumanState( const std::string&, E_Conversion = eToSim );
    E_HumanWound ConvertToHumanWound( const std::string&, E_Conversion = eToSim );
    E_InjuriesSeriousness ConvertToInjuriesSeriousness( const std::string&, E_Conversion = eToSim );
    E_LayerTypes ConvertToLayerTypes( const std::string&, E_Conversion = eToSim );
    E_LightingType ConvertToLightingType( const std::string&, E_Conversion = eToSim );
    E_Location ConvertToLocation( const std::string&, E_Conversion = eToSim );
    E_LocationCategory ConvertToLocationCategory( const std::string&, E_Conversion = eToSim );
    E_LocationType ConvertToLocationType( const std::string&, E_Conversion = eToSim );
    E_MeetingEngagementStatus ConvertToMeetingEngagementStatus( const std::string&, E_Conversion = eToSim );
    E_MissionType ConvertToMissionType( const std::string&, E_Conversion = eToTr );
    E_Modes ConvertToModes( const std::string&, E_Conversion = eToSim );
    E_NatureLevel ConvertToNatureLevel( const std::string&, E_Conversion = eToSim );
    E_NbcState ConvertToNbcState( const std::string&, E_Conversion = eToSim );
    E_ObstacleActivation ConvertToObstacleActivation( const std::string&, E_Conversion = eToSim );
    E_OperationalStatus ConvertToOperationalStatus( const std::string&, E_Conversion = eToSim );
    E_PerceptionLevel ConvertToPerceptionLevel( const std::string&, E_Conversion = eToSim );
    E_PerceptionType ConvertToPerceptionType( const std::string&, E_Conversion = eToSim );
    E_PopulationAttitude ConvertToPopulationAttitude( const std::string&, E_Conversion = eToSim );
    E_PopulationRoe ConvertToPopulationRoe( const std::string&, E_Conversion = eToSim );
    E_Roe ConvertToRoe( const std::string&, E_Conversion = eToSim );
    E_UnitExperience ConvertToUnitExperience( const std::string&, E_Conversion = eToSim );
    E_UnitMorale ConvertToUnitMorale( const std::string&, E_Conversion = eToSim );
    E_UnitPosture ConvertToUnitPosture( const std::string&, E_Conversion = eToSim );
    E_UnitStress ConvertToUnitStress( const std::string&, E_Conversion = eToSim );
    E_UnitTiredness ConvertToUnitTiredness( const std::string&, E_Conversion = eToSim );
    E_VisualisationScale ConvertToVisualisationScale( const std::string&, E_Conversion = eToSim );
    E_WeatherType ConvertToWeatherType( const std::string&, E_Conversion = eToSim );

    sword::DotationType ConvertToDotationType( const std::string&, E_Conversion );
    sword::EnumLogisticLevel ConvertToLogisticLevel( const std::string&, E_Conversion = eToSim );
    sword::KnowledgeMagicAction_Type ConvertToKnowledgeMagicActionType( const std::string&, E_Conversion = eToSim );
    sword::LogFuneralHandlingUpdate_EnumLogFuneralHandlingStatus ConvertToLogFuneralHandlingStatus( const std::string&, E_Conversion = eToSim );
    sword::LogMaintenanceHandlingUpdate_EnumLogMaintenanceHandlingStatus ConvertToLogMaintenanceHandlingStatus( const std::string&, E_Conversion = eToSim );
    sword::LogMedicalHandlingUpdate_EnumLogMedicalHandlingStatus ConvertToLogMedicalHandlingStatus( const std::string&, E_Conversion = eToSim );
    sword::LogSupplyHandlingUpdate_EnumLogSupplyHandlingStatus ConvertToLogSupplyHandlingStatus( const std::string&, E_Conversion = eToSim );
    sword::MagicAction_Type ConvertToMagicActionType( const std::string&, E_Conversion = eToSim );
    sword::ObjectMagicAction_Type ConvertToObjectMagicActionType( const std::string&, E_Conversion = eToSim );
    sword::UnitMagicAction_Type ConvertToUnitMagicActionType( const std::string&, E_Conversion = eToSim );
}

#endif // __ENT_Tr_h_
