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
class MIL_RC_ObservationTirIndirect;
class MIL_RC_AllocationConsentieBientotEpuisee;
class MIL_RC_TirSurCampAmi;   
class MIL_RC_TireParCampAmi; 
class MIL_RC_TirSurCampNeutre;
class MIL_RC_TireParCampNeutre;
class MIL_RC_TirSurCivil;      
class MIL_RC_TireParCivil;
class MIL_RC_PionDecontamine;

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
    static const MIL_RC*                                    pRcMaterielRetourDeMaintenance_;
    static const MIL_RC_ObservationTirIndirect*             pRcObservationTirIndirect_;
    static const MIL_RC*                                    pRcDemandeRavitaillement_;
    static const MIL_RC*                                    pRcDemandeEvacuationSanitaire_;
    static const MIL_RC*                                    pRcDemandeEvacuationMateriel_;
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
    static const MIL_RC_PionDecontamine*                    pRcPionDecontamine_;
    static const MIL_RC*                                    pRcPlusDeCarburant_;
    static const MIL_RC*                                    pRcTerrainDifficile_;
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
    virtual void Send( const MIL_AgentPion& sender, E_RcType nType, DIA_Parameters& parameters ) const;
    virtual void Send( const MIL_Automate&  sender, E_RcType nType, DIA_Parameters& parameters ) const;

    template < typename T > void Send( const T& sender, E_RcType nType ) const;
    //@}

