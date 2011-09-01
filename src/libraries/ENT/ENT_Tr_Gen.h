// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ENT_Tr_Gen_h_
#define __ENT_Tr_Gen_h_

#include "ENT_Tr_ABC.h"
#include "ENT_Enums_Gen.h"

// =============================================================================
/** @class  AGR_ENT_Tr_Skeleton
    @brief  AGR_ENT_Tr_Skeleton
*/
// Created: APE 2005-02-09
// =============================================================================
class ENT_Tr : public ENT_Tr_ABC
{
public:
    static void InitTranslations();

public:
    // ConvertFrom functions
    static const std::string& ConvertFromLocationType( E_LocationType, E_Conversion = eToSim );
    static const std::string& ConvertFromDotationFamily( E_DotationFamily, E_Conversion = eToSim );
    static const std::string& ConvertFromAmmunitionFamily( E_AmmunitionFamily, E_Conversion = eToSim );
    static const std::string& ConvertFromNatureLevel( E_NatureLevel, E_Conversion = eToSim );
    static const std::string& ConvertFromDiplomacy( E_Diplomacy, E_Conversion = eToSim );
    static const std::string& ConvertFromForceRatioStatus( E_ForceRatioStatus, E_Conversion = eToSim );
    static const std::string& ConvertFromMeetingEngagementStatus( E_MeetingEngagementStatus, E_Conversion = eToSim );
    static const std::string& ConvertFromOperationalStatus( E_OperationalStatus, E_Conversion = eToSim );
    static const std::string& ConvertFromRoe( E_Roe, E_Conversion = eToSim );
    static const std::string& ConvertFromUnitPosture( E_UnitPosture, E_Conversion = eToSim );
    static const std::string& ConvertFromFireAvailability( E_FireAvailability, E_Conversion = eToSim );
    static const std::string& ConvertFromPopulationRoe( E_PopulationRoe, E_Conversion = eToSim );
    static const std::string& ConvertFromUnitTiredness( E_UnitTiredness, E_Conversion = eToSim );
    static const std::string& ConvertFromUnitMorale( E_UnitMorale, E_Conversion = eToSim );
    static const std::string& ConvertFromUnitExperience( E_UnitExperience, E_Conversion = eToSim );
    static const std::string& ConvertFromUnitStress( E_UnitStress, E_Conversion = eToSim );
    static const std::string& ConvertFromLightingType( E_LightingType, E_Conversion = eToSim );
    static const std::string& ConvertFromWeatherType( E_WeatherType, E_Conversion = eToSim );
    static const std::string& ConvertFromLogMaintenanceHandlingStatus( E_LogMaintenanceHandlingStatus, E_Conversion = eToSim );
    static const std::string& ConvertFromLogSupplyHandlingStatus( E_LogSupplyHandlingStatus, E_Conversion = eToSim );
    static const std::string& ConvertFromLogMedicalHandlingStatus( E_LogMedicalHandlingStatus, E_Conversion = eToSim );
    static const std::string& ConvertFromDemolitionTargetType( E_DemolitionTargetType, E_Conversion = eToSim );
    static const std::string& ConvertFromPopulationAttitude( E_PopulationAttitude, E_Conversion = eToSim );
    static const std::string& ConvertFromLocation( E_Location, E_Conversion = eToSim );
    static const std::string& ConvertFromActionParameter( E_ActionParameter, E_Conversion = eToSim );
    static const std::string& ConvertFromCrossingType( E_CrossingType, E_Conversion = eToSim );
    static const std::string& ConvertFromHumanWound( E_HumanWound, E_Conversion = eToSim );
    static const std::string& ConvertFromHumanRank( E_HumanRank, E_Conversion = eToSim );
    static const std::string& ConvertFromHumanState( E_HumanState, E_Conversion = eToSim );
    static const std::string& ConvertFromHumanLocation( E_HumanLocation, E_Conversion = eToSim );
    static const std::string& ConvertFromEquipmentState( E_EquipmentState, E_Conversion = eToSim );
    static const std::string& ConvertFromInjuriesSeriousness( E_InjuriesSeriousness, E_Conversion = eToSim );
    static const std::string& ConvertFromStockCategory( E_StockCategory, E_Conversion = eToSim );

