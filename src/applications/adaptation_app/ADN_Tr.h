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

#include "ENT/ENT_Tr.h"
#include "ADN_Enums.h"
#include "clients_kernel/Tools.h"

namespace ADN_Tr
{
    //! @name Initializer
    //@{
    void InitTranslations();
    //@}

    //! @name Convert From functions
    //@{
    const std::string& ConvertFromConsumptionType( E_ConsumptionType, ENT_Tr::E_Conversion = ENT_Tr::eToSim );
    const std::string& ConvertFromVisionObject( E_VisionObject, ENT_Tr::E_Conversion = ENT_Tr::eToSim );
    const std::string& ConvertFromKeyPoint( E_KeyPoint, ENT_Tr::E_Conversion = ENT_Tr::eToSim );
    const std::string& ConvertFromAgentTypePion( E_AgentTypePion, ENT_Tr::E_Conversion = ENT_Tr::eToSim );
    const std::string& ConvertFromAgentTypeAutomate( E_AgentTypeAutomate, ENT_Tr::E_Conversion = ENT_Tr::eToSim );
    const std::string& ConvertFromNatureAtlasType( E_NatureAtlasType, ENT_Tr::E_Conversion = ENT_Tr::eToSim );
    const std::string& ConvertFromSpeedImpact( E_SpeedImpact, ENT_Tr::E_Conversion = ENT_Tr::eToSim );
    const std::string& ConvertFromCrossingHeight( E_CrossingHeight, ENT_Tr::E_Conversion = ENT_Tr::eToSim );
    const std::string& ConvertFromSensorWeatherModifiers( E_SensorWeatherModifiers, ENT_Tr::E_Conversion = ENT_Tr::eToSim );
    const std::string& ConvertFromDoctorSkills( E_DoctorSkills, ENT_Tr::E_Conversion = ENT_Tr::eToSim );
    const std::string& ConvertFromProtectionType( E_ProtectionType, ENT_Tr::E_Conversion = ENT_Tr::eToSim );
    const std::string& ConvertFromRadarType( E_RadarType, ENT_Tr::E_Conversion = ENT_Tr::eToSim );
    const std::string& ConvertFromEquipmentState_ADN( E_EquipmentState_ADN, ENT_Tr::E_Conversion = ENT_Tr::eToSim );
    const std::string& ConvertFromTypeMunitionTirIndirect( E_TypeMunitionTirIndirect, ENT_Tr::E_Conversion = ENT_Tr::eToSim );
    const std::string& ConvertFromMissionParameterType( E_MissionParameterType, ENT_Tr::E_Conversion = ENT_Tr::eToSim );
    const std::string& ConvertFromSupplyConvoyType( E_SupplyConvoyType, ENT_Tr::E_Conversion = ENT_Tr::eToSim );
    const std::string& ConvertFromConstructorType( E_ConstructorType, ENT_Tr::E_Conversion = ENT_Tr::eToSim );
    const std::string& ConvertFromPropagationModel( E_PropagationModel, ENT_Tr::E_Conversion = ENT_Tr::eToSim );
    const std::string& ConvertFromWorkspaceElement( E_WorkspaceElements, ENT_Tr::E_Conversion = ENT_Tr::eToTr );
    const std::string& ConvertFromDays( E_Days, ENT_Tr::E_Conversion = ENT_Tr::eToTr );
    const std::string& ConvertFromContextParameters( E_ContextParameters, ENT_Tr::E_Conversion = ENT_Tr::eToTr );
    const std::string& ConvertFromEntityType( E_EntityType, ENT_Tr::E_Conversion = ENT_Tr::eToTr );
    //@}

    //! @name Convert To functions
    //@{
    E_ConsumptionType         ConvertToConsumptionType  ( const std::string& );
    E_VisionObject            ConvertToVisionObject     ( const std::string& );
    E_KeyPoint                ConvertToKeyPoint         ( const std::string& );
    E_AgentTypePion           ConvertToAgentTypePion    ( const std::string& );
    E_AgentTypeAutomate       ConvertToAgentTypeAutomate( const std::string& );
    E_NatureAtlasType         ConvertToNatureAtlasType  ( const std::string& );
    E_SpeedImpact             ConvertToSpeedImpact      ( const std::string& );
    E_CrossingHeight          ConvertToCrossingHeight   ( const std::string& );
    E_SensorWeatherModifiers  ConvertToSensorWeatherModifiers( const std::string& );
    E_DoctorSkills            ConvertToDoctorSkills     ( const std::string& );
    E_ProtectionType          ConvertToProtectionType   ( const std::string& );
    E_RadarType               ConvertToRadarType        ( const std::string& );
    E_EquipmentState_ADN      ConvertToEquipmentState_ADN( const std::string& );
    E_TypeMunitionTirIndirect ConvertToTypeMunitionTirIndirect( const std::string& );
    E_MissionParameterType    ConvertToMissionParameterType( const std::string& );
    E_SupplyConvoyType        ConvertToSupplyConvoyType( const std::string& );
    E_ConstructorType         ConvertToConstructorType( const std::string& );
    E_PropagationModel        ConvertToPropagationModel( const std::string& );
    E_WorkspaceElements       ConvertToWorkspaceElements( const std::string& );
    E_Days                    ConvertToDays( const std::string& );
    E_ContextParameters       ConvertToContextParameters( const std::string& );
    E_EntityType              ConvertToEntityType( const std::string& );
    //@}
};

#endif // __ADN_Tr_h_