protected:
    //! @name Types
    //@{
    enum E_RC
    {
        /* RAS */
        eRC_RAS,

        /* Obstacle */
        eRC_EnPosteFaceAObstacle,       // Paramètre : Rep_Objet
        eRC_EnContournementObstacle,
    
        /* Phase de mission */
        eRC_MissionTerminee,
        eRC_MissionImpossible,
        eRC_PretPourNouvelleMission,
        eRC_RejointFuseau,
        eRC_ProgressionSurAxe,
        eRC_ProgressionVersLocalisation,
        eRC_ArriveSurLocalisation,
        eRC_ReconnaissanceLocalisation,
        eRC_EnPosteFaceADirectionDangereuse,
        eRC_EnPosteSurBond,
        eRC_EnJalonnement,
        eRC_PreparationMission,      // Paramètre OPTIONAL : delai en float
        eRC_DebutMission,
        eRC_FinMission,
        eRC_DispositifInstalle,
        eRC_EnPosition,
        eRC_ObjectifAtteint,
        eRC_FuseauRejoint,
        eRC_Debouche,
        eRC_SurPIA,                     // Paramètres T_Pion
        eRC_PassageSurPIA,
        eRC_UniteRecueillie,            // Paramètres T_Pion
        eRC_EnCoursDeFranchissement,
        eRC_AttenteOrdrePoursuivre,
        eRC_AttenteOrdreDeboucher,
    
        /* Ennemi */
        eRC_EnPause,                    // Paramètre : Rep_Pion
        eRC_EnEsquive,                  // Paramètre : Rep_Pion
        eRC_PrecisionContact,
        eRC_PionPriseContact,
        eRC_PionRuptureContact,
        eRC_EnnemiDetruit,              // Paramètre : Rep_Pion
        eRC_PerteDeContactVisuel,       
        eRC_PosteFaceEni,
        eRC_Decroche,
        eRC_PriseAPartie,
        eRC_PointTenuParEni,         // Paramètre : Rep_Pion
        eRC_PresenceEniProbable,
        eRC_ProgressionVersEni,
        eRC_EniSurObjectif,
        eRC_Riposte,
        eRC_Automate_PriseContact,
        eRC_Automate_RuptureContact,
    
        /* Rens */
        eRC_EnControleZone,
        eRC_EnSurveillance,
        eRC_EnInfiltration,
        eRC_EnExfiltration,
        eRC_AttenteConditionFavorablePourInfiltration,
        eRC_DroneLance,
        eRC_DroneEnExploitation,
        eRC_DroneExploite,
        eRC_DebutMiseEnOeuvreDrone,
        eRC_FinMiseEnOeuvreDrone,
        eRC_Brouille,
        eRC_EnEcoute,
        eRC_EnEcouteRadar,
        
        /* Ami */
        eRC_Releve,                 // Paramètres T_Pion
        eRC_SuitUnite,
        eRC_PasseEnAppui,
        eRC_EnAppui,
       
        /* Etat du pion */
        eRC_DetruitTactiquement,
    
        /* Deplacement */
        eRC_AmbianceVitesse,
        eRC_AmbianceSurete,
        eRC_SurLima,                    // Paramètre : enum E_TypeLima
        eRC_SurTypeTerrainParticulier,  // Paramètre : enum Type Terrain
    
        /* Génie */
        eRC_TravauxRealises,
        eRC_ObstacleDegage,
        eRC_OrganisationChantier,
        eRC_DesorganisationChantier,
        eRC_ReconnaissancePositionObstacle,
        eRC_ContournementRealise,
        eRC_AttenteRenforcement,
        eRC_DebutTravaux,
        eRC_DebutFranchissement,
        eRC_FinFranchissement,
        eRC_ReussiteRenforcement,        // Paramètres T_Pion, T_Automate
        eRC_EchecRenforcement,           // Paramètres T_Pion, T_Automate
        eRC_ItineraireRetabli,
        eRC_RenforcementBientotFini,     // Paramètre T_Pion
        eRC_ObstacleEnAttenteActivation, // Paramètre T_ConnaissanceObjet
    
        /* Logistique du pion */
        eRC_SituationLogFavorable,      
        eRC_SituationLogDefavorable,    // Paramètre : enum E_TypeDotation
    
        /* Log */
        eRC_MaterielRetourDeMaintenance,
        eRC_SectionDeployee,
        eRC_DemandeRavitaillement,
        eRC_DemandeEvacuationSanitaire,
        eRC_DemandeEvacuationMateriel,
        eRC_AllocationConsentieBientotEpuisee, // Paramètre dotation (envoyé par moteur)

        /* LOG / Transport / Embarquement */
        eRC_EnCoursRavitaillement,
        eRC_RavitaillementTermine,
        eRC_HelicoptereEnObservation,
        eRC_PerteInfoGuidage,
        eRC_TransportUnitePasPrete,
        eRC_TransportEnCours,
        eRC_TransportEmbarquement,
        eRC_TransportDebarquement,
        eRC_VehiculeLaisseSurPosition,
        eRC_RejointPositionEmbarquement,
        eRC_EnAttenteTransporteurs,
        
        /* NBC */
        eRC_ContamineNBC,
        eRC_DecontamineNBC,
        eRC_TenueProtectionNBCMise,
        eRC_TenueProtectionNBCEnlevee,
        eRC_AlerteNBC,
        eRC_SiteDecontaminationAnime,
        eRC_SiteDecontaminationDesactive,
        eRC_DemandeDecontamination,
        eRC_ZoneReconnue,
        eRC_ZoneDecontaminee,
        eRC_SiteROTAReconnu,
        eRC_IncidentNBCTermine,
        eRC_PionDecontamine,                // Paramètre T_Pion
    
        /* Artillerie */
        eRC_ReconnaissanceTerminee,
        eRC_ReconnaissanceInitialeTerminee,
        eRC_RejointPlotRavitaillement,
        eRC_PremierCoupParti,
        eRC_TirExecute,
        eRC_ChangementDePosition,
        eRC_DebutSurveillance,
        eRC_DebutInstallation,
        eRC_PretPourConduiteTir,
        eRC_ObservationTirIndirect,
        eRC_ReceptionOrdreDeFeu,
        eRC_PretAuTir,
        eRC_ZoneImplantationOccupee,
        eRC_InstallationSurZoneImplantation,
        eRC_TirHorsDePortee,
        eRC_TirAnnule,

        /* Population */
        eRC_PresencePopulation,
        eRC_OrientationPopulationVersCampRefugies,
        eRC_PasDeCampRefugiesDisponible,
        eRC_PresenceEnnemiRendu,
        eRC_OrientationEnnemiRenduVersCampPrisonniers,
        eRC_PasDeCampPrisonniersDisponible,

        /* Evt exceptionnels */
        eRC_TirDansZoneInterdite,
        eRC_TirSurCampAmi,
        eRC_TireParCampAmi,
        eRC_TirSurCampNeutre,
        eRC_TireParCampNeutre,
        eRC_TirSurCivil,
        eRC_TireParCivil,
        eRC_DestructionPC,
        eRC_DecesBlesse,
        eRC_DecesBlessePendantTransport,
        eRC_DecesBlessePendantHospitalisation,

        eRC_MissionImpossiblePionsSubordonnesNonOperationnels,
        eRC_MissionImpossibleLimaInvalide,
        eRC_MissionImpossibleZoneHorsFuseau,
        eRC_RapportDeForceDefavorable,
        eRC_TerrainDifficile,
        eRC_PlusDeCarburant,
        eRC_DispositifDisperse,
        eRC_PionNeutralise,
        eRC_SilenceRadio,
        eRC_TirRestreint,
        eRC_TirInterdit,
        eRC_TirLibre,        

        /* Adaptation */
        eRC_Trace, // Paramètre : ID en float
        eRC_TraceID, // Paramètre : string
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
