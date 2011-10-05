// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_Report_h_
#define __MIL_Report_h_

#include "MIL_MissionParameterFactory.h"
#include "Tools/MIL_IDManager.h"

namespace xml
{
    class xistream;
}

namespace client
{
    class Report;
}

class MIL_ParameterType_ABC;
class PHY_ComposanteTypePion;
class PHY_DotationCategory;
class MIL_Effect_IndirectFire;
class MIL_MissionParameter_ABC;

// =============================================================================
/** @class  MIL_Report
    @brief  MIL_Report
*/
// Created: NLD 2006-11-14
// =============================================================================
class MIL_Report
{
public:
    //! @name Types
    //@{
    enum E_Type
    {
        eRcTypeMessage     = 0,
        eRcTypeOperational = 1,
        eRcTypeEvent       = 2,
        eRcTypeWarning     = 3
    };

    enum E_EngineReport
    {
        eReport_ReAvailableAfterRepairation,
        eReport_EquipementRepairedInPlace,
        eReport_EquipementBackFromMaintenance,
        eReport_IndirectFireObservation,
        eReport_StockSupplyRequest,
        eReport_DotationSupplyRequest,
        eReport_MedicalEvacuationRequest,
        eReport_EquipementEvacuationRequest,
        eReport_DotationSupplyDone,
        eReport_StockSupplyDone,
        eReport_DotationSupplyCanceled,
        eReport_StockSupplyCanceled,
        eReport_LogisticStockThresholdExceeded,
        eReport_LogisticDotationThresholdExceeded,
        eReport_HumanBackFromMedical,
        eReport_WoundedManDeath,
        eReport_WoundedManDeathDuringTransport,
        eReport_WoundedManDeathDuringHospitalization,
        eReport_FireInForbiddenArea,
        eReport_FratricideIndirectFire,
        eReport_IndirectFireOnPopulation, // 20
        eReport_FireOnFriendSide,
        eReport_FiredByFriendSide,
        eReport_FireOnNeutralSide,
        eReport_FiredByNeutralSide,
        eReport_FireOnCivilian,
        eReport_FiredByCivilian,
        eReport_CommandPostDestruction,
        eReport_QuotaAlmostConsumed,
        eReport_StockCapacityExceeded,
        eReport_MissionImpossible_,
        eReport_UnitDecontaminated,
        eReport_OutOfGas,
        eReport_DifficultTerrain,
        eReport_Surrendered,
        eReport_CancelSurrender,
        eReport_MaintenanceWorkRateExceeded,
        eReport_EquipmentLoanInProgress,
        eReport_EquipmentLoanCanceled,
        eReport_EquipmentLoanDone,
        eReport_EquipmentLoanPartiallyDone, // 40
        eReport_EquipmentLoanImpossible,
        eReport_EquipmentLoanRetrievingDone,
        eReport_EquipmentLoanRetrievingPartiallyDone,
        eReport_EquipmentLoanRetrievingImpossible,
        eReport_EquipmentLent,
        eReport_EquipmentLoanGivenBack,
        eReport_InterventionAgainstPopulationStarted,
        eReport_MineExplosionOnPopulation,
        eReport_ConfrontationWithPopulation,
        eReport_Blocked,
        eReport_Filtered,
        eReport_TerroristAttackAgainstPopulation,
        eReport_EvacuationResourcesLevelReached,
        eReport_CollectionResourcesLevelReached,
        eReport_DoctorResourcesLevelReached,
        eReport_RepairerResourcesLevelReached,
        eReport_HaulerResourcesLevelReached,
        eReport_ConvoyTransporterResourcesLevelReached,
        eReport_EmbarkmentStarted,
        eReport_EmbarkmentInterrupted, // 60
        eReport_EmbarkmentResumed,
        eReport_EmbarkmentFinished,
        eReport_DisembarkmentStarted,
        eReport_DisembarkmentInterrupted,
        eReport_DisembarkmentResumed,
        eReport_DisembarkmentFinished,
        eReport_ActiveProtectionCounter,
        eReport_ActiveProtectionHardKill,
        eReport_Poisoned,
        eReport_Contaminated,
        eReport_FireOnEnemySide,
        eReport_FiredByEnemySide,
        eReport_IndirectFireOnTarget,
        eReport_DestructionPC,
        eReport_MunitionInterdite,
        eReport_MunitionAutorise,
        eReport_DamagesCausedToNeutralSide,
        eReport_DamagesCausedByNeutralSide,
        eReport_InfrastructureDamaged,
        eReport_LivingAreaDamaged,
        eReport_BorderCrossed, // 80
        eReport_TacticallyDestroyed,
        eReport_CiviliansEncountered,
        eReport_PrisonersUnsupplied,
        eReport_PrisonersCampFull,
        eNbrReport
    };

