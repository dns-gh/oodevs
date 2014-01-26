// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-02-18 $
// $Archive: /MVW_v10/Build/SDK/ent/src/ENT_Tr.h $
// $Author: Ape $
// $Modtime: 18/02/05 17:32 $
// $Revision: 1 $
// $Workfile: ENT_Tr.h $
//
// *****************************************************************************

#ifndef __ENT_Tr_h_
#define __ENT_Tr_h_

#include "ENT_Enums.h"
#include <string>

namespace sword
{
    enum MagicAction_Type;
    enum UnitMagicAction_Type;
    enum KnowledgeMagicAction_Type;
    enum ObjectMagicAction_Type;
    enum EnumLogisticLevel;
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
    const std::string& ConvertFromLocationType( E_LocationType, E_Conversion = eToSim );
    const std::string& ConvertFromDotationFamily( E_DotationFamily, E_Conversion = eToSim );
    const std::string& ConvertFromAmmunitionType( E_AmmunitionType, E_Conversion = eToSim );
    const std::string& ConvertFromNatureLevel( E_NatureLevel, E_Conversion = eToSim );
    const std::string& ConvertFromDiplomacy( E_Diplomacy, E_Conversion = eToSim );
    const std::string& ConvertFromForceRatioStatus( E_ForceRatioStatus, E_Conversion = eToSim );
    const std::string& ConvertFromMeetingEngagementStatus( E_MeetingEngagementStatus, E_Conversion = eToSim );
    const std::string& ConvertFromOperationalStatus( E_OperationalStatus, E_Conversion = eToSim );
    const std::string& ConvertFromRoe( E_Roe, E_Conversion = eToSim );
    const std::string& ConvertFromUnitPosture( E_UnitPosture, E_Conversion = eToSim );
    const std::string& ConvertFromFireAvailability( E_FireAvailability, E_Conversion = eToSim );
    const std::string& ConvertFromPopulationRoe( E_PopulationRoe, E_Conversion = eToSim );
    const std::string& ConvertFromUnitTiredness( E_UnitTiredness, E_Conversion = eToSim );
    const std::string& ConvertFromUnitMorale( E_UnitMorale, E_Conversion = eToSim );
    const std::string& ConvertFromUnitExperience( E_UnitExperience, E_Conversion = eToSim );
    const std::string& ConvertFromUnitStress( E_UnitStress, E_Conversion = eToSim );
    const std::string& ConvertFromLightingType( E_LightingType, E_Conversion = eToSim );
    const std::string& ConvertFromWeatherType( E_WeatherType, E_Conversion = eToSim );
    const std::string& ConvertFromLogMaintenanceHandlingStatus( E_LogMaintenanceHandlingStatus, E_Conversion = eToSim );
    const std::string& ConvertFromLogSupplyHandlingStatus( E_LogSupplyHandlingStatus, E_Conversion = eToSim );
    const std::string& ConvertFromLogMedicalHandlingStatus( E_LogMedicalHandlingStatus, E_Conversion = eToSim );
    const std::string& ConvertFromLogFuneralHandlingStatus( E_LogFuneralHandlingStatus, E_Conversion = eToSim );
    const std::string& ConvertFromObstacleActivation( E_ObstacleActivation, E_Conversion = eToSim );
    const std::string& ConvertFromPopulationAttitude( E_PopulationAttitude, E_Conversion = eToSim );
    const std::string& ConvertFromLocation( E_Location, E_Conversion = eToSim );
    const std::string& ConvertFromCrossingType( E_CrossingType, E_Conversion = eToSim );
    const std::string& ConvertFromHumanWound( E_HumanWound, E_Conversion = eToSim );
    const std::string& ConvertFromHumanRank( E_HumanRank, E_Conversion = eToSim );
    const std::string& ConvertFromHumanState( E_HumanState, E_Conversion = eToSim );
    const std::string& ConvertFromHumanLocation( E_HumanLocation, E_Conversion = eToSim );
    const std::string& ConvertFromEquipmentState( E_EquipmentState, E_Conversion = eToSim );
    const std::string& ConvertFromInjuriesSeriousness( E_InjuriesSeriousness, E_Conversion = eToSim );
    const std::string& ConvertFromBreakdownType( E_BreakdownType, ENT_Tr::E_Conversion = ENT_Tr::eToSim );
    const std::string& ConvertFromBreakdownNTI( E_BreakdownNTI, ENT_Tr::E_Conversion = ENT_Tr::eToSim );
    const std::string& ConvertFromGhostType( E_GhostType, E_Conversion = eToSim );
    const std::string& ConvertFromNbcState( E_NbcState, E_Conversion = eToSim );
    const std::string& ConvertFromModes( E_Modes, E_Conversion = eToSim );
    const std::string& ConvertFromAgentNbcSuit( E_AgentNbcSuit, E_Conversion = eToSim );
    const std::string& ConvertFromAviationRange( E_AviationRange, E_Conversion = eToSim );
    const std::string& ConvertFromLayerType( E_LayerTypes, E_Conversion = eToTr );
    const std::string& ConvertFromEventType( E_EventTypes, E_Conversion = eToTr );
    const std::string& ConvertFromMissionType( E_MissionType, E_Conversion = eToTr );
    const std::string& ConvertFromEventDockModes( E_EventDockModes, E_Conversion = eToTr );
    const std::string& ConvertFromMagicActionType( sword::MagicAction_Type, E_Conversion = eToTr );
    const std::string& ConvertFromUnitMagicActionType( sword::UnitMagicAction_Type, E_Conversion = eToTr );
    const std::string& ConvertFromKnowledgeMagicActionType( sword::KnowledgeMagicAction_Type, E_Conversion = eToTr );
    const std::string& ConvertFromObjectMagicActionType( sword::ObjectMagicAction_Type, E_Conversion = eToTr );
    const std::string& ConvertFromLogisticLevel( sword::EnumLogisticLevel, E_Conversion = eToTr );

