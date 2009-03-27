// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-02-09 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_ENT_Tr_Skeleton.h $
// $Author: Ape $
// $Modtime: 3/03/05 16:59 $
// $Revision: 3 $
// $Workfile: AGR_ENT_Tr_Skeleton.h $
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
class ENT_Tr
: public ENT_Tr_ABC
{
public:
    static void InitTranslations();

public:
    // ConvertFrom functions 
    static const std::string& ConvertFromChangeHierarchyErrorCode( E_ChangeHierarchyErrorCode, E_Conversion = eToSim );
    static const std::string& ConvertFromLocationType( E_LocationType, E_Conversion = eToSim );
    static const std::string& ConvertFromDotationFamily( E_DotationFamily, E_Conversion = eToSim );
    static const std::string& ConvertFromAmmunitionFamily( E_AmmunitionFamily, E_Conversion = eToSim );
    static const std::string& ConvertFromUnitIdentificationLevel( E_UnitIdentificationLevel, E_Conversion = eToSim );
    static const std::string& ConvertFromNatureLevel( E_NatureLevel, E_Conversion = eToSim );
    static const std::string& ConvertFromHumanWound( E_HumanWound, E_Conversion = eToSim );
    static const std::string& ConvertFromHumanRank( E_HumanRank, E_Conversion = eToSim );
    static const std::string& ConvertFromDiplomacy( E_Diplomacy, E_Conversion = eToSim );
    static const std::string& ConvertFromChangeDiplomacyErrorCode( E_ChangeDiplomacyErrorCode, E_Conversion = eToSim );
    static const std::string& ConvertFromForceRatioStatus( E_ForceRatioStatus, E_Conversion = eToSim );
    static const std::string& ConvertFromMeetingEngagementStatus( E_MeetingEngagementStatus, E_Conversion = eToSim );
    static const std::string& ConvertFromOperationalStatus( E_OperationalStatus, E_Conversion = eToSim );
    static const std::string& ConvertFromRoe( E_Roe, E_Conversion = eToSim );
    static const std::string& ConvertFromUnitErrorCode( E_UnitErrorCode, E_Conversion = eToSim );
    static const std::string& ConvertFromUnitPosture( E_UnitPosture, E_Conversion = eToSim );
    static const std::string& ConvertFromFireAvailability( E_FireAvailability, E_Conversion = eToSim );
    static const std::string& ConvertFromPopulationRoe( E_PopulationRoe, E_Conversion = eToSim );
    static const std::string& ConvertFromUnitTiredness( E_UnitTiredness, E_Conversion = eToSim );
    static const std::string& ConvertFromUnitMorale( E_UnitMorale, E_Conversion = eToSim );
    static const std::string& ConvertFromUnitExperience( E_UnitExperience, E_Conversion = eToSim );
    static const std::string& ConvertFromAutomatMode( E_AutomatMode, E_Conversion = eToSim );
    static const std::string& ConvertFromSetAutomatModeErrorCode( E_SetAutomatModeErrorCode, E_Conversion = eToSim );
    static const std::string& ConvertFromControlErrorCode( E_ControlErrorCode, E_Conversion = eToSim );
    static const std::string& ConvertFromSimulationState( E_SimulationState, E_Conversion = eToSim );
    static const std::string& ConvertFromPrecipitationType( E_PrecipitationType, E_Conversion = eToSim );
    static const std::string& ConvertFromFireEffectType( E_FireEffectType, E_Conversion = eToSim );
    static const std::string& ConvertFromInfoContextErrorCode( E_InfoContextErrorCode, E_Conversion = eToSim );
    static const std::string& ConvertFromLimaType( E_LimaType, E_Conversion = eToSim );
    static const std::string& ConvertFromLogMaintenanceRegimeTravail( E_LogMaintenanceRegimeTravail, E_Conversion = eToSim );
    static const std::string& ConvertFromLogMaintenanceHandlingStatus( E_LogMaintenanceHandlingStatus, E_Conversion = eToSim );
    static const std::string& ConvertFromLogSupplyHandlingStatus( E_LogSupplyHandlingStatus, E_Conversion = eToSim );
    static const std::string& ConvertFromMsgLogSupplyChangeQuotasAck( E_MsgLogSupplyChangeQuotasAck, E_Conversion = eToSim );
    static const std::string& ConvertFromMsgLogSupplyPushFlowAck( E_MsgLogSupplyPushFlowAck, E_Conversion = eToSim );
    static const std::string& ConvertFromLogMedicalHandlingStatus( E_LogMedicalHandlingStatus, E_Conversion = eToSim );
    static const std::string& ConvertFromDemolitionTargetType( E_DemolitionTargetType, E_Conversion = eToSim );    
    static const std::string& ConvertFromObjectErrorCode( E_ObjectErrorCode, E_Conversion = eToSim );

    static const std::string& ConvertFromOrderErrorCode( E_OrderErrorCode, E_Conversion = eToSim );
    static const std::string& ConvertFromReportType( E_ReportType, E_Conversion = eToSim );
    static const std::string& ConvertFromAutomatOrderFormation( E_AutomatOrderFormation, E_Conversion = eToSim );
    static const std::string& ConvertFromPopulationErrorCode( E_PopulationErrorCode, E_Conversion = eToSim );
    static const std::string& ConvertFromPopulationAttitude( E_PopulationAttitude, E_Conversion = eToSim );


    // ConvertTo functions
    static E_ChangeHierarchyErrorCode ConvertToChangeHierarchyErrorCode( const std::string& );
    static E_LocationType ConvertToLocationType( const std::string& );
    static E_DotationFamily ConvertToDotationFamily( const std::string& );
    static E_AmmunitionFamily ConvertToAmmunitionFamily( const std::string& );
    static E_UnitIdentificationLevel ConvertToUnitIdentificationLevel( const std::string& );
    static E_NatureLevel ConvertToNatureLevel( const std::string& );
    static E_HumanWound ConvertToHumanWound( const std::string& );
    static E_HumanRank ConvertToHumanRank( const std::string& );
    static E_Diplomacy ConvertToDiplomacy( const std::string& );
    static E_ChangeDiplomacyErrorCode ConvertToChangeDiplomacyErrorCode( const std::string& );
    static E_ForceRatioStatus ConvertToForceRatioStatus( const std::string& );
    static E_MeetingEngagementStatus ConvertToMeetingEngagementStatus( const std::string& );
    static E_OperationalStatus ConvertToOperationalStatus( const std::string& );
    static E_Roe ConvertToRoe( const std::string& );
    static E_UnitErrorCode ConvertToUnitErrorCode( const std::string& );
    static E_UnitPosture ConvertToUnitPosture( const std::string& );
    static E_FireAvailability ConvertToFireAvailability( const std::string& );
    static E_PopulationRoe ConvertToPopulationRoe( const std::string& );
    static E_UnitTiredness ConvertToUnitTiredness( const std::string& );
    static E_UnitMorale ConvertToUnitMorale( const std::string& );
    static E_UnitExperience ConvertToUnitExperience( const std::string& );
    static E_AutomatMode ConvertToAutomatMode( const std::string& );
    static E_SetAutomatModeErrorCode ConvertToSetAutomatModeErrorCode( const std::string& );
    static E_ControlErrorCode ConvertToControlErrorCode( const std::string& );
    static E_SimulationState ConvertToSimulationState( const std::string& );
    static E_PrecipitationType ConvertToPrecipitationType( const std::string& );
    static E_FireEffectType ConvertToFireEffectType( const std::string& );
    static E_InfoContextErrorCode ConvertToInfoContextErrorCode( const std::string& );
    static E_LimaType ConvertToLimaType( const std::string& );
    static E_LogMaintenanceRegimeTravail ConvertToLogMaintenanceRegimeTravail( const std::string& );
    static E_LogMaintenanceHandlingStatus ConvertToLogMaintenanceHandlingStatus( const std::string& );
    static E_LogSupplyHandlingStatus ConvertToLogSupplyHandlingStatus( const std::string& );
    static E_MsgLogSupplyChangeQuotasAck ConvertToMsgLogSupplyChangeQuotasAck( const std::string& );
    static E_MsgLogSupplyPushFlowAck ConvertToMsgLogSupplyPushFlowAck( const std::string& );
    static E_LogMedicalHandlingStatus ConvertToLogMedicalHandlingStatus( const std::string& );
    static E_ObjectErrorCode ConvertToObjectErrorCode( const std::string& );
    static E_DemolitionTargetType ConvertToDemolitionTargetType( const std::string& );
    static E_OrderErrorCode ConvertToOrderErrorCode( const std::string& );
    static E_ReportType ConvertToReportType( const std::string& );
    static E_AutomatOrderFormation ConvertToAutomatOrderFormation( const std::string& );
    static E_PopulationErrorCode ConvertToPopulationErrorCode( const std::string& );
    static E_PopulationAttitude ConvertToPopulationAttitude( const std::string& );


private:
    // Typedefs
    typedef ENT_Tr::converter<E_ChangeHierarchyErrorCode> T_ConverterChangeHierarchyErrorCode;
    typedef ENT_Tr::converter<E_LocationType> T_ConverterLocationType;
    typedef ENT_Tr::converter<E_DotationFamily> T_ConverterDotationFamily;
    typedef ENT_Tr::converter<E_AmmunitionFamily> T_ConverterAmmunitionFamily;
    typedef ENT_Tr::converter<E_UnitIdentificationLevel> T_ConverterUnitIdentificationLevel;
    typedef ENT_Tr::converter<E_NatureLevel> T_ConverterNatureLevel;
    typedef ENT_Tr::converter<E_HumanWound> T_ConverterHumanWound;
    typedef ENT_Tr::converter<E_HumanRank> T_ConverterHumanRank;
    typedef ENT_Tr::converter<E_Diplomacy> T_ConverterDiplomacy;
    typedef ENT_Tr::converter<E_ChangeDiplomacyErrorCode> T_ConverterChangeDiplomacyErrorCode;
    typedef ENT_Tr::converter<E_ForceRatioStatus> T_ConverterForceRatioStatus;
    typedef ENT_Tr::converter<E_MeetingEngagementStatus> T_ConverterMeetingEngagementStatus;
    typedef ENT_Tr::converter<E_OperationalStatus> T_ConverterOperationalStatus;
    typedef ENT_Tr::converter<E_Roe> T_ConverterRoe;
    typedef ENT_Tr::converter<E_UnitErrorCode> T_ConverterUnitErrorCode;
    typedef ENT_Tr::converter<E_UnitPosture> T_ConverterUnitPosture;
    typedef ENT_Tr::converter<E_FireAvailability> T_ConverterFireAvailability;
    typedef ENT_Tr::converter<E_PopulationRoe> T_ConverterPopulationRoe;
    typedef ENT_Tr::converter<E_UnitTiredness> T_ConverterUnitTiredness;
    typedef ENT_Tr::converter<E_UnitMorale> T_ConverterUnitMorale;
    typedef ENT_Tr::converter<E_UnitExperience> T_ConverterUnitExperience;
    typedef ENT_Tr::converter<E_AutomatMode> T_ConverterAutomatMode;
    typedef ENT_Tr::converter<E_SetAutomatModeErrorCode> T_ConverterSetAutomatModeErrorCode;
    typedef ENT_Tr::converter<E_ControlErrorCode> T_ConverterControlErrorCode;
    typedef ENT_Tr::converter<E_SimulationState> T_ConverterSimulationState;
    typedef ENT_Tr::converter<E_PrecipitationType> T_ConverterPrecipitationType;
    typedef ENT_Tr::converter<E_FireEffectType> T_ConverterFireEffectType;
    typedef ENT_Tr::converter<E_InfoContextErrorCode> T_ConverterInfoContextErrorCode;
    typedef ENT_Tr::converter<E_LimaType> T_ConverterLimaType;
    typedef ENT_Tr::converter<E_LogMaintenanceRegimeTravail> T_ConverterLogMaintenanceRegimeTravail;
    typedef ENT_Tr::converter<E_LogMaintenanceHandlingStatus> T_ConverterLogMaintenanceHandlingStatus;
    typedef ENT_Tr::converter<E_LogSupplyHandlingStatus> T_ConverterLogSupplyHandlingStatus;
    typedef ENT_Tr::converter<E_MsgLogSupplyChangeQuotasAck> T_ConverterMsgLogSupplyChangeQuotasAck;
    typedef ENT_Tr::converter<E_MsgLogSupplyPushFlowAck> T_ConverterMsgLogSupplyPushFlowAck;
    typedef ENT_Tr::converter<E_LogMedicalHandlingStatus> T_ConverterLogMedicalHandlingStatus;
    typedef ENT_Tr::converter<E_DemolitionTargetType> T_ConverterDemolitionTargetType;
    typedef ENT_Tr::converter<E_ObjectErrorCode> T_ConverterObjectErrorCode;
    typedef ENT_Tr::converter<E_OrderErrorCode> T_ConverterOrderErrorCode;
    typedef ENT_Tr::converter<E_ReportType> T_ConverterReportType;
    typedef ENT_Tr::converter<E_AutomatOrderFormation> T_ConverterAutomatOrderFormation;
    typedef ENT_Tr::converter<E_PopulationErrorCode> T_ConverterPopulationErrorCode;
    typedef ENT_Tr::converter<E_PopulationAttitude> T_ConverterPopulationAttitude;


    // Converters
    static T_ConverterChangeHierarchyErrorCode ChangeHierarchyErrorCodeConverter_ [];
    static T_ConverterLocationType LocationTypeConverter_ [];
    static T_ConverterDotationFamily DotationFamilyConverter_ [];
    static T_ConverterAmmunitionFamily AmmunitionFamilyConverter_ [];
    static T_ConverterUnitIdentificationLevel UnitIdentificationLevelConverter_ [];
    static T_ConverterNatureLevel NatureLevelConverter_ [];
    static T_ConverterHumanWound HumanWoundConverter_ [];
    static T_ConverterHumanRank HumanRankConverter_ [];
    static T_ConverterDiplomacy DiplomacyConverter_ [];
    static T_ConverterChangeDiplomacyErrorCode ChangeDiplomacyErrorCodeConverter_ [];
    static T_ConverterForceRatioStatus ForceRatioStatusConverter_ [];
    static T_ConverterMeetingEngagementStatus MeetingEngagementStatusConverter_ [];
    static T_ConverterOperationalStatus OperationalStatusConverter_ [];
    static T_ConverterRoe RoeConverter_ [];
    static T_ConverterUnitErrorCode UnitErrorCodeConverter_ [];
    static T_ConverterUnitPosture UnitPostureConverter_ [];
    static T_ConverterFireAvailability FireAvailabilityConverter_ [];
    static T_ConverterPopulationRoe PopulationRoeConverter_ [];
    static T_ConverterUnitTiredness UnitTirednessConverter_ [];
    static T_ConverterUnitMorale UnitMoraleConverter_ [];
    static T_ConverterUnitExperience UnitExperienceConverter_ [];
    static T_ConverterAutomatMode AutomatModeConverter_ [];
    static T_ConverterSetAutomatModeErrorCode SetAutomatModeErrorCodeConverter_ [];
    static T_ConverterControlErrorCode ControlErrorCodeConverter_ [];
    static T_ConverterSimulationState SimulationStateConverter_ [];
    static T_ConverterPrecipitationType PrecipitationTypeConverter_ [];
    static T_ConverterFireEffectType FireEffectTypeConverter_ [];
    static T_ConverterInfoContextErrorCode InfoContextErrorCodeConverter_ [];
    static T_ConverterLimaType LimaTypeConverter_ [];
    static T_ConverterLogMaintenanceRegimeTravail LogMaintenanceRegimeTravailConverter_ [];
    static T_ConverterLogMaintenanceHandlingStatus LogMaintenanceHandlingStatusConverter_ [];
    static T_ConverterLogSupplyHandlingStatus LogSupplyHandlingStatusConverter_ [];
    static T_ConverterMsgLogSupplyChangeQuotasAck MsgLogSupplyChangeQuotasAckConverter_ [];
    static T_ConverterMsgLogSupplyPushFlowAck MsgLogSupplyPushFlowAckConverter_ [];
    static T_ConverterLogMedicalHandlingStatus LogMedicalHandlingStatusConverter_ [];
    static T_ConverterDemolitionTargetType DemolitionTargetTypeConverter_ [];
    static T_ConverterObjectErrorCode ObjectErrorCodeConverter_ [];
    static T_ConverterOrderErrorCode OrderErrorCodeConverter_ [];
    static T_ConverterReportType ReportTypeConverter_ [];
    static T_ConverterAutomatOrderFormation AutomatOrderFormationConverter_ [];
    static T_ConverterPopulationErrorCode PopulationErrorCodeConverter_ [];
    static T_ConverterPopulationAttitude PopulationAttitudeConverter_ [];

};


#endif // __ENT_Tr_Gen_h_