    enum E_DecisionalReport
    {
        eRC_RAS,
        eRC_EnPosteFaceAObstacle, // Parametre : T_ConnaissanceObjet
        eRC_EnContournementObstacle,
        eRC_MissionImpossible,
        eRC_ProgressionSurAxe,
        eRC_ProgressionVersPosition,
        eRC_ArriveSurPosition,
        eRC_ReconnaissancePoint,
        eRC_EnPosteFaceADirectionDangereuse,
        eRC_EnJalonnement,
        eRC_PreparationMission, // Parametre : float
        eRC_DebutMission,
        eRC_FinMission,
        eRC_DispositifInstalle,
        eRC_EnPosition,
        eRC_ObjectifAtteint,
        eRC_Debouche,
        eRC_RejointUnite,
        eRC_EscorteUnite,
        eRC_EnFreinage,
        eRC_Assaut, // 20
        eRC_EnCouverture,
        eRC_EnEclairage,
        eRC_EnManoeuvreRavitaillement,
        eRC_SurPIA, // Parametre : T_Pion
        eRC_PassageSurPIA,
        eRC_UniteRecueillie, // Parametre : T_Pion
        eRC_EnCoursDeFranchissement,
        eRC_AttenteOrdrePoursuivre,
        eRC_AttenteOrdreDeboucher,
        eRC_Fixe,
        eRC_EnEsquive,
        eRC_PriseContact,
        eRC_RuptureContact,
        eRC_PrecisionContact, // Parametre : T_ConnaissanceAgent
        eRC_EnnemiDetruit, // Parametre : T_ConnaissanceAgent
        eRC_PrisAPartieParTirDirect,
        eRC_PrisSousTirArtillerie,
        eRC_PointTenuParEni, // Parametre : T_ConnaissanceAgent
        eRC_ProgressionVersEni, // Parametre : T_ConnaissanceAgent
        eRC_Decroche, // 40
        eRC_EniSurObjectif, // Parametre : T_ConnaissanceAgent
        eRC_Riposte,
        eRC_PretPourEngagementSurEnnemi,
        eRC_DebutControleZone,
        eRC_FinControleZone,
        eRC_DebutSurveillance,
        eRC_FinSurveillance,
        eRC_EnInfiltration,
        eRC_EnExfiltration,
        eRC_AttenteConditionFavorablePourInfiltration,
        eRC_DroneLance,
        eRC_DroneEnExploitation,
        eRC_DroneExploite,
        eRC_DroneDisponible,
        eRC_DebutMiseEnOeuvreDrone,
        eRC_FinMiseEnOeuvreDrone,
        eRC_ActivationBrouillage,
        eRC_DesactivationBrouillage,
        eRC_ActivationSystemeEcoute,
        eRC_DesactivationSystemeEcoute, // 60
        eRC_ActivationSystemeEcouteRadar,
        eRC_DesactivationSystemeEcouteRadar,
        eRC_DebutLiaison,
        eRC_FinLiaison,
        eRC_EtablissementLiaisonImpossible,
        eRC_UniteTropDistante, // Parametre : T_Pion
        eRC_EnregistrementDonnes,
        eRC_ExploitationDonnees,
        eRC_ExtractionDonnes,
        eRC_Releve, // Parametre : T_Pion
        eRC_SuitUnite,
        eRC_PasseEnAppui,
        eRC_EnAppui,
        eRC_NonOperationnel,
        eRC_Operationnel,
        eRC_AmbianceVitesse,
        eRC_AmbianceSurete,
        eRC_SurLima, // Parametre : id
        eRC_DebutTravaux,
        eRC_FinTravaux, // 80
        eRC_ObstacleDegage,
        eRC_OrganisationChantier,
        eRC_DesorganisationChantier,
        eRC_ReconnaissancePositionObstacle,
        eRC_ContournementRealise,
        eRC_AttenteRenforcement,
        eRC_DebutExploitationSiteFranchissement,
        eRC_FinExploitationSiteFranchissement,
        eRC_ReussiteRenforcement, // Parametres : pion_renforcant (T_Pion), automate_renforce (T_Pion),
        eRC_EchecRenforcement, // Parametres : pion_renforcant (T_Pion), automate_renforce (T_Pion),
        eRC_ObstacleEnAttenteActivation, // Parametre : T_ConnaissanceObjet
        eRC_DebutAmenagementBerges,
        eRC_FinAmenagementBerges,
        eRC_RegimeMaintenanceDelaiDepasse,
        eRC_SectionDeployee,
        eRC_ANouveauDisponibleApresReparation,
        eRC_MaterielRetourDeMaintenance, // Parametre : TypeEquipement
        eRC_MaterielRepareSurPlace, // Parametre : TypeEquipement
        eRC_DemandeRavitaillementDotations,
        eRC_DemandeRavitaillementStocks, // 100
        eRC_DemandeEvacuationSanitaire,
        eRC_DemandeEvacuationMateriel,
        eRC_AllocationConsentieBientotEpuisee, // Parametre : TypeDotation
        eRC_DepassementCapaciteStockage,
        eRC_SeuilLogistiqueDotationDepasse, // Parametre : TypeDotation
        eRC_SeuilLogistiqueStockDepasse, // Parametre : TypeDotation
        eRC_RavitaillementDotationsEffectue,
        eRC_RavitaillementStockEffectue,
        eRC_RavitaillementDotationsAnnule,
        eRC_RavitaillementStockAnnule,
        eRC_HumainRetourDeSante,
        eRC_AlerteDisponibiliteMoyensReleve,
        eRC_AlerteDisponibiliteMoyensRamassage,
        eRC_AlerteDisponibiliteMedecins,
        eRC_AlerteDisponibiliteRemorqueurs,
        eRC_AlerteDisponibiliteReparateurs,
        eRC_AlerteDisponibiliteVecteurs,
        eRC_PretMaterielEnCours,
        eRC_PretMaterielAnnule,
        eRC_PretMaterielEffectue, // 120
        eRC_PretMaterielImpossible,
        eRC_PretMaterielPartiellementEffectue,
        eRC_RecuperationMaterielPreteImpossible,
        eRC_RecuperationMaterielPreteEffectuee,
        eRC_RecuperationMaterielPretePartiellementEffectuee,
        eRC_MaterielPrete,
        eRC_MaterielRendu,
        eRC_Convoi_DeplacementVersPointChargement,
        eRC_Convoi_ChargementEnCours,
        eRC_Convoi_ChargementEffectue,
        eRC_Convoi_DeplacementVersPointDechargement,
        eRC_Convoi_DechargementEnCours,
        eRC_Convoi_DechargementEffectue,
        eRC_Convoi_Retour,
        eRC_EnCoursRavitaillement,
        eRC_RavitaillementTermine,
        eRC_HelicoptereEnObservation,
        eRC_PerteInfoGuidage,
        eRC_TransportUnitePasPrete, // Parametre : T_Pion
        eRC_TransportUnitePrete, // 140 - Parametre : T_Pion
        eRC_TransportEnCours,
        eRC_TransportEmbarquement,
        eRC_TransportDebarquement,
        eRC_TransportImpossiblePasDeMoyens,
        eRC_VehiculeLaisseSurPosition,
        eRC_RejointPositionEmbarquement,
        eRC_EnAttenteTransporteurs,
        eRC_ContamineNBC,
        eRC_DecontamineNBC,
        eRC_TenueProtectionNBCMise,
        eRC_TenueProtectionNBCEnlevee,
        eRC_AlerteNBCNiveau4,
        eRC_FinAlerteNBCNiveau4,
        eRC_SiteDecontaminationAnime,
        eRC_SiteDecontaminationDesactive,
        eRC_DemandeDecontamination,
        eRC_ZoneReconnue,
        eRC_ZoneDecontaminee,
        eRC_SiteROTAReconnu,
        eRC_IncidentNBCTermine, // 160
        eRC_UniteDecontaminee, // Parametre : T_Pion
        eRC_SiteSature,
        eRC_DebutMesure,
        eRC_FinMesure,
        eRC_ReconnaissanceTerminee,
        eRC_ReconnaissanceInitialeTerminee,
        eRC_RejointPlotRavitaillement,
        eRC_PremierCoupParti,
        eRC_TirExecute,
        eRC_TirHorsDePortee,
        eRC_TirImpossiblePlusDeMunitions,
        eRC_TirAnnule,
        eRC_ChangementDePosition,
        eRC_DebutInstallation,
        eRC_FinInstallation,
        eRC_PretPourConduiteTir,
        eRC_ObservationTirIndirect, // Parametre : TirPion
        eRC_ZoneImplantationOccupee,
        eRC_InstallationSurZoneImplantation,
        eRC_ReceptionOrdreDeFeu, // 180
        eRC_PretAuTir,
        eRC_PresencePopulation,
        eRC_OrientationPopulationVersCampRefugies,
        eRC_PasDeCampRefugiesDisponible,
        eRC_PresenceEnnemiRendu,
        eRC_OrientationEnnemiRenduVersCampPrisonniers,
        eRC_PasDeCampPrisonniersDisponible,
        eRC_Rendu,
        eRC_RedditionAnnulee,
        eRC_TirDansZoneInterdite,
        eRC_TirSurCampAmi,
        eRC_TireParCampAmi,
        eRC_TirSurCampNeutre,
        eRC_TireParCampNeutre,
        eRC_TirSurCivil,
        eRC_TireParCivil,
        eRC_TirIndirectFratricide,
        eRC_TirIndirectSurPopulation,
        eRC_DestructionPC,
        eRC_DecesBlesse, // 200
        eRC_DecesBlessePendantTransport,
        eRC_DecesBlessePendantHospitalisation,
        eRC_MissionImpossibleUnitesSubordonneesNonOperationnelles,
        eRC_MissionImpossibleLimaInvalide,
        eRC_MissionImpossibleZoneHorsFuseau,
        eRC_RapportDeForceDefavorable,
        eRC_TerrainDifficile,
        eRC_PlusDeCarburant,
        eRC_DispositifDisperse,
        eRC_DebutNeutralisation,
        eRC_FinNeutralisation,
        eRC_DebutSilenceRadio,
        eRC_FinSilenceRadio,
        eRC_DebutSilenceRadar,
        eRC_FinSilenceRadar,
        eRC_TirRestreint,
        eRC_TirInterdit,
        eRC_TirLibre,
        eRC_Trace, // Parametre : message
        eRC_TraceID, // 220 - Parametre : float
        eRC_EmploiForceInterdit,
        eRC_MaintienADistanceParMoyensNonLetaux,
        eRC_ArmesLetalesAutorisees,
        eRC_PopulationDetectee, // Parametre : T_ConnaissancePopulation
        eRC_PriseAPartieParPopulation, // Parametre : T_ConnaissancePopulation
        eRC_RiposteContrePopulation, // Parametre : T_ConnaissancePopulation
        eRC_PopulationDangereuseAProximite,
        eRC_EsquiveFaceAPopulation,
        eRC_DecrochageFaceAPopulation,
        eRC_MiseEnPlaceControleDeFoule,
        eRC_MiseEnPlaceFiltrage,
        eRC_AnimationBarrage,
        eRC_AnimationCanalisation,
        eRC_CollecteRenseignementsSurPopulations,
        eRC_PopulationReconnue,
        eRC_SituationDifficilementTenableFaceAPopulation,
        eRC_UtilisationArmesNonLetalesFaceAPopulation,
        eRC_UtilisationArmementsFaceAPopulation,
        eRC_DebutInterventionFaceAPopulation, // Parametre : id
        eRC_PopulationVictimeAffrontements, // 240 - Parametres : victimes (float), dont_blesses (float),
        eRC_AttitudePopulation, // Parametre : id
        eRC_AggravationAttitude,
        eRC_AffrontementAvecForcesEnnemies,
        eRC_PopulationTemporairementControlee,
        eRC_PopulationTemporairementRepoussee,
        eRC_RepriseDesAffrontements,
        eRC_AgressionDePopulation, // Parametre : T_ConnaissancePopulation
        eRC_AgressionParPopulation, // Parametre : T_ConnaissancePopulation
        eRC_RiposteContreAgressionParPopulation, // Parametre : T_ConnaissancePopulation
        eRC_AttaqueInstallation,
        eRC_EnStationnement,
        eRC_Bloquee,
        eRC_Filtree,
        eRC_AttentatTerroristeDansPopulation,
        eRC_PopulationVictimeExplosionMines, // Parametres : victimes (float), dont_blesses (float),
        eRC_ExecutionAttentat,
        eRC_BM_DebutMission,
        eRC_BM_FinMission,
        eRC_BM_DebutPhase,
        eRC_BM_FinPhase, // 260
        eRC_ActivationObstacleDeManoeuvre,
        eRC_DebutAnimationObjet,
        eRC_TirDirectSurCible,
        eRC_FinAnimationObjet,
        eRC_IllumineCible,
        eRC_DebutDegagement,
        eRC_FinDegagement,
        eRC_TirDirectImpossible,
        eRC_TirIndirectSurCible,
        eRC_TirSansCapacite,
        eRC_TirIndirectMunitionInvalide,
        eRC_TirIndirectImpossible,
        eRC_CibleNonPercue,
        eRC_EnnemiTactiquementDetruit,
        eRC_VisionRadarVerrouilleeSur,
        eRC_DebutReconnaissanceBlocUrbain,
        eRC_FinReconnaissanceBlocUrbain,
        eRC_DebutReconnaissancePoint,
        eRC_FinReconnaissancePoint,
        eRC_FinTirDirectSurCible, // 280
        eRC_DebutTravauxObjet,
        eRC_FinTravauxObjet,
        eRC_FinTirIndirectSurCible,
        eRC_DebutNeutralisationCible,
        eRC_FinNeutralisationCible,
        eRC_VisionVerrouilleeSur,
        eRC_DebutConstructionObjet,
        eRC_ObjetDejaOccupe,
        eRC_DebutSilencePartielRadio,
        eRC_DebutReconnaissanceZone,
        eRC_FinReconnaissanceZone,
        eRC_ConstructionObjetImpossible,
        eRC_PasDotationConstructionObjet,
        eRC_Embarquement,
        eRC_Debarquement,
        eRC_DebutFouilleBlocUrbain,
        eRC_FinFouilleBlocUrbain,
        eRC_DebutFouilleZone,
        eRC_FinFouilleZone,
        eRC_MunitionInterdite, // 300
        eRC_MunitionAutorise,
        eRC_DebutBouclageZone,
        eRC_FinBouclageZone,
        eRC_DebutAppui,
        eRC_FinAppui,
        eRC_ObjetNonContournable,
        eRC_ObjetNonSupprimable,
        eRC_TirIndirectTargetNotIlluminated,
        eRC_TirIndirectForbiddenAmmo,
        eRC_MissionImpossibleReason,
        eRC_CouvertureActive,
        eRC_CouvertureDesactive,
        eRC_DebutControleBlocUrbain,
        eRC_FinControleBlocUrbain,
        eRC_DebutControlPoint,
        eRC_FinControlPoint,
        eRC_TirIndirectNoCapacity,
        eRC_DebutSilenceRadioPartiel,
        eRC_EmbarkmentStarted,
        eRC_EmbarkmentInterrupted, // 320
        eRC_EmbarkmentResumed,
        eRC_EmbarkmentFinished,
        eRC_DisembarkmentStarted,
        eRC_DisembarkmentInterrupted,
        eRC_DisembarkmentResumed,
        eRC_DisembarkmentFinished,
        eRC_ActiveProtectionCounter,
        eRC_ActiveProtectionHardKill,
        eRC_Poisoned,
        eRC_Contaminated,
        eRC_TirSurCampEnnemi,
        eRC_TireParCampEnnemi,
        eRC_DroneDoitEtreMisEnOeuvre,
        eRC_ReussiteRenforcementPion,
        eRC_EchecRenforcementPion,
        eRC_LeaveLima,
        eRC_ArrivedOnExtinguishableObject,
        eRC_ObjectExtinguished,
        eRC_ArrivedOnAlertZone,
        eRC_ZoneAlerted, // 340
        eRC_ArrivedOnGaragePosition,
        eRC_ExtractFromStock_Arrived,
        eRC_ExtractFromStock_Extracted,
        eRC_SupplyStock_Supplied,
        eRC_SupplyStock_Arrived,
        eRC_SupplyStock_Distributed,
        eRC_ZoneConfined,
        eRC_ArrivedOnConfinedZone,
        eRC_PasDeDroneDisponible,
        eRC_FirstTourOfZoneConfinementDone,
        eRC_ZoneConfinementInProgress,
        eRC_ZoneEvacuated,
        eRC_ArrivedOnEvacuateZone,
        eRC_ZoneEvacuationInProgress,
        eRC_DebutValorisation,
        eRC_FinValorisation,
        eRC_EchecRelever,
        eRC_criticalIntelligence,
        eRC_criticalIntelligenceNone,
        eRC_FinTravauxBlocUrbain, // 360
        eRC_BUNonTraficable,
        eRC_StartDeploy,
        eRC_DebutFouillePoint,
        eRC_FinFouillePoint,
        eRC_NotDeployedIndirectFireImpossible,
        eRC_AgressionImpossible,
        eRC_NoLogisticSuperior,
        eRC_CrossingSiteNotRecon,
        eRC_InvalidObject,
        eRC_TerroristCaptured,
        eRC_MiseEnPlaceProducePoint,
        eRC_ProduceImpossible,
        eRC_FinProducePoint,
        eRC_TimeInUndergroundNetwork,
        eRC_BadUndergroundNetwork,
        eRC_NotActivatedUndergroundNetwork,
        eRC_ImpossibleAtteindreObjectif,
        eRC_ExplosionSurBouchonMine,
        eRC_PathfindWarning,
        eRC_DotationIndisponible, // 380
        eRC_ObservationEffetTirIndirect,
        eRC_DamagesCausedToNeutralSide,
        eRC_DamagesCausedByNeutralSide,
        eRc_InfrastructureDamaged,
        eRc_LivingAreaDamaged,
        eRc_BorderCrossed,
        eRc_TacticallyDestroyed,
        eRc_CiviliansEncountered,
        eRc_PrisonersUnsupplied,
        eRc_PrisonersCampFull,
		eRC_NoPositionsToDestroyTargets,
		eRC_NoPositionsToNeutralizeTargets,
		eRC_NoPositionsToGetCoverFromEntities,
		eRC_NoPositionsToObserveTargets,
		eRC_NoPositionsToReachTargets,
		eRC_TheTargetCannotBeReconnoitre,
		eRC_TargetIsNotAccessible,
		eRC_NoKeyPositionsFoundInArea,
		eRC_Engaged,
		eRC_Disengaged,
		eRC_AlreadyEngaged,
		eRC_AlreadyDisengaged,
		eRC_NoPEInAutomat,
        eLast
    };
    //@}
public:
    //! @name Factory
    //@{
    static void Initialize( xml::xistream& xis );
    static const MIL_Report* Find( unsigned int nID );
    //@}

