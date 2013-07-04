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

    enum E_DecisionalReport
    {
        eRC_MissionImpossible = 3,
        eRC_PrisSousTirArtillerie = 37,
        eRC_RegimeMaintenanceDelaiDepasse = 94,
        eRC_SectionDeployee = 95,
        eRC_ANouveauDisponibleApresReparation = 96,
        eRC_MaterielRetourDeMaintenance = 97,
        eRC_MaterielRepareSurPlace = 98,
        eRC_DemandeRavitaillementDotations = 99,
        eRC_DemandeRavitaillementStocks = 100,
        eRC_DemandeEvacuationSanitaire = 101,
        eRC_DemandeEvacuationMateriel = 102,
        eRC_AllocationConsentieBientotEpuisee = 103,
        eRC_DepassementCapaciteStockage = 104,
        eRC_SeuilLogistiqueDotationDepasse = 105,
        eRC_SeuilLogistiqueStockDepasse = 106,
        eRC_RavitaillementDotationsEffectue = 107,
        eRC_RavitaillementStockEffectue = 108,
        eRC_RavitaillementDotationsAnnule = 109,
        eRC_RavitaillementStockAnnule = 110,
        eRC_HumainRetourDeSante = 111,
        eRC_AlerteDisponibiliteMoyensReleve = 112,
        eRC_AlerteDisponibiliteMoyensRamassage = 113,
        eRC_AlerteDisponibiliteMedecins = 114,
        eRC_AlerteDisponibiliteRemorqueurs = 115,
        eRC_AlerteDisponibiliteReparateurs = 116,
        eRC_AlerteDisponibiliteVecteurs = 117,
        eRC_PretMaterielEnCours = 118,
        eRC_PretMaterielAnnule = 119,
        eRC_PretMaterielEffectue = 120,
        eRC_PretMaterielImpossible = 121,
        eRC_PretMaterielPartiellementEffectue = 122,
        eRC_RecuperationMaterielPreteImpossible = 123,
        eRC_RecuperationMaterielPreteEffectuee = 124,
        eRC_RecuperationMaterielPretePartiellementEffectuee = 125,
        eRC_MaterielPrete = 126,
        eRC_MaterielRendu = 127,
        eRC_ObservationTirIndirect = 177,
        eRC_Rendu = 188,
        eRC_RedditionAnnulee = 189,
        eRC_TirDansZoneInterdite = 190,
        eRC_TirSurCampAmi = 191,
        eRC_TireParCampAmi = 192,
        eRC_TirSurCampNeutre = 193,
        eRC_TireParCampNeutre = 194,
        eRC_TirSurCivil = 195,
        eRC_TireParCivil = 196,
        eRC_TirIndirectFratricide = 197,
        eRC_TirIndirectSurPopulation = 198,
        eRC_DestructionPC = 199,
        eRC_DecesBlesse = 200,
        eRC_DecesBlessePendantTransport = 201,
        eRC_DecesBlessePendantHospitalisation = 202,
        eRC_TerrainDifficile = 207,
        eRC_PlusDeCarburant = 208,
        eRC_DebutNeutralisation = 210,
        eRC_FinNeutralisation = 211,
        eRC_PriseAPartieParPopulation = 225,
        eRC_DebutInterventionFaceAPopulation = 239,
        eRC_PopulationVictimeAffrontements = 240,
        eRC_AttitudePopulation = 241,
        eRC_Bloquee = 252,
        eRC_Filtree = 253,
        eRC_AttentatTerroristeDansPopulation = 254,
        eRC_PopulationVictimeExplosionMines = 255,
        eRC_TirIndirectSurCible = 269,
        eRC_MunitionInterdite = 300,
        eRC_MunitionAutorise = 301,
        eRC_EmbarkmentStarted = 319,
        eRC_EmbarkmentInterrupted = 320,
        eRC_EmbarkmentFinished = 322,
        eRC_DisembarkmentStarted = 323,
        eRC_DisembarkmentInterrupted = 324,
        eRC_DisembarkmentFinished = 326,
        eRC_ProtectionActiveReussie = 327,
        eRC_ProtectionActiveHardKillReussie = 328,
        eRC_Empoisonne = 329,
        eRC_Contamine = 330,
        eRC_TirSurCampEnnemi = 331,
        eRC_TireParCampEnnemi = 332,
        eRC_StartDeploy = 362,
        eRC_NotActivatedUndergroundNetwork = 376,
        eRC_ExplosionSurBouchonMine = 378,
        eRC_DamagesCausedToNeutralSide = 382,
        eRC_DamagesCausedByNeutralSide = 383,
        eRC_InfrastructureDamaged = 384,
        eRC_LivingAreaDamaged = 385,
        eRC_BorderCrossed = 386,
        eRC_TacticallyDestroyed = 387,
        eRC_CiviliansEncountered = 388,
        eRC_PrisonersUnsupplied = 389,
        eRC_PrisonersCampFull = 390,
        eRC_SectionUndeployed = 404,
        eRC_StartUndeploy = 405,
        eRC_DifficultMovementProgression = 406,
        eRC_UrbanCollisionStarted = 408,
        eRC_UrbanCollisionStopped = 409,
        eRC_PrisSousTirIED = 417,
        eRC_Questionning = 420,
        eRC_FragOrderReceived = 423,
        eRC_FireObserver = 427,
        eRC_DetectedUnit = 436,
        eRC_FriendUnitRecognized = 437,
        eRC_FriendUnitIdentified = 438,
        eRC_EnemyUnitRecognized = 439,
        eRC_EnemyUnitIdentified = 440,
        eRC_NeutralUnitRecognized = 441,
        eRC_NeutralUnitIdentified = 442,
        eRC_PrisSousTirEclairant = 444,
        eRC_PrisSousTirFumigene = 445,
        eRC_DetectedObject = 446,
        eRC_RepairEvacuationNoMeans = 449,
        eRC_SupplierUnavailable = 450,
        eRC_LogQuotaExceeded = 451,
        eRC_LogQuotaExceededForAgent = 452,
        eRC_TransportedUnitCannotReceiveOrder = 453,
        eRC_LogNoSuperior = 454,
        eRC_LogNoStock = 455,
        eRC_AttenteOrdrePoursuivreConvoi = 503,
        eRC_TirIndirectLessInterventionType = 504,
        eRC_CloseCrowdAttitudeChanged = 506,
        eRC_CloseCrowdUrbanDestruction = 507,
        eRC_SurrenderedUnitRecognized = 510,
        eRC_SurrenderedUnitIdentified = 511,
        eRC_TransportRoundTrip = 512,
        eRC_LogSuperiorAdded = 513,
        eRC_LogSuperiorRemoved = 514,
        eRC_CloseCrowdDemonstration = 517,
        eRC_MilitaryEquipmentPlundered = 518,
        eRC_CorpseTransported = 769,
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
    template< typename T > static void PostEvent( const T& receiver, E_DecisionalReport nReport );
    template< typename T > static void PostEvent( const T& receiver, E_DecisionalReport nReport, const MIL_AgentPion& pion );
    template< typename T > static void PostEvent( const T& receiver, E_DecisionalReport nReport, const PHY_ComposanteTypePion& parameter );
    template< typename T > static void PostEvent( const T& receiver, E_DecisionalReport nReport, const PHY_DotationCategory& parameter );
    template< typename T > static void PostEvent( const T& receiver, E_DecisionalReport nReport, const PHY_DotationCategory& parameter, const MIL_AgentPion& pion );
    template< typename T > static void PostEvent( const T& receiver, E_DecisionalReport nReport, double nParam1, double nParam2 );
    template< typename T > static void PostEvent( const T& receiver, E_DecisionalReport nReport, int nParam );
    template< typename T > static void PostEvent( const T& receiver, E_DecisionalReport nReport, const std::string& nParam );
    template< typename T > static void PostEvent( const T& receiver, E_DecisionalReport nReport, int nParam1, int nParam2, int nParam3 );
    template< typename T > static void PostEvent( const T& receiver, E_DecisionalReport nReport, const MIL_Effect_IndirectFire& flyingShell );
    template< typename T > static void PostEvent( const T& receiver, E_DecisionalReport nReport, boost::shared_ptr< DEC_Knowledge_Agent > agentKnowledge );
    template< typename T > static void PostEvent( const T& receiver, E_DecisionalReport nReport, boost::shared_ptr< DEC_Knowledge_Population >& populationKnowledge );
    template< typename T > static void PostEvent( const T& receiver, E_DecisionalReport nReport, boost::shared_ptr< DEC_Knowledge_Population >& populationKnowledge, int nParam2 );
    template< typename T > static void PostEvent( const T& receiver, E_DecisionalReport nReport, int nParam1, const std::string& nParam2, int nParam3, int nParam4, int nParam5 );
    template< typename T > static void PostEvent( const T& receiver, E_DecisionalReport nReport, std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > >& parameters );
    //@}

private:
    //! @name Constructors/Destructor
    //@{
     MIL_Report( unsigned int nID, xml::xistream& xis );
    ~MIL_Report();
    //@}

    //! @name Tools
    //@{
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
    //@}

private:
    //! @name Member data
    //@{
    unsigned int nID_;
    std::string strMessage_;
    E_Type category_;
    T_ParameterVector parameters_;
    static T_ReportMap reports_;
    static MIL_IDManager ids_;
    //@}
};

#include "MIL_Report.inl"

#endif // __MIL_Report_h_
