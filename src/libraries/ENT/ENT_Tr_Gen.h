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
    static const std::string& ConvertFromTypeLocalisation( E_TypeLocalisation, E_Conversion = eToSim );
    static const std::string& ConvertFromGDH_Qualificatif( E_GDH_Qualificatif, E_Conversion = eToSim );
    static const std::string& ConvertFromHumanWound( E_HumanWound, E_Conversion = eToSim );
    static const std::string& ConvertFromUnitIdentificationLevel( E_UnitIdentificationLevel, E_Conversion = eToSim );
    static const std::string& ConvertFromFamilleDotation( E_FamilleDotation, E_Conversion = eToSim );
    static const std::string& ConvertFromFamilleMunition( E_FamilleMunition, E_Conversion = eToSim );
    static const std::string& ConvertFromNatureLevel( E_NatureLevel, E_Conversion = eToSim );
    static const std::string& ConvertFromHumanRank( E_HumanRank, E_Conversion = eToSim );
    static const std::string& ConvertFromDiplomatie( E_Diplomatie, E_Conversion = eToSim );
    static const std::string& ConvertFromChangeDiplomatieErrorCode( E_ChangeDiplomatieErrorCode, E_Conversion = eToSim );
    static const std::string& ConvertFromEtatRapFor( E_EtatRapFor, E_Conversion = eToSim );
    static const std::string& ConvertFromEtatCombatRencontre( E_EtatCombatRencontre, E_Conversion = eToSim );
    static const std::string& ConvertFromEtatOperationnel( E_EtatOperationnel, E_Conversion = eToSim );
    static const std::string& ConvertFromRoe( E_Roe, E_Conversion = eToSim );
    static const std::string& ConvertFromUnitAttrErrorCode( E_UnitAttrErrorCode, E_Conversion = eToSim );
    static const std::string& ConvertFromUnitPosture( E_UnitPosture, E_Conversion = eToSim );
    static const std::string& ConvertFromDisponibiliteAuTir( E_DisponibiliteAuTir, E_Conversion = eToSim );
    static const std::string& ConvertFromRoePopulation( E_RoePopulation, E_Conversion = eToSim );
    static const std::string& ConvertFromUnitFatigue( E_UnitFatigue, E_Conversion = eToSim );
    static const std::string& ConvertFromUnitMoral( E_UnitMoral, E_Conversion = eToSim );
    static const std::string& ConvertFromUnitExperience( E_UnitExperience, E_Conversion = eToSim );
    static const std::string& ConvertFromAutomateState( E_AutomateState, E_Conversion = eToSim );
    static const std::string& ConvertFromSetAutomateModeErrorCode( E_SetAutomateModeErrorCode, E_Conversion = eToSim );
    static const std::string& ConvertFromChangeAutomateErrorCode( E_ChangeAutomateErrorCode, E_Conversion = eToSim );
    static const std::string& ConvertFromChangeLiensLogistiquesErrorCode( E_ChangeLiensLogistiquesErrorCode, E_Conversion = eToSim );
    static const std::string& ConvertFrom_ALAT_PorteeAction( E__ALAT_PorteeAction, E_Conversion = eToSim );
    static const std::string& ConvertFrom_ALAT_AmbianceMission( E__ALAT_AmbianceMission, E_Conversion = eToSim );
    static const std::string& ConvertFromModeDeploiement( E_ModeDeploiement, E_Conversion = eToSim );
    static const std::string& ConvertFromTypeLima( E_TypeLima, E_Conversion = eToSim );
    static const std::string& ConvertFromInfoContextErrorCode( E_InfoContextErrorCode, E_Conversion = eToSim );
    static const std::string& ConvertFromOrderErrorCode( E_OrderErrorCode, E_Conversion = eToSim );
    static const std::string& ConvertFromOrderState( E_OrderState, E_Conversion = eToSim );
    static const std::string& ConvertFromObjectType( E_ObjectType, E_Conversion = eToSim );
    static const std::string& ConvertFromMissionGenSousTypeObstacle( E_MissionGenSousTypeObstacle, E_Conversion = eToSim );
    static const std::string& ConvertFromObjectErrorCode( E_ObjectErrorCode, E_Conversion = eToSim );
    static const std::string& ConvertFromModeDeploiementSectionMistral( E_ModeDeploiementSectionMistral, E_Conversion = eToSim );
    static const std::string& ConvertFromGenTypePontage( E_GenTypePontage, E_Conversion = eToSim );
    static const std::string& ConvertFromMissionInfNatureObjectif( E_MissionInfNatureObjectif, E_Conversion = eToSim );
    static const std::string& ConvertFromLogSanteTraitementEtat( E_LogSanteTraitementEtat, E_Conversion = eToSim );
    static const std::string& ConvertFromAutomateOrderFormation( E_AutomateOrderFormation, E_Conversion = eToSim );
    static const std::string& ConvertFromCtrlErrorCode( E_CtrlErrorCode, E_Conversion = eToSim );
    static const std::string& ConvertFromEtatSim( E_EtatSim, E_Conversion = eToSim );
    static const std::string& ConvertFromPrecipitationType( E_PrecipitationType, E_Conversion = eToSim );
    static const std::string& ConvertFromPopulationAttitude( E_PopulationAttitude, E_Conversion = eToSim );
    static const std::string& ConvertFromPopulationAttrErrorCode( E_PopulationAttrErrorCode, E_Conversion = eToSim );
    static const std::string& ConvertFromTypeCR( E_TypeCR, E_Conversion = eToSim );
    static const std::string& ConvertFromFireEffectType( E_FireEffectType, E_Conversion = eToSim );
    static const std::string& ConvertFromChangeGroupeConnaissanceErrorCode( E_ChangeGroupeConnaissanceErrorCode, E_Conversion = eToSim );
    static const std::string& ConvertFromLogMaintenanceRegimeTravail( E_LogMaintenanceRegimeTravail, E_Conversion = eToSim );
    static const std::string& ConvertFromLogMaintenanceTraitementEtat( E_LogMaintenanceTraitementEtat, E_Conversion = eToSim );
    static const std::string& ConvertFromLogRavitaillementTraitementEtat( E_LogRavitaillementTraitementEtat, E_Conversion = eToSim );
    static const std::string& ConvertFromMsgLogRavitaillementChangeQuotasAck( E_MsgLogRavitaillementChangeQuotasAck, E_Conversion = eToSim );
    static const std::string& ConvertFromMsgLogRavitaillementPousserFluxAck( E_MsgLogRavitaillementPousserFluxAck, E_Conversion = eToSim );
    static const std::string& ConvertFromAmbianceMission( E_AmbianceMission, E_Conversion = eToSim );
    static const std::string& ConvertFromTC2GererMaterielAvantDeplacement( E_TC2GererMaterielAvantDeplacement, E_Conversion = eToSim );

    static const std::string& ConvertFromAutomataMission  ( E_AutomataMission  , E_Conversion = eToTr );
    static const std::string& ConvertFromUnitMission      ( E_UnitMission      , E_Conversion = eToTr );
    static const std::string& ConvertFromPopulationMission( E_PopulationMission, E_Conversion = eToTr );
    static const std::string& ConvertFromFragOrder        ( E_FragOrder        , E_Conversion = eToTr );

    // ConvertTo functions
    static E_TypeLocalisation ConvertToTypeLocalisation( const std::string& );
    static E_GDH_Qualificatif ConvertToGDH_Qualificatif( const std::string& );
    static E_HumanWound ConvertToHumanWound( const std::string& );
    static E_UnitIdentificationLevel ConvertToUnitIdentificationLevel( const std::string& );
    static E_FamilleDotation ConvertToFamilleDotation( const std::string& );
    static E_FamilleMunition ConvertToFamilleMunition( const std::string& );
    static E_NatureLevel ConvertToNatureLevel( const std::string& );
    static E_HumanRank ConvertToHumanRank( const std::string& );
    static E_Diplomatie ConvertToDiplomatie( const std::string& );
    static E_ChangeDiplomatieErrorCode ConvertToChangeDiplomatieErrorCode( const std::string& );
    static E_EtatRapFor ConvertToEtatRapFor( const std::string& );
    static E_EtatCombatRencontre ConvertToEtatCombatRencontre( const std::string& );
    static E_EtatOperationnel ConvertToEtatOperationnel( const std::string& );
    static E_Roe ConvertToRoe( const std::string& );
    static E_UnitAttrErrorCode ConvertToUnitAttrErrorCode( const std::string& );
    static E_UnitPosture ConvertToUnitPosture( const std::string& );
    static E_DisponibiliteAuTir ConvertToDisponibiliteAuTir( const std::string& );
    static E_RoePopulation ConvertToRoePopulation( const std::string& );
    static E_UnitFatigue ConvertToUnitFatigue( const std::string& );
    static E_UnitMoral ConvertToUnitMoral( const std::string& );
    static E_UnitExperience ConvertToUnitExperience( const std::string& );
    static E_AutomateState ConvertToAutomateState( const std::string& );
    static E_SetAutomateModeErrorCode ConvertToSetAutomateModeErrorCode( const std::string& );
    static E_ChangeAutomateErrorCode ConvertToChangeAutomateErrorCode( const std::string& );
    static E_ChangeLiensLogistiquesErrorCode ConvertToChangeLiensLogistiquesErrorCode( const std::string& );
    static E__ALAT_PorteeAction ConvertTo_ALAT_PorteeAction( const std::string& );
    static E__ALAT_AmbianceMission ConvertTo_ALAT_AmbianceMission( const std::string& );
    static E_ModeDeploiement ConvertToModeDeploiement( const std::string& );
    static E_TypeLima ConvertToTypeLima( const std::string& );
    static E_InfoContextErrorCode ConvertToInfoContextErrorCode( const std::string& );
    static E_OrderErrorCode ConvertToOrderErrorCode( const std::string& );
    static E_OrderState ConvertToOrderState( const std::string& );
    static E_ObjectType ConvertToObjectType( const std::string& );
    static E_MissionGenSousTypeObstacle ConvertToMissionGenSousTypeObstacle( const std::string& );
    static E_ObjectErrorCode ConvertToObjectErrorCode( const std::string& );
    static E_ModeDeploiementSectionMistral ConvertToModeDeploiementSectionMistral( const std::string& );
    static E_GenTypePontage ConvertToGenTypePontage( const std::string& );
    static E_MissionInfNatureObjectif ConvertToMissionInfNatureObjectif( const std::string& );
    static E_LogSanteTraitementEtat ConvertToLogSanteTraitementEtat( const std::string& );
    static E_AutomateOrderFormation ConvertToAutomateOrderFormation( const std::string& );
    static E_CtrlErrorCode ConvertToCtrlErrorCode( const std::string& );
    static E_EtatSim ConvertToEtatSim( const std::string& );
    static E_PrecipitationType ConvertToPrecipitationType( const std::string& );
    static E_PopulationAttitude ConvertToPopulationAttitude( const std::string& );
    static E_PopulationAttrErrorCode ConvertToPopulationAttrErrorCode( const std::string& );
    static E_TypeCR ConvertToTypeCR( const std::string& );
    static E_FireEffectType ConvertToFireEffectType( const std::string& );
    static E_ChangeGroupeConnaissanceErrorCode ConvertToChangeGroupeConnaissanceErrorCode( const std::string& );
    static E_LogMaintenanceRegimeTravail ConvertToLogMaintenanceRegimeTravail( const std::string& );
    static E_LogMaintenanceTraitementEtat ConvertToLogMaintenanceTraitementEtat( const std::string& );
    static E_LogRavitaillementTraitementEtat ConvertToLogRavitaillementTraitementEtat( const std::string& );
    static E_MsgLogRavitaillementChangeQuotasAck ConvertToMsgLogRavitaillementChangeQuotasAck( const std::string& );
    static E_MsgLogRavitaillementPousserFluxAck ConvertToMsgLogRavitaillementPousserFluxAck( const std::string& );
    static E_AmbianceMission ConvertToAmbianceMission( const std::string& );
    static E_TC2GererMaterielAvantDeplacement ConvertToTC2GererMaterielAvantDeplacement( const std::string& );

    static E_AutomataMission   ConvertToAutomataMission  ( const std::string& );
    static E_UnitMission       ConvertToUnitMission      ( const std::string& );
    static E_PopulationMission ConvertToPopulationMission( const std::string& );
    static E_FragOrder         ConvertToFragOrder        ( const std::string& );

private:
    // Typedefs
    typedef ENT_Tr::converter<E_TypeLocalisation> T_ConverterTypeLocalisation;
    typedef ENT_Tr::converter<E_GDH_Qualificatif> T_ConverterGDH_Qualificatif;
    typedef ENT_Tr::converter<E_HumanWound> T_ConverterHumanWound;
    typedef ENT_Tr::converter<E_UnitIdentificationLevel> T_ConverterUnitIdentificationLevel;
    typedef ENT_Tr::converter<E_FamilleDotation> T_ConverterFamilleDotation;
    typedef ENT_Tr::converter<E_FamilleMunition> T_ConverterFamilleMunition;
    typedef ENT_Tr::converter<E_NatureLevel> T_ConverterNatureLevel;
    typedef ENT_Tr::converter<E_HumanRank> T_ConverterHumanRank;
    typedef ENT_Tr::converter<E_Diplomatie> T_ConverterDiplomatie;
    typedef ENT_Tr::converter<E_ChangeDiplomatieErrorCode> T_ConverterChangeDiplomatieErrorCode;
    typedef ENT_Tr::converter<E_EtatRapFor> T_ConverterEtatRapFor;
    typedef ENT_Tr::converter<E_EtatCombatRencontre> T_ConverterEtatCombatRencontre;
    typedef ENT_Tr::converter<E_EtatOperationnel> T_ConverterEtatOperationnel;
    typedef ENT_Tr::converter<E_Roe> T_ConverterRoe;
    typedef ENT_Tr::converter<E_UnitAttrErrorCode> T_ConverterUnitAttrErrorCode;
    typedef ENT_Tr::converter<E_UnitPosture> T_ConverterUnitPosture;
    typedef ENT_Tr::converter<E_DisponibiliteAuTir> T_ConverterDisponibiliteAuTir;
    typedef ENT_Tr::converter<E_RoePopulation> T_ConverterRoePopulation;
    typedef ENT_Tr::converter<E_UnitFatigue> T_ConverterUnitFatigue;
    typedef ENT_Tr::converter<E_UnitMoral> T_ConverterUnitMoral;
    typedef ENT_Tr::converter<E_UnitExperience> T_ConverterUnitExperience;
    typedef ENT_Tr::converter<E_AutomateState> T_ConverterAutomateState;
    typedef ENT_Tr::converter<E_SetAutomateModeErrorCode> T_ConverterSetAutomateModeErrorCode;
    typedef ENT_Tr::converter<E_ChangeAutomateErrorCode> T_ConverterChangeAutomateErrorCode;
    typedef ENT_Tr::converter<E_ChangeLiensLogistiquesErrorCode> T_ConverterChangeLiensLogistiquesErrorCode;
    typedef ENT_Tr::converter<E__ALAT_PorteeAction> T_Converter_ALAT_PorteeAction;
    typedef ENT_Tr::converter<E__ALAT_AmbianceMission> T_Converter_ALAT_AmbianceMission;
    typedef ENT_Tr::converter<E_ModeDeploiement> T_ConverterModeDeploiement;
    typedef ENT_Tr::converter<E_TypeLima> T_ConverterTypeLima;
    typedef ENT_Tr::converter<E_InfoContextErrorCode> T_ConverterInfoContextErrorCode;
    typedef ENT_Tr::converter<E_OrderErrorCode> T_ConverterOrderErrorCode;
    typedef ENT_Tr::converter<E_OrderState> T_ConverterOrderState;
    typedef ENT_Tr::converter<E_ObjectType> T_ConverterObjectType;
    typedef ENT_Tr::converter<E_MissionGenSousTypeObstacle> T_ConverterMissionGenSousTypeObstacle;
    typedef ENT_Tr::converter<E_ObjectErrorCode> T_ConverterObjectErrorCode;
    typedef ENT_Tr::converter<E_ModeDeploiementSectionMistral> T_ConverterModeDeploiementSectionMistral;
    typedef ENT_Tr::converter<E_GenTypePontage> T_ConverterGenTypePontage;
    typedef ENT_Tr::converter<E_MissionInfNatureObjectif> T_ConverterMissionInfNatureObjectif;
    typedef ENT_Tr::converter<E_LogSanteTraitementEtat> T_ConverterLogSanteTraitementEtat;
    typedef ENT_Tr::converter<E_AutomateOrderFormation> T_ConverterAutomateOrderFormation;
    typedef ENT_Tr::converter<E_CtrlErrorCode> T_ConverterCtrlErrorCode;
    typedef ENT_Tr::converter<E_EtatSim> T_ConverterEtatSim;
    typedef ENT_Tr::converter<E_PrecipitationType> T_ConverterPrecipitationType;
    typedef ENT_Tr::converter<E_PopulationAttitude> T_ConverterPopulationAttitude;
    typedef ENT_Tr::converter<E_PopulationAttrErrorCode> T_ConverterPopulationAttrErrorCode;
    typedef ENT_Tr::converter<E_TypeCR> T_ConverterTypeCR;
    typedef ENT_Tr::converter<E_FireEffectType> T_ConverterFireEffectType;
    typedef ENT_Tr::converter<E_ChangeGroupeConnaissanceErrorCode> T_ConverterChangeGroupeConnaissanceErrorCode;
    typedef ENT_Tr::converter<E_LogMaintenanceRegimeTravail> T_ConverterLogMaintenanceRegimeTravail;
    typedef ENT_Tr::converter<E_LogMaintenanceTraitementEtat> T_ConverterLogMaintenanceTraitementEtat;
    typedef ENT_Tr::converter<E_LogRavitaillementTraitementEtat> T_ConverterLogRavitaillementTraitementEtat;
    typedef ENT_Tr::converter<E_MsgLogRavitaillementChangeQuotasAck> T_ConverterMsgLogRavitaillementChangeQuotasAck;
    typedef ENT_Tr::converter<E_MsgLogRavitaillementPousserFluxAck> T_ConverterMsgLogRavitaillementPousserFluxAck;
    typedef ENT_Tr::converter<E_AmbianceMission> T_ConverterAmbianceMission;
    typedef ENT_Tr::converter<E_TC2GererMaterielAvantDeplacement> T_ConverterTC2GererMaterielAvantDeplacement;

    typedef ENT_Tr::converter<E_AutomataMission>   T_ConverterAutomataMission;
    typedef ENT_Tr::converter<E_UnitMission>       T_ConverterUnitMission;
    typedef ENT_Tr::converter<E_PopulationMission> T_ConverterPopulationMission;
    typedef ENT_Tr::converter<E_FragOrder>         T_ConverterFragOrder;

    // Converters
    static T_ConverterTypeLocalisation TypeLocalisationConverter_ [];
    static T_ConverterGDH_Qualificatif GDH_QualificatifConverter_ [];
    static T_ConverterHumanWound HumanWoundConverter_ [];
    static T_ConverterUnitIdentificationLevel UnitIdentificationLevelConverter_ [];
    static T_ConverterFamilleDotation FamilleDotationConverter_ [];
    static T_ConverterFamilleMunition FamilleMunitionConverter_ [];
    static T_ConverterNatureLevel NatureLevelConverter_ [];
    static T_ConverterHumanRank HumanRankConverter_ [];
    static T_ConverterDiplomatie DiplomatieConverter_ [];
    static T_ConverterChangeDiplomatieErrorCode ChangeDiplomatieErrorCodeConverter_ [];
    static T_ConverterEtatRapFor EtatRapForConverter_ [];
    static T_ConverterEtatCombatRencontre EtatCombatRencontreConverter_ [];
    static T_ConverterEtatOperationnel EtatOperationnelConverter_ [];
    static T_ConverterRoe RoeConverter_ [];
    static T_ConverterUnitAttrErrorCode UnitAttrErrorCodeConverter_ [];
    static T_ConverterUnitPosture UnitPostureConverter_ [];
    static T_ConverterDisponibiliteAuTir DisponibiliteAuTirConverter_ [];
    static T_ConverterRoePopulation RoePopulationConverter_ [];
    static T_ConverterUnitFatigue UnitFatigueConverter_ [];
    static T_ConverterUnitMoral UnitMoralConverter_ [];
    static T_ConverterUnitExperience UnitExperienceConverter_ [];
    static T_ConverterAutomateState AutomateStateConverter_ [];
    static T_ConverterSetAutomateModeErrorCode SetAutomateModeErrorCodeConverter_ [];
    static T_ConverterChangeAutomateErrorCode ChangeAutomateErrorCodeConverter_ [];
    static T_ConverterChangeLiensLogistiquesErrorCode ChangeLiensLogistiquesErrorCodeConverter_ [];
    static T_Converter_ALAT_PorteeAction _ALAT_PorteeActionConverter_ [];
    static T_Converter_ALAT_AmbianceMission _ALAT_AmbianceMissionConverter_ [];
    static T_ConverterModeDeploiement ModeDeploiementConverter_ [];
    static T_ConverterTypeLima TypeLimaConverter_ [];
    static T_ConverterInfoContextErrorCode InfoContextErrorCodeConverter_ [];
    static T_ConverterOrderErrorCode OrderErrorCodeConverter_ [];
    static T_ConverterOrderState OrderStateConverter_ [];
    static T_ConverterObjectType ObjectTypeConverter_ [];
    static T_ConverterMissionGenSousTypeObstacle MissionGenSousTypeObstacleConverter_ [];
    static T_ConverterObjectErrorCode ObjectErrorCodeConverter_ [];
    static T_ConverterModeDeploiementSectionMistral ModeDeploiementSectionMistralConverter_ [];
    static T_ConverterGenTypePontage GenTypePontageConverter_ [];
    static T_ConverterMissionInfNatureObjectif MissionInfNatureObjectifConverter_ [];
    static T_ConverterLogSanteTraitementEtat LogSanteTraitementEtatConverter_ [];
    static T_ConverterAutomateOrderFormation AutomateOrderFormationConverter_ [];
    static T_ConverterCtrlErrorCode CtrlErrorCodeConverter_ [];
    static T_ConverterEtatSim EtatSimConverter_ [];
    static T_ConverterPrecipitationType PrecipitationTypeConverter_ [];
    static T_ConverterPopulationAttitude PopulationAttitudeConverter_ [];
    static T_ConverterPopulationAttrErrorCode PopulationAttrErrorCodeConverter_ [];
    static T_ConverterTypeCR TypeCRConverter_ [];
    static T_ConverterFireEffectType FireEffectTypeConverter_ [];
    static T_ConverterChangeGroupeConnaissanceErrorCode ChangeGroupeConnaissanceErrorCodeConverter_ [];
    static T_ConverterLogMaintenanceRegimeTravail LogMaintenanceRegimeTravailConverter_ [];
    static T_ConverterLogMaintenanceTraitementEtat LogMaintenanceTraitementEtatConverter_ [];
    static T_ConverterLogRavitaillementTraitementEtat LogRavitaillementTraitementEtatConverter_ [];
    static T_ConverterMsgLogRavitaillementChangeQuotasAck MsgLogRavitaillementChangeQuotasAckConverter_ [];
    static T_ConverterMsgLogRavitaillementPousserFluxAck MsgLogRavitaillementPousserFluxAckConverter_ [];
    static T_ConverterAmbianceMission AmbianceMissionConverter_ [];
    static T_ConverterTC2GererMaterielAvantDeplacement TC2GererMaterielAvantDeplacementConverter_ [];

    static T_ConverterAutomataMission   automataMissionConverter_[];
    static T_ConverterUnitMission       unitMissionConverter_[];
    static T_ConverterPopulationMission populationMissionConverter_[];
    static T_ConverterFragOrder         fragOrderConverter_[];
};


#endif // __ENT_Tr_Gen_h_