    //! @name Operations
    //@{
    template< typename T > void Send( const T& sender, E_Type nType, std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > >& diaParameters ) const;
    //@}

    //! @name $$ BOF
    //@{
    template< typename T > static void PostEvent( const T& receiver, E_EngineReport nReport );
    template< typename T > static void PostEvent( const T& receiver, E_EngineReport nReport, const PHY_ComposanteTypePion& parameter );
    template< typename T > static void PostEvent( const T& receiver, E_EngineReport nReport, const PHY_DotationCategory& parameter );
    template< typename T > static void PostEvent( const T& receiver, E_EngineReport nReport, double nParam1, double nParam2 );
    template< typename T > static void PostEvent( const T& receiver, E_EngineReport nReport, int nParam );
    template< typename T > static void PostEvent( const T& receiver, E_EngineReport nReport, int nParam1, int nParam2, int nParam3 );
    template< typename T > static void PostEvent( const T& receiver, E_EngineReport nReport, const MIL_Effect_IndirectFire& flyingShell );
    template< typename T > static void PostEvent( const T& receiver, E_EngineReport nReport, boost::shared_ptr< DEC_Knowledge_Agent > agentKnowledge );
    //@}

private:
    //! @name Constructors/Destructor
    //@{
     MIL_Report( unsigned int nID, xml::xistream& xis );
    ~MIL_Report();
    //@}

    //! @name Tools
    //@{
    template< typename T >
    static void PostEvent( const T& receiver, E_EngineReport nReport, std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > >& parameters );

    bool DoSend( client::Report& message, E_Type nType, std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > >& params ) const;
    //@}

    //! @name Helpers
    //@{
    static void ReadReport( xml::xistream& xis );
    void ReadParameter( xml::xistream& xis );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< unsigned int, const MIL_Report* > T_ReportMap;
    typedef T_ReportMap::const_iterator               CIT_ReportMap;

    typedef std::vector< const MIL_ParameterType_ABC* > T_ParameterVector;
    typedef T_ParameterVector::const_iterator         CIT_ParameterVector;

    typedef std::vector< E_DecisionalReport > T_DiaEventVector;
    //@}

private:
    //! @name Member data
    //@{
    unsigned int nID_;
    std::string strMessage_;
    E_Type category_;
    T_ParameterVector parameters_;
    static T_ReportMap reports_;
    static T_DiaEventVector diaEvents_;
    static MIL_IDManager ids_;
    //@}
};

#include "MIL_Report.inl"

#endif // __MIL_Report_h_
