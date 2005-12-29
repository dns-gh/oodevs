// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Rc/MIL_RC.h $
// $Author: Nld $
// $Modtime: 10/05/05 18:16 $
// $Revision: 15 $
// $Workfile: MIL_RC.h $
//
// *****************************************************************************

#ifndef __MIL_RC_h_
#define __MIL_RC_h_

#include "MIL.h"

class MIL_AgentPion;
class MIL_Automate;
class MIL_Population;
class MIL_RC_ObservationTirIndirect;
class MIL_RC_AllocationConsentieBientotEpuisee;
class MIL_RC_TirSurCampAmi;   
class MIL_RC_TireParCampAmi; 
class MIL_RC_TirSurCampNeutre;
class MIL_RC_TireParCampNeutre;
class MIL_RC_TirSurCivil;      
class MIL_RC_TireParCivil;
class MIL_RC_UniteDecontaminee;
class MIL_RC_UniteDetectee;     
class MIL_RC_UniteAmieReconnue;  
class MIL_RC_UniteEnnemieReconnue;
class MIL_RC_UniteNeutreReconnue;
class MIL_RC_UniteAmieIdentifiee;  
class MIL_RC_UniteEnnemieIdentifiee;
class MIL_RC_UniteNeutreIdentifiee;
class MIL_RC_ObjetDetecte;
class MIL_RC_SeuilLogistiqueStockDepasse;
class MIL_RC_SeuilLogistiqueDotationDepasse;

struct ASN1T_MsgCR;

// =============================================================================
// @class  MIL_RC
// Created: JVT 2004-08-03
// =============================================================================
class MIL_RC
{
    MT_COPYNOTALLOWED( MIL_RC )
    
public:
    //! @name Types
    //@{
    typedef std::map< uint, const MIL_RC*> T_RCMap;
    typedef T_RCMap::const_iterator        CIT_RCMap;

    enum E_RcType
    {
        eRcTypeMessage     = 0,
        eRcTypeOperational = 1,
        eRcTypeEvent       = 2,
        eRcTypeWarning     = 3
    };
    //@}
    
    //! @name Statics
    //@{    
    static const MIL_RC*                                    pRcEnCoursDeFranchissement_;
    static const MIL_RC*                                    pRcANouveauDisponibleApresReparation_;
    static const MIL_RC*                                    pRcMaterielRepareSurPlace_;
    static const MIL_RC*                                    pRcMaterielRetourDeMaintenance_;
    static const MIL_RC_ObservationTirIndirect*             pRcObservationTirIndirect_;
    static const MIL_RC*                                    pRcDemandeRavitaillementStocks_;
    static const MIL_RC*                                    pRcDemandeRavitaillementDotations_;
    static const MIL_RC*                                    pRcDemandeEvacuationSanitaire_;
    static const MIL_RC*                                    pRcDemandeEvacuationMateriel_;
    static const MIL_RC*                                    pRcRavitaillementDotationsEffectue_;
    static const MIL_RC*                                    pRcRavitaillementStockEffectue_;
    static const MIL_RC*                                    pRcRavitaillementDotationsAnnule_;
    static const MIL_RC*                                    pRcRavitaillementStockAnnule_;
    static const MIL_RC_SeuilLogistiqueStockDepasse*        pRcSeuilLogistiqueStockDepasse_;
    static const MIL_RC_SeuilLogistiqueDotationDepasse*     pRcSeuilLogistiqueDotationDepasse_;
    static const MIL_RC*                                    pRcHumainRetourDeSante_;
    static const MIL_RC*                                    pRcDecesBlesse_;
    static const MIL_RC*                                    pRcDecesBlessePendantTransport_;
    static const MIL_RC*                                    pRcDecesBlessePendantHospitalisation_;
    static const MIL_RC*                                    pRcTirDansZoneInterdite_;
    static const MIL_RC_TirSurCampAmi*                      pRcTirSurCampAmi_;
    static const MIL_RC_TireParCampAmi*                     pRcTireParCampAmi_;
    static const MIL_RC_TirSurCampNeutre*                   pRcTirSurCampNeutre_;
    static const MIL_RC_TireParCampNeutre*                  pRcTireParCampNeutre_;
    static const MIL_RC_TirSurCivil*                        pRcTirSurCivil_;
    static const MIL_RC_TireParCivil*                       pRcTireParCivil_;
    static const MIL_RC*                                    pRcDestructionPC_;
    static const MIL_RC_AllocationConsentieBientotEpuisee*  pRcAllocationConsentieBientotEpuisee_;
    static const MIL_RC*                                    pRcMissionImpossible_;
    static const MIL_RC_UniteDecontaminee*                  pRcUniteDecontaminee_;
    static const MIL_RC*                                    pRcPlusDeCarburant_;
    static const MIL_RC*                                    pRcTerrainDifficile_;
    static const MIL_RC_UniteDetectee*                      pRcUniteDetectee_;
    static const MIL_RC_UniteAmieReconnue*                  pRcUniteAmieReconnue_;
    static const MIL_RC_UniteEnnemieReconnue*               pRcUniteEnnemieReconnue_;
    static const MIL_RC_UniteNeutreReconnue*                pRcUniteNeutreReconnue_;
    static const MIL_RC_UniteAmieIdentifiee*                pRcUniteAmieIdentifiee_;
    static const MIL_RC_UniteEnnemieIdentifiee*             pRcUniteEnnemieIdentifiee_;
    static const MIL_RC_UniteNeutreIdentifiee*              pRcUniteNeutreIdentifiee_;    
    static const MIL_RC_ObjetDetecte*                       pRcObjetDetecte_;   
    static const MIL_RC*                                    pRcRendu_;
    //@}
   
public:
    //! @name Manager
    //@{
    static void Initialize();
    static void Terminate ();

    static const MIL_RC* Find( uint nID );
    //@}
    
    //! @name Accessors
    //@{
    uint GetID() const;    
    //@}
    
    //! @name Operations
    //@{
    virtual void Send( const MIL_AgentPion&  sender, E_RcType nType, DIA_Parameters& parameters ) const;
    virtual void Send( const MIL_Automate&   sender, E_RcType nType, DIA_Parameters& parameters ) const;
    virtual void Send( const MIL_Population& sender, E_RcType nType, DIA_Parameters& parameters ) const;

    template < typename T > void Send( const T& sender, E_RcType nType ) const;
    //@}

protected:
    //! @name Types
    //@{
    enum E_RC
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
        eRC_DemandeRavitaillementDotations,
        eRC_DemandeRavitaillementStock,
        eRC_DemandeEvacuationSanitaire,
        eRC_DemandeEvacuationMateriel,
        eRC_AllocationConsentieBientotEpuisee, // Parametre : TypeDotation
        eRC_SeuilLogistiqueDotationDepasse, // Parametre : TypeDotation
        eRC_SeuilLogistiqueStockDepasse, // Parametre : TypeDotation
        eRC_RavitaillementDotationsEffectue,
        eRC_RavitaillementStockEffectue,
        eRC_RavitaillementDotationsAnnule,
        eRC_RavitaillementStockAnnule,
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
        eNbr

    };
    //@}


       
protected:
    MIL_RC( E_RC nRC, int nAsnID );
    MIL_RC( E_RC nRC );
    virtual ~MIL_RC();
    
    //! @name Tools
    //@{
    template< typename T > void FillRcMessage ( ASN1T_MsgCR& asnMsg, T& sender, E_RcType nType ) const;
    //@}

private:
    const E_RC  nRC_;
    const int   nAsnID_;

private:
    static T_RCMap rcs_;
};

#include "MIL_RC.inl"

#endif // __MIL_RC_h_
