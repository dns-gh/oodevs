// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Tr_h_
#define __ADN_Tr_h_

#include "ENT/ENT_Tr_ABC.h"
#include "ADN_Enums.h"
#include "clients_kernel/Tools.h"

// =============================================================================
/** @class  ADN_Tr
@brief  ADN_Tr
*/
// Created: APE 2005-02-17
// =============================================================================
class ADN_Tr
    : public ENT_Tr_ABC
{
public:
    //! @name Convert From functions
    //@{
    static const std::string& ConvertFromLocation ( E_Location , E_Conversion = eToSim );
    static const std::string& ConvertFromConsumptionType ( E_ConsumptionType, E_Conversion = eToSim );
    static const std::string& ConvertFromTimeCategory( E_TimeCategory, E_Conversion = eToSim );
    static const std::string& ConvertFromVisionObject( E_VisionObject, E_Conversion = eToSim );
    static const std::string& ConvertFromKeyPoint( E_KeyPoint, E_Conversion = eToSim );
    static const std::string& ConvertFromAgentTypePion( E_AgentTypePion, E_Conversion = eToSim );
    static const std::string& ConvertFromAgentTypeAutomate( E_AgentTypeAutomate, E_Conversion = eToSim );
    static const std::string& ConvertFromNatureAtlasType( E_NatureAtlasType, E_Conversion = eToSim );
    static const std::string& ConvertFromSpeedImpact( E_SpeedImpact, E_Conversion = eToSim );
    static const std::string& ConvertFromCrossingHeight( E_CrossingHeight, E_Conversion = eToSim );
    static const std::string& ConvertFromSensorWeatherModifiers( E_SensorWeatherModifiers, E_Conversion = eToSim );
    static const std::string& ConvertFromDoctorSkills( E_DoctorSkills, E_Conversion = eToSim );
    static const std::string& ConvertFromProtectionType( E_ProtectionType, E_Conversion = eToSim );
    static const std::string& ConvertFromBreakdownType( E_BreakdownType, E_Conversion = eToSim );
    static const std::string& ConvertFromBreakdownNTI( E_BreakdownNTI, E_Conversion = eToSim );
    static const std::string& ConvertFromRadarType( E_RadarType, E_Conversion = eToSim );
    static const std::string& ConvertFromEquipmentState( E_EquipmentState_ADN, E_Conversion = eToSim );
    static const std::string& ConvertFromTypeMunitionTirIndirect( E_TypeMunitionTirIndirect, E_Conversion = eToSim );
    static const std::string& ConvertFromMissionParameterType( E_MissionParameterType, E_Conversion = eToSim );
    static const std::string& ConvertFromSupplyConvoyType( E_SupplyConvoyType, E_Conversion = eToSim );
    //@}

    //! @name Convert To functions
    //@{
    static E_Location        ConvertToLocation         ( const std::string& );
    static E_ConsumptionType ConvertToConsumptionType  ( const std::string& );
    static E_TimeCategory    ConvertToTimeCategory     ( const std::string& );
    static E_VisionObject    ConvertToVisionObject     ( const std::string& );
    static E_KeyPoint        ConvertToKeyPoint         ( const std::string& );
    static E_AgentTypePion   ConvertToAgentTypePion    ( const std::string& );
    static E_AgentTypeAutomate   ConvertToAgentTypeAutomate( const std::string& );
    static E_NatureAtlasType ConvertToNatureAtlasType  ( const std::string& );
    static E_SpeedImpact     ConvertToSpeedImpact      ( const std::string& );
    static E_CrossingHeight  ConvertToCrossingHeight   ( const std::string& );
    static E_SensorWeatherModifiers ConvertToSensorWeatherModifiers( const std::string& );
    static E_DoctorSkills    ConvertToDoctorSkills     ( const std::string& );
    static E_ProtectionType  ConvertToProtectionType   ( const std::string& );
    static E_BreakdownType   ConvertToBreakdownType    ( const std::string& );
    static E_BreakdownNTI    ConvertToBreakdownNTI     ( const std::string& );
    static E_RadarType       ConvertToRadarType        ( const std::string& );
    static E_EquipmentState_ADN  ConvertToEquipmentState( const std::string& );
    static E_TypeMunitionTirIndirect ConvertToTypeMunitionTirIndirect( const std::string& );
    static E_MissionParameterType ConvertToMissionParameterType( const std::string& );
    static E_SupplyConvoyType   ConvertToSupplyConvoyType( const std::string& );
    //@}

    static void InitTranslations();

public:
    //! @name typdefs
    //@{
    typedef converter<E_Location>                   T_ConverterLocation;
    typedef converter<E_ConsumptionType>            T_ConverterConsumptionType;
    typedef converter<E_TimeCategory>               T_ConverterTimeCategory;
    typedef converter<E_VisionObject>               T_ConverterVisionObject;
    typedef converter<E_KeyPoint>                   T_ConverterKeyPoint;
    typedef converter<E_AgentTypePion>              T_ConverterAgentTypePion;
    typedef converter<E_AgentTypeAutomate>          T_ConverterAgentTypeAutomate;
    typedef converter<E_NatureAtlasType>            T_ConverterNatureAtlasType;
    typedef converter<E_SpeedImpact>                T_ConverterSpeedImpact;
    typedef converter<E_CrossingHeight>             T_ConverterCrossingHeight;
    typedef converter<E_SensorWeatherModifiers>     T_ConverterSensorWeatherModifiers;
    typedef converter<E_DoctorSkills>               T_ConverterDoctorSkills;
    typedef converter<E_ProtectionType>             T_ConverterProtectionType;
    typedef converter<E_BreakdownType>              T_ConverterBreakdownType;
    typedef converter<E_BreakdownNTI>               T_ConverterBreakdownNTI;
    typedef converter<E_RadarType>                  T_ConverterRadarType;
    typedef converter<E_EquipmentState_ADN>         T_ConverterEquipmentState;
    typedef converter<E_TypeMunitionTirIndirect>    T_ConverterTypeMunitionTirIndirect;
    typedef converter<E_MissionParameterType>       T_ConverterMissionParameterType;
    typedef converter<E_SupplyConvoyType>           T_ConverterSupplyConvoyType;
    //@}

    //! @name Convertors
    //@{
    static T_ConverterConsumptionType   consumptionTypeConverter_[];
    static T_ConverterTimeCategory      timeCategoryConverter_[];
    static T_ConverterVisionObject      visionObjectConverter_[];
    static T_ConverterKeyPoint          keyPointConverter_[];
    static T_ConverterAgentTypePion     agentTypePionConverter_[];
    static T_ConverterAgentTypeAutomate agentTypeAutomateConverter_[];
    static T_ConverterNatureAtlasType   natureAtlasTypeConverter_[];
    static T_ConverterSpeedImpact       speedImpactConverter_[];
    static T_ConverterCrossingHeight    crossingHeightConverter_[];
    static T_ConverterSensorWeatherModifiers sensorWeatherModifiersConverter_[];
    static T_ConverterDoctorSkills      doctorSkillsConverter_[];
    static T_ConverterProtectionType    protectionTypeConverter_[];
    static T_ConverterBreakdownType     breakdownTypeConverter_[];
    static T_ConverterBreakdownNTI      breakdownNTIConverter_[];
    static T_ConverterRadarType         radarTypeConverter_[];
    static T_ConverterEquipmentState    equipmentStateConverter_[];
    static T_ConverterTypeMunitionTirIndirect munitionTirIndirectConverter_ [];
    static T_ConverterMissionParameterType missionParameterTypeConverter_ [];
    static T_ConverterSupplyConvoyType  supplyConvoyTypeConverter_[];
    //@}
};

#endif // __ADN_Tr_h_