    // ConvertTo functions
    static E_LocationType ConvertToLocationType( const std::string& );
    static E_DotationFamily ConvertToDotationFamily( const std::string& );
    static E_AmmunitionFamily ConvertToAmmunitionFamily( const std::string& );
    static E_NatureLevel ConvertToNatureLevel( const std::string& );
    static E_Diplomacy ConvertToDiplomacy( const std::string& );
    static E_ForceRatioStatus ConvertToForceRatioStatus( const std::string& );
    static E_MeetingEngagementStatus ConvertToMeetingEngagementStatus( const std::string& );
    static E_OperationalStatus ConvertToOperationalStatus( const std::string& );
    static E_Roe ConvertToRoe( const std::string& );
    static E_UnitPosture ConvertToUnitPosture( const std::string& );
    static E_FireAvailability ConvertToFireAvailability( const std::string& );
    static E_PopulationRoe ConvertToPopulationRoe( const std::string& );
    static E_UnitTiredness ConvertToUnitTiredness( const std::string& );
    static E_UnitMorale ConvertToUnitMorale( const std::string& );
    static E_UnitExperience ConvertToUnitExperience( const std::string& );
    static E_UnitStress ConvertToUnitStress( const std::string& );
    static E_WeatherType ConvertToWeatherType( const std::string& );
    static E_LogMaintenanceHandlingStatus ConvertToLogMaintenanceHandlingStatus( const std::string& );
    static E_LogSupplyHandlingStatus ConvertToLogSupplyHandlingStatus( const std::string& );
    static E_LogMedicalHandlingStatus ConvertToLogMedicalHandlingStatus( const std::string& );
    static E_DemolitionTargetType ConvertToDemolitionTargetType( const std::string& );
    static E_PopulationAttitude ConvertToPopulationAttitude( const std::string& );
    static E_Location ConvertToLocation( const std::string& );
    static E_ActionParameter ConvertToActionParameter( const std::string& );
    static E_CrossingType ConvertToCrossingType( const std::string& );
    static E_HumanWound ConvertToHumanWound( const std::string& );
    static E_HumanRank ConvertToHumanRank( const std::string& );
    static E_HumanState ConvertToHumanState( const std::string& );
    static E_HumanLocation ConvertToHumanLocation( const std::string& );
    static E_EquipmentState ConvertToEquipmentState( const std::string& );
    static E_InjuriesSeriousness ConvertToInjuriesSeriousness( const std::string& );
    static E_StockCategory ConvertToStockCategory ( const std::string& );

private:
    // Typedefs
    typedef ENT_Tr::converter<E_LocationType> T_ConverterLocationType;
    typedef ENT_Tr::converter<E_DotationFamily> T_ConverterDotationFamily;
    typedef ENT_Tr::converter<E_AmmunitionFamily> T_ConverterAmmunitionFamily;
    typedef ENT_Tr::converter<E_NatureLevel> T_ConverterNatureLevel;
    typedef ENT_Tr::converter<E_Diplomacy> T_ConverterDiplomacy;
    typedef ENT_Tr::converter<E_ForceRatioStatus> T_ConverterForceRatioStatus;
    typedef ENT_Tr::converter<E_MeetingEngagementStatus> T_ConverterMeetingEngagementStatus;
    typedef ENT_Tr::converter<E_OperationalStatus> T_ConverterOperationalStatus;
    typedef ENT_Tr::converter<E_Roe> T_ConverterRoe;
    typedef ENT_Tr::converter<E_UnitPosture> T_ConverterUnitPosture;
    typedef ENT_Tr::converter<E_FireAvailability> T_ConverterFireAvailability;
    typedef ENT_Tr::converter<E_PopulationRoe> T_ConverterPopulationRoe;
    typedef ENT_Tr::converter<E_UnitTiredness> T_ConverterUnitTiredness;
    typedef ENT_Tr::converter<E_UnitMorale> T_ConverterUnitMorale;
    typedef ENT_Tr::converter<E_UnitExperience> T_ConverterUnitExperience;
    typedef ENT_Tr::converter<E_UnitStress> T_ConverterUnitStress;
    typedef ENT_Tr::converter<E_LightingType> T_ConverterLightingType;
    typedef ENT_Tr::converter<E_WeatherType> T_ConverterWeatherType;
    typedef ENT_Tr::converter<E_LogMaintenanceHandlingStatus> T_ConverterLogMaintenanceHandlingStatus;
    typedef ENT_Tr::converter<E_LogSupplyHandlingStatus> T_ConverterLogSupplyHandlingStatus;
    typedef ENT_Tr::converter<E_LogMedicalHandlingStatus> T_ConverterLogMedicalHandlingStatus;
    typedef ENT_Tr::converter<E_DemolitionTargetType> T_ConverterDemolitionTargetType;
    typedef ENT_Tr::converter<E_PopulationAttitude> T_ConverterPopulationAttitude;
    typedef ENT_Tr::converter<E_Location> T_ConverterLocation;
    typedef ENT_Tr::converter<E_ActionParameter> T_ConverterActionParameter;
    typedef ENT_Tr::converter<E_CrossingType> T_ConverterCrossingType;
    typedef ENT_Tr::converter<E_HumanWound> T_ConverterHumanWound;
    typedef ENT_Tr::converter<E_HumanRank> T_ConverterHumanRank;
    typedef ENT_Tr::converter<E_HumanState> T_ConverterHumanState;
    typedef ENT_Tr::converter<E_HumanLocation> T_ConverterHumanLocation;
    typedef ENT_Tr::converter<E_EquipmentState> T_ConverterEquipmentState;
    typedef ENT_Tr::converter<E_InjuriesSeriousness> T_ConverterInjuriesSeriousness;
    typedef ENT_Tr::converter<E_StockCategory> T_ConverterStockCategory;

    // Converters
    static T_ConverterLocationType LocationTypeConverter_ [];
    static T_ConverterDotationFamily DotationFamilyConverter_ [];
    static T_ConverterAmmunitionFamily AmmunitionFamilyConverter_ [];
    static T_ConverterNatureLevel NatureLevelConverter_ [];
    static T_ConverterDiplomacy DiplomacyConverter_ [];
    static T_ConverterForceRatioStatus ForceRatioStatusConverter_ [];
    static T_ConverterMeetingEngagementStatus MeetingEngagementStatusConverter_ [];
    static T_ConverterOperationalStatus OperationalStatusConverter_ [];
    static T_ConverterRoe RoeConverter_ [];
    static T_ConverterUnitPosture UnitPostureConverter_ [];
    static T_ConverterFireAvailability FireAvailabilityConverter_ [];
    static T_ConverterPopulationRoe PopulationRoeConverter_ [];
    static T_ConverterUnitTiredness UnitTirednessConverter_ [];
    static T_ConverterUnitMorale UnitMoraleConverter_ [];
    static T_ConverterUnitExperience UnitExperienceConverter_ [];
    static T_ConverterUnitStress UnitStressConverter_ [];
    static T_ConverterLightingType LightingTypeConverter_ [];
    static T_ConverterWeatherType WeatherTypeConverter_ [];
    static T_ConverterLogMaintenanceHandlingStatus LogMaintenanceHandlingStatusConverter_ [];
    static T_ConverterLogSupplyHandlingStatus LogSupplyHandlingStatusConverter_ [];
    static T_ConverterLogMedicalHandlingStatus LogMedicalHandlingStatusConverter_ [];
    static T_ConverterDemolitionTargetType DemolitionTargetTypeConverter_ [];
    static T_ConverterPopulationAttitude PopulationAttitudeConverter_ [];
    static T_ConverterLocation LocationConverter_ [];
    static T_ConverterActionParameter ActionParameterConverter_ [];
    static T_ConverterCrossingType CrossingTypeConverter_ [];
    static T_ConverterHumanWound HumanWoundConverter_ [];
    static T_ConverterHumanRank HumanRankConverter_ [];
    static T_ConverterHumanState HumanStateConverter_ [];
    static T_ConverterHumanLocation HumanLocationConverter_ [];
    static T_ConverterEquipmentState EquipmentStateConverter_ [];
    static T_ConverterInjuriesSeriousness InjuriesSeriousnessConverter_ [];
    static T_ConverterStockCategory stockCategoryConverter_[];
};

#endif // __ENT_Tr_Gen_h_
