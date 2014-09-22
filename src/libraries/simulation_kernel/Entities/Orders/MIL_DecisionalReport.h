// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_DecisionalReport_h_
#define __MIL_DecisionalReport_h_

#include <boost/noncopyable.hpp>
#include <string>

// =============================================================================
/** @class  MIL_DecisionalReport
    @brief  MIL_DecisionalReport
*/
// Created: SLI 2013-01-08
// =============================================================================
struct MIL_DecisionalReport : public boost::noncopyable
{
    //! @name Constructor/Destructors
    //@{
    MIL_DecisionalReport( const std::string& key ) : key_( key ) {}
    //@}

    //! @name Operators
    //@{
    operator const std::string&() const { return key_; }
    //@}

private:
    //! @name Member data
    //@{
    const std::string key_;
    //@}
};

namespace report
{
    extern const MIL_DecisionalReport eRC_AlerteDisponibiliteMedecins;
    extern const MIL_DecisionalReport eRC_AlerteDisponibiliteMoyensRamassage;
    extern const MIL_DecisionalReport eRC_AlerteDisponibiliteMoyensReleve;
    extern const MIL_DecisionalReport eRC_AlerteDisponibiliteRemorqueurs;
    extern const MIL_DecisionalReport eRC_AlerteDisponibiliteReparateurs;
    extern const MIL_DecisionalReport eRC_AlerteDisponibiliteVecteurs;
    extern const MIL_DecisionalReport eRC_AllocationConsentieBientotEpuisee;
    extern const MIL_DecisionalReport eRC_ANouveauDisponibleApresReparation;
    extern const MIL_DecisionalReport eRC_ArmedCiviliansDetected;
    extern const MIL_DecisionalReport eRC_AttentatTerroristeDansPopulation;
    extern const MIL_DecisionalReport eRC_AttitudePopulation;
    extern const MIL_DecisionalReport eRC_Bloquee;
    extern const MIL_DecisionalReport eRC_BorderCrossed;
    extern const MIL_DecisionalReport eRC_CiviliansEncountered;
    extern const MIL_DecisionalReport eRC_CloseCrowdAttitudeChanged;
    extern const MIL_DecisionalReport eRC_CloseCrowdDemonstration;
    extern const MIL_DecisionalReport eRC_CloseCrowdUrbanDestruction;
    extern const MIL_DecisionalReport eRC_Contamine;
    extern const MIL_DecisionalReport eRC_CorpseTransported;
    extern const MIL_DecisionalReport eRC_DamagesCausedByNeutralSide;
    extern const MIL_DecisionalReport eRC_DamagesCausedToNeutralSide;
    extern const MIL_DecisionalReport eRC_DebutInterventionFaceAPopulation;
    extern const MIL_DecisionalReport eRC_DebutNeutralisation;
    extern const MIL_DecisionalReport eRC_DecesBlesse;
    extern const MIL_DecisionalReport eRC_DecesBlessePendantHospitalisation;
    extern const MIL_DecisionalReport eRC_DecesBlessePendantTransport;
    extern const MIL_DecisionalReport eRC_DemandeEvacuationMateriel;
    extern const MIL_DecisionalReport eRC_DemandeEvacuationSanitaire;
    extern const MIL_DecisionalReport eRC_DemandeRavitaillementDotations;
    extern const MIL_DecisionalReport eRC_DemandeRavitaillementStocks;
    extern const MIL_DecisionalReport eRC_DepassementCapaciteStockage;
    extern const MIL_DecisionalReport eRC_DestructionPC;
    extern const MIL_DecisionalReport eRC_DetectedUnit;
    extern const MIL_DecisionalReport eRC_DifficultMovementProgression;
    extern const MIL_DecisionalReport eRC_DisembarkmentFinished;
    extern const MIL_DecisionalReport eRC_DisembarkmentInterrupted;
    extern const MIL_DecisionalReport eRC_DisembarkmentStarted;
    extern const MIL_DecisionalReport eRC_EmbarkmentFinished;
    extern const MIL_DecisionalReport eRC_EmbarkmentInterrupted;
    extern const MIL_DecisionalReport eRC_EmbarkmentStarted;
    extern const MIL_DecisionalReport eRC_Empoisonne;
    extern const MIL_DecisionalReport eRC_EnemyUnitIdentified;
    extern const MIL_DecisionalReport eRC_EnemyUnitRecognized;
    extern const MIL_DecisionalReport eRC_ExplosionSurBouchonMine;
    extern const MIL_DecisionalReport eRC_Filtree;
    extern const MIL_DecisionalReport eRC_FinNeutralisation;
    extern const MIL_DecisionalReport eRC_FireObserver;
    extern const MIL_DecisionalReport eRC_FragOrderReceived;
    extern const MIL_DecisionalReport eRC_FriendUnitIdentified;
    extern const MIL_DecisionalReport eRC_FriendUnitRecognized;
    extern const MIL_DecisionalReport eRC_HumainRetourDeSante;
    extern const MIL_DecisionalReport eRC_InfrastructureDamaged;
    extern const MIL_DecisionalReport eRC_Jammed;
    extern const MIL_DecisionalReport eRC_LivingAreaDamaged;
    extern const MIL_DecisionalReport eRC_LogNoStock;
    extern const MIL_DecisionalReport eRC_LogNoSuperior;
    extern const MIL_DecisionalReport eRC_LogQuotaExceeded;
    extern const MIL_DecisionalReport eRC_LogQuotaExceededForAgent;
    extern const MIL_DecisionalReport eRC_LogSuperiorAdded;
    extern const MIL_DecisionalReport eRC_LogSuperiorRemoved;
    extern const MIL_DecisionalReport eRC_MaterielPrete;
    extern const MIL_DecisionalReport eRC_MaterielRendu;
    extern const MIL_DecisionalReport eRC_MaterielRepareSurPlace;
    extern const MIL_DecisionalReport eRC_MaterielRetourDeMaintenance;
    extern const MIL_DecisionalReport eRC_MilitaryEquipmentPlundered;
    extern const MIL_DecisionalReport eRC_MissionImpossible;
    extern const MIL_DecisionalReport eRC_MunitionAutorise;
    extern const MIL_DecisionalReport eRC_MunitionInterdite;
    extern const MIL_DecisionalReport eRC_NeutralUnitIdentified;
    extern const MIL_DecisionalReport eRC_NeutralUnitRecognized;
    extern const MIL_DecisionalReport eRC_NotActivatedUndergroundNetwork;
    extern const MIL_DecisionalReport eRC_ObservationTirIndirect;
    extern const MIL_DecisionalReport eRC_PlusDeCarburant;
    extern const MIL_DecisionalReport eRC_PopulationVictimeAffrontements;
    extern const MIL_DecisionalReport eRC_PopulationVictimeExplosionMines;
    extern const MIL_DecisionalReport eRC_PretMaterielAnnule;
    extern const MIL_DecisionalReport eRC_PretMaterielEffectue;
    extern const MIL_DecisionalReport eRC_PretMaterielEnCours;
    extern const MIL_DecisionalReport eRC_PretMaterielImpossible;
    extern const MIL_DecisionalReport eRC_PretMaterielPartiellementEffectue;
    extern const MIL_DecisionalReport eRC_PriseAPartieParPopulation;
    extern const MIL_DecisionalReport eRC_PrisonersCampFull;
    extern const MIL_DecisionalReport eRC_PrisonersUnsupplied;
    extern const MIL_DecisionalReport eRC_PrisSousTirArtillerie;
    extern const MIL_DecisionalReport eRC_PrisSousTirEclairant;
    extern const MIL_DecisionalReport eRC_PrisSousTirFumigene;
    extern const MIL_DecisionalReport eRC_PrisSousTirIED;
    extern const MIL_DecisionalReport eRC_ProtectionActiveHardKillReussie;
    extern const MIL_DecisionalReport eRC_ProtectionActiveReussie;
    extern const MIL_DecisionalReport eRC_Questionning;
    extern const MIL_DecisionalReport eRC_RavitaillementDotationsAnnule;
    extern const MIL_DecisionalReport eRC_RavitaillementDotationsEffectue;
    extern const MIL_DecisionalReport eRC_RavitaillementStockAnnule;
    extern const MIL_DecisionalReport eRC_RavitaillementStockEffectue;
    extern const MIL_DecisionalReport eRC_RecuperationMaterielPreteEffectuee;
    extern const MIL_DecisionalReport eRC_RecuperationMaterielPreteImpossible;
    extern const MIL_DecisionalReport eRC_RecuperationMaterielPretePartiellementEffectuee;
    extern const MIL_DecisionalReport eRC_RedditionAnnulee;
    extern const MIL_DecisionalReport eRC_RegimeMaintenanceDelaiDepasse;
    extern const MIL_DecisionalReport eRC_Rendu;
    extern const MIL_DecisionalReport eRC_RepairEvacuationNoMeans;
    extern const MIL_DecisionalReport eRC_SectionDeployee;
    extern const MIL_DecisionalReport eRC_SectionUndeployed;
    extern const MIL_DecisionalReport eRC_SeuilLogistiqueDotationDepasse;
    extern const MIL_DecisionalReport eRC_SeuilLogistiqueStockDepasse;
    extern const MIL_DecisionalReport eRC_StartDeploy;
    extern const MIL_DecisionalReport eRC_StartUndeploy;
    extern const MIL_DecisionalReport eRC_SupplierUnavailable;
    extern const MIL_DecisionalReport eRC_SurrenderedUnitIdentified;
    extern const MIL_DecisionalReport eRC_SurrenderedUnitRecognized;
    extern const MIL_DecisionalReport eRC_TacticallyDestroyed;
    extern const MIL_DecisionalReport eRC_TerrainDifficile;
    extern const MIL_DecisionalReport eRC_TirDansZoneInterdite;
    extern const MIL_DecisionalReport eRC_TireParCampAmi;
    extern const MIL_DecisionalReport eRC_TireParCampEnnemi;
    extern const MIL_DecisionalReport eRC_TireParCampNeutre;
    extern const MIL_DecisionalReport eRC_TireParCivil;
    extern const MIL_DecisionalReport eRC_TirIndirectFratricide;
    extern const MIL_DecisionalReport eRC_TirIndirectSurCible;
    extern const MIL_DecisionalReport eRC_TirIndirectSurPopulation;
    extern const MIL_DecisionalReport eRC_TirSurCampAmi;
    extern const MIL_DecisionalReport eRC_TirSurCampEnnemi;
    extern const MIL_DecisionalReport eRC_TirSurCampNeutre;
    extern const MIL_DecisionalReport eRC_TirSurCivil;
    extern const MIL_DecisionalReport eRC_TransportedUnitCannotReceiveOrder;
    extern const MIL_DecisionalReport eRC_UrbanCollisionStarted;
    extern const MIL_DecisionalReport eRC_UrbanCollisionStopped;
    extern const MIL_DecisionalReport eRC_CrossedLima;
    extern const MIL_DecisionalReport eRC_IdentifiedUnitSurrendered;
    extern const MIL_DecisionalReport eRC_RecognizedUnitSurrendered;
    extern const MIL_DecisionalReport eRC_IEDHeard;
    extern const MIL_DecisionalReport eRC_InFireObject;
    extern const MIL_DecisionalReport eRC_IndirectFireObserved;
    extern const MIL_DecisionalReport eRC_BlockedByObject;
}

#endif // MIL_DecisionalReport
