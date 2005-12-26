// *****************************************************************************
//
// $Created: SBO 2005-05-13 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Communications/MessageReport.h $
// $Author: Sbo $
// $Modtime: 20/07/05 18:10 $
// $Revision: 1 $
// $Workfile: MessageReport.h $
//
// *****************************************************************************

#ifndef __MessageReport_h_
#define __MessageReport_h_

#include "Types.h"
#include "CommunicationType.h"

namespace TIC
{

// =============================================================================
// @class  MessageReport
// Created: SBO 2005-06-30
// =============================================================================
class MessageReport
{
    MT_COPYNOTALLOWED( MessageReport )

public:
    //! @name constant
    //@{
    static const MessageReport notDefined;
    //@}

public:
    //! @name Manager
    //@{
    static void Initialize();
    static void Terminate ();

    static const MessageReport& Find( const std::string& strName );
    static const MessageReport& Find( uint               asnId   );
    //@}

    //! @name Accessors
    //@{
    const std::string&                GetName   () const;
          uint                        GetID     () const;
          uint                        GetAsnID  () const;
    const CommunicationType::ESubType GetSubType() const;
    //@}

private:
    //! @name Types
    //@{
    enum E_Type
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
        eRC_PreparationMission, // Parametre : delai (float), 
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
        eRC_UniteTropDistante,
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
        eRC_SectionDeployee,
        eRC_ANouveauDisponibleApresReparation,
        eRC_MaterielRetourDeMaintenance,
        eRC_MaterielRepareSurPlace,
        eRC_DemandeRavitaillement,
        eRC_DemandeEvacuationSanitaire,
        eRC_DemandeEvacuationMateriel,
        eRC_AllocationConsentieBientotEpuisee, // Parametre : TypeDotation
        eRC_RavitaillementDotationsEffectue,
        eRC_RavitaillementStockEffectue,
        eRC_HumainRetourDeSante,
        eRC_VSRAM_PretEnCours,
        eRC_VSRAM_PretAnnule,
        eRC_VSRAM_PretEffectue,
        eRC_VSRAM_RecuperationEffectuee,
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
        eRC_TirDansZoneInterdite,
        eRC_TirSurCampAmi, // Parametre : T_Pion
        eRC_TireParCampAmi, // Parametre : T_Pion
        eRC_TirSurCampNeutre, // Parametre : T_Pion
        eRC_TireParCampNeutre, // Parametre : T_Pion
        eRC_TirSurCivil, // Parametre : T_Pion
        eRC_TireParCivil, // Parametre : T_Pion
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
        eRC_UniteDetectee, // Parametre : T_ConnaissanceAgent
        eRC_UniteAmieReconnue, // Parametre : T_ConnaissanceAgent
        eRC_UniteEnnemieReconnue, // Parametre : T_ConnaissanceAgent
        eRC_UniteNeutreReconnue, // Parametre : T_ConnaissanceAgent
        eRC_UniteAmieIdentifiee, // Parametre : T_ConnaissanceAgent
        eRC_UniteEnnemieIdentifiee, // Parametre : T_ConnaissanceAgent
        eRC_UniteNeutreIdentifiee, // Parametre : T_ConnaissanceAgent
        eRC_ObjetDetecte, // Parametre : T_ConnaissanceObjet
        eRC_Trace, // Parametre : message
        eRC_TraceID, // Parametre : float
        eRC_TirInterditSurPopulation,
        eRC_TirSommationSurPopulation,
        eRC_TirDissuasionSurPopulation,
        eRC_TirNeutralisationSurPopulation,
        eRC_PopulationDetectee, // Parametre : T_ConnaissancePopulation
        eRC_PriseAPartieParPopulation, // Parametre : T_ConnaissancePopulation
        eRC_RiposteSurPopulation, // Parametre : T_ConnaissancePopulation
        eRC_PopulationDangereuseAProximite,
        eRC_EsquiveDeLaPopulation,
        eRC_DecrochageFaceAPopulation,
        eRC_MiseEnPlaceControleDeFoule,
        eRC_MiseEnPlaceFiltrage,
        eRC_AnimationBarrage,
        eRC_AnimationCanalisation,
        eRC_CollecteRenseignementsSurPopulations,
        eRC_PopulationTotalementReconnue,
        eRC_AttitudeCalme,
        eRC_AttitudeAgitee,
        eRC_AttitudeExcitee,
        eRC_AttitudeAgressive,
        eRC_AggravationAttitude,
        eRC_AffrontementAvecForcesEnnemies,
        eRC_PopulationTemporairementControlee,
        eRC_PopulationTemporairementRepoussee,
        eRC_RepriseDesAffrontements,
        eRC_RecuperationProgressive, // Parametre : float
        eRC_AgressionDePopulation, // Parametre : T_ConnaissancePopulation
        eRC_AgressionParPopulation, // Parametre : T_ConnaissancePopulation
        eRC_RiposteContreAgressionParPopulation, // Parametre : T_ConnaissancePopulation
        eRC_AttaqueInstallation, // Parametre : T_ConnaissanceObjet
        eRC_EnStationnement,
        eRC_ExecutionAttentat,

        eNone
    };

    typedef std::map< std::string, const MessageReport*, sCaseInsensitiveLess > T_MessageReportMap;
    typedef T_MessageReportMap::const_iterator                                  CIT_MessageReportMap;
    //@}

private:
    //! @name Constructors/Destructor
    //@{
     MessageReport( const std::string& strName, E_Type nType, uint nAsnID, CommunicationType::ESubType eSubType );
    ~MessageReport();
    //@}

private:
    //! @name Member data
    //@{
    const std::string                 strName_;
    const E_Type                      nType_;
    const uint                        nAsnID_;
    const CommunicationType::ESubType eSubType_;
    //@}

    //! @name Static member
    //@{
    static T_MessageReportMap messages_;
    //@}
};

} // end namespace TIC

#include "MessageReport.inl"

#endif // __MessageReport_h_
