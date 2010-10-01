// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_Report_h_
#define __MIL_Report_h_

#include "MIL_MissionParameterFactory.h"
#include "MT_Tools/MT_IdentifierManager.h"

namespace xml
{
    class xistream;
}

class MIL_ParameterType_ABC;
class DEC_KnowledgeResolver_ABC;
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
        eReport_IndirectFireOnPopulation,
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
        eReport_EquipmentLoanPartiallyDone,
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
        eReport_EmbarkmentInterrupted,
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
        eReport_MunitionInterdite,
        eReport_MunitionAutorise,
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
        eRC_Assaut,
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
        eRC_Decroche,
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
        eRC_DesactivationSystemeEcoute,
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
        eRC_FinTravaux,
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
        eRC_DemandeRavitaillementStocks,
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
        eRC_PretMaterielEffectue,
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
        eRC_TransportUnitePrete, // Parametre : T_Pion
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
        eRC_IncidentNBCTermine,
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
        eRC_ReceptionOrdreDeFeu,
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
        eRC_DecesBlesse,
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
        eRC_TraceID, // Parametre : float
        eRC_EmploiForceInterdit,
        eRC_MaintienADistanceParMoyensNonLetaux,
        eRC_DispersionParMoyensDeDefenseActifs,
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
        eRC_UtilisationMoyensDefenseActifsFaceAPopulation,
        eRC_UtilisationArmementsFaceAPopulation,
        eRC_DebutInterventionFaceAPopulation, // Parametre : id
        eRC_PopulationVictimeAffrontements, // Parametres : victimes (float), dont_blesses (float),
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
        eRC_Embarquement,
        eRC_Debarquement,
        eRC_EmbarquementFin,
        eRC_DebarquementFin,
        eRC_DebutRatissageBlocUrbain,
        eRC_FinRatissageBlocUrbain,
        eRC_DebutRatissageZone,
        eRC_FinRatissageZone,
        eRC_DebutBouclageZone,
        eRC_FinBouclageZone,
        eRC_DebutAppui,
        eRC_FinAppui,
        eRC_ObjetNonContournable,

        eRC_TirIndirectSurCible = 271,
        eRC_MunitionInterdite = 302,
        eRC_MunitionAutorise = 303,

        eRC_EmbarkmentStarted = 3000,
        eRC_EmbarkmentInterrupted = 3001,
        eRC_EmbarkmentResumed = 3002,
        eRC_EmbarkmentFinished = 3003,
        eRC_DisembarkmentStarted = 3004,
        eRC_DisembarkmentInterrupted = 3005,
        eRC_DisembarkmentResumed = 3006,
        eRC_DisembarkmentFinished = 3007,
        eRC_ActiveProtectionCounter = 3008,
        eRC_ActiveProtectionHardKill = 3009,
        eRC_Poisoned = 3010,
        eRC_Contaminated = 3011,
        eRC_TirSurCampEnnemi = 3012,
        eRC_TireParCampEnnemi = 3013,
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

    bool DoSend( unsigned int nSenderID, E_Type nType, const DEC_KnowledgeResolver_ABC& knowledgeResolver, std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > >& params ) const;
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
    T_ParameterVector parameters_;
    static T_ReportMap reports_;
    static T_DiaEventVector diaEvents_;
    static MT_IdentifierManager ids_;
    //@}
};

#include "MIL_Report.inl"

#endif // __MIL_Report_h_