    // ConvertTo functions
    E_LocationType ConvertToLocationType( const std::string& );
    E_DotationFamily ConvertToDotationFamily( const std::string& );
    E_AmmunitionType ConvertToAmmunitionType( const std::string& );
    E_NatureLevel ConvertToNatureLevel( const std::string& );
    E_Diplomacy ConvertToDiplomacy( const std::string& );
    E_ForceRatioStatus ConvertToForceRatioStatus( const std::string& );
    E_MeetingEngagementStatus ConvertToMeetingEngagementStatus( const std::string& );
    E_OperationalStatus ConvertToOperationalStatus( const std::string& );
    E_Roe ConvertToRoe( const std::string& );
    E_UnitPosture ConvertToUnitPosture( const std::string& );
    E_FireAvailability ConvertToFireAvailability( const std::string& );
    E_PopulationRoe ConvertToPopulationRoe( const std::string& );
    E_UnitTiredness ConvertToUnitTiredness( const std::string& );
    E_UnitMorale ConvertToUnitMorale( const std::string& );
    E_UnitExperience ConvertToUnitExperience( const std::string& );
    E_UnitStress ConvertToUnitStress( const std::string& );
    E_LightingType ConvertToLightingType( const std::string& );
    E_WeatherType ConvertToWeatherType( const std::string& );
    E_LogMaintenanceHandlingStatus ConvertToLogMaintenanceHandlingStatus( const std::string& );
    E_LogSupplyHandlingStatus ConvertToLogSupplyHandlingStatus( const std::string& );
    E_LogMedicalHandlingStatus ConvertToLogMedicalHandlingStatus( const std::string& );
    E_LogFuneralHandlingStatus ConvertToLogFuneralHandlingStatus( const std::string& );
    E_ObstacleActivation ConvertToObstacleActivation( const std::string&, E_Conversion = eToSim );
    E_PopulationAttitude ConvertToPopulationAttitude( const std::string& );
    E_Location ConvertToLocation( const std::string& );
    E_CrossingType ConvertToCrossingType( const std::string& );
    E_HumanWound ConvertToHumanWound( const std::string&, E_Conversion = eToSim );
    E_HumanRank ConvertToHumanRank( const std::string& );
    E_HumanState ConvertToHumanState( const std::string& );
    E_HumanLocation ConvertToHumanLocation( const std::string& );
    E_EquipmentState ConvertToEquipmentState( const std::string& );
    E_InjuriesSeriousness ConvertToInjuriesSeriousness( const std::string& );
    E_BreakdownType ConvertToBreakdownType( const std::string& );
    E_BreakdownNTI ConvertToBreakdownNTI( const std::string& );
    E_GhostType ConvertToGhostType( const std::string& );
    E_NbcState ConvertToNbcState( const std::string& );
    E_Modes ConvertToModes( const std::string& );
    E_AgentNbcSuit ConvertToAgentNbcSuit( const std::string& );
    E_AviationRange ConvertToAviationRange( const std::string& );
    E_LayerTypes ConvertToLayerType( const std::string& );
    E_EventTypes ConvertToEventType( const std::string&, E_Conversion = eToTr );
    E_MissionType ConvertToMissionType( const std::string&, E_Conversion = eToTr );
    E_EventDockModes ConvertToEventDockModes( const std::string&, E_Conversion = eToTr );
    sword::MagicAction_Type ConvertToMagicActionType( const std::string&, E_Conversion = eToSim );
    sword::UnitMagicAction_Type ConvertToUnitMagicActionType( const std::string&, E_Conversion = eToSim );
    sword::KnowledgeMagicAction_Type ConvertToKnowledgeMagicActionType( const std::string&, E_Conversion = eToSim );
    sword::ObjectMagicAction_Type ConvertToObjectMagicActionType( const std::string&, E_Conversion = eToSim );
    sword::EnumLogisticLevel ConvertToLogisticLevel( const std::string&, E_Conversion = eToSim );
}

#endif // __ENT_Tr_h_
