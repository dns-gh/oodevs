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

#include <string>

// =============================================================================
/** @class  MIL_DecisionalReport
    @brief  MIL_DecisionalReport
*/
// Created: SLI 2013-01-08
// =============================================================================
struct MIL_DecisionalReport
{
    //! @name Constructor/Destructors
    //@{
    inline MIL_DecisionalReport( const std::string& key ) : key_( key ){}
    //@}

    //! @name Operators
    //@{
    inline operator const std::string&() const{ return key_; }
    //@}
private:
    //! @name Member data
    //@{
    std::string key_;
    //@}
};

namespace report
{
    const MIL_DecisionalReport eRC_MissionImpossible( "eRC_MissionImpossible" );
    const MIL_DecisionalReport eRC_PrisSousTirArtillerie( "eRC_PrisSousTirArtillerie" );
    const MIL_DecisionalReport eRC_RegimeMaintenanceDelaiDepasse( "eRC_RegimeMaintenanceDelaiDepasse" );
    const MIL_DecisionalReport eRC_SectionDeployee( "eRC_SectionDeployee" );
    const MIL_DecisionalReport eRC_ANouveauDisponibleApresReparation( "eRC_ANouveauDisponibleApresReparation" );
    const MIL_DecisionalReport eRC_MaterielRetourDeMaintenance( "eRC_MaterielRetourDeMaintenance" );
    const MIL_DecisionalReport eRC_MaterielRepareSurPlace( "eRC_MaterielRepareSurPlace" );
    const MIL_DecisionalReport eRC_DemandeRavitaillementDotations( "eRC_DemandeRavitaillementDotations" );
    const MIL_DecisionalReport eRC_DemandeRavitaillementStocks( "eRC_DemandeRavitaillementStocks" );
    const MIL_DecisionalReport eRC_DemandeEvacuationSanitaire( "eRC_DemandeEvacuationSanitaire" );
    const MIL_DecisionalReport eRC_DemandeEvacuationMateriel( "eRC_DemandeEvacuationMateriel" );
    const MIL_DecisionalReport eRC_AllocationConsentieBientotEpuisee( "eRC_AllocationConsentieBientotEpuisee" );
    const MIL_DecisionalReport eRC_DepassementCapaciteStockage( "eRC_DepassementCapaciteStockage" );
    const MIL_DecisionalReport eRC_SeuilLogistiqueDotationDepasse( "eRC_SeuilLogistiqueDotationDepasse" );
    const MIL_DecisionalReport eRC_SeuilLogistiqueStockDepasse( "eRC_SeuilLogistiqueStockDepasse" );
    const MIL_DecisionalReport eRC_RavitaillementDotationsEffectue( "eRC_RavitaillementDotationsEffectue" );
    const MIL_DecisionalReport eRC_RavitaillementStockEffectue( "eRC_RavitaillementStockEffectue" );
    const MIL_DecisionalReport eRC_RavitaillementDotationsAnnule( "eRC_RavitaillementDotationsAnnule" );
    const MIL_DecisionalReport eRC_RavitaillementStockAnnule( "eRC_RavitaillementStockAnnule" );
    const MIL_DecisionalReport eRC_HumainRetourDeSante( "eRC_HumainRetourDeSante" );
    const MIL_DecisionalReport eRC_AlerteDisponibiliteMoyensReleve( "eRC_AlerteDisponibiliteMoyensReleve" );
    const MIL_DecisionalReport eRC_AlerteDisponibiliteMoyensRamassage( "eRC_AlerteDisponibiliteMoyensRamassage" );
    const MIL_DecisionalReport eRC_AlerteDisponibiliteMedecins( "eRC_AlerteDisponibiliteMedecins" );
    const MIL_DecisionalReport eRC_AlerteDisponibiliteRemorqueurs( "eRC_AlerteDisponibiliteRemorqueurs" );
    const MIL_DecisionalReport eRC_AlerteDisponibiliteReparateurs( "eRC_AlerteDisponibiliteReparateurs" );
    const MIL_DecisionalReport eRC_AlerteDisponibiliteVecteurs( "eRC_AlerteDisponibiliteVecteurs" );
    const MIL_DecisionalReport eRC_PretMaterielEnCours( "eRC_PretMaterielEnCours" );
    const MIL_DecisionalReport eRC_PretMaterielAnnule( "eRC_PretMaterielAnnule" );
    const MIL_DecisionalReport eRC_PretMaterielEffectue( "eRC_PretMaterielEffectue" );
    const MIL_DecisionalReport eRC_PretMaterielImpossible( "eRC_PretMaterielImpossible" );
    const MIL_DecisionalReport eRC_PretMaterielPartiellementEffectue( "eRC_PretMaterielPartiellementEffectue" );
    const MIL_DecisionalReport eRC_RecuperationMaterielPreteImpossible( "eRC_RecuperationMaterielPreteImpossible" );
    const MIL_DecisionalReport eRC_RecuperationMaterielPreteEffectuee( "eRC_RecuperationMaterielPreteEffectuee" );
    const MIL_DecisionalReport eRC_RecuperationMaterielPretePartiellementEffectuee( "eRC_RecuperationMaterielPretePartiellementEffectuee" );
    const MIL_DecisionalReport eRC_MaterielPrete( "eRC_MaterielPrete" );
    const MIL_DecisionalReport eRC_MaterielRendu( "eRC_MaterielRendu" );
    const MIL_DecisionalReport eRC_ObservationTirIndirect( "eRC_ObservationTirIndirect" );
    const MIL_DecisionalReport eRC_Rendu( "eRC_Rendu" );
    const MIL_DecisionalReport eRC_RedditionAnnulee( "eRC_RedditionAnnulee" );
    const MIL_DecisionalReport eRC_TirDansZoneInterdite( "eRC_TirDansZoneInterdite" );
    const MIL_DecisionalReport eRC_TirSurCampAmi( "eRC_TirSurCampAmi" );
    const MIL_DecisionalReport eRC_TireParCampAmi( "eRC_TireParCampAmi" );
    const MIL_DecisionalReport eRC_TirSurCampNeutre( "eRC_TirSurCampNeutre" );
    const MIL_DecisionalReport eRC_TireParCampNeutre( "eRC_TireParCampNeutre" );
    const MIL_DecisionalReport eRC_TirSurCivil( "eRC_TirSurCivil" );
    const MIL_DecisionalReport eRC_TireParCivil( "eRC_TireParCivil" );
    const MIL_DecisionalReport eRC_TirIndirectFratricide( "eRC_TirIndirectFratricide" );
    const MIL_DecisionalReport eRC_TirIndirectSurPopulation( "eRC_TirIndirectSurPopulation" );
    const MIL_DecisionalReport eRC_DestructionPC( "eRC_DestructionPC" );
    const MIL_DecisionalReport eRC_DecesBlesse( "eRC_DecesBlesse" );
    const MIL_DecisionalReport eRC_DecesBlessePendantTransport( "eRC_DecesBlessePendantTransport" );
    const MIL_DecisionalReport eRC_DecesBlessePendantHospitalisation( "eRC_DecesBlessePendantHospitalisation" );
    const MIL_DecisionalReport eRC_TerrainDifficile( "eRC_TerrainDifficile" );
    const MIL_DecisionalReport eRC_PlusDeCarburant( "eRC_PlusDeCarburant" );
    const MIL_DecisionalReport eRC_DebutNeutralisation( "eRC_DebutNeutralisation" );
    const MIL_DecisionalReport eRC_FinNeutralisation( "eRC_FinNeutralisation" );
    const MIL_DecisionalReport eRC_PriseAPartieParPopulation( "eRC_PriseAPartieParPopulation" );
    const MIL_DecisionalReport eRC_DebutInterventionFaceAPopulation( "eRC_DebutInterventionFaceAPopulation" );
    const MIL_DecisionalReport eRC_PopulationVictimeAffrontements( "eRC_PopulationVictimeAffrontements" );
    const MIL_DecisionalReport eRC_AttitudePopulation( "eRC_AttitudePopulation" );
    const MIL_DecisionalReport eRC_Bloquee( "eRC_Bloquee" );
    const MIL_DecisionalReport eRC_Filtree( "eRC_Filtree" );
    const MIL_DecisionalReport eRC_AttentatTerroristeDansPopulation( "eRC_AttentatTerroristeDansPopulation" );
    const MIL_DecisionalReport eRC_PopulationVictimeExplosionMines( "eRC_PopulationVictimeExplosionMines" );
    const MIL_DecisionalReport eRC_TirIndirectSurCible( "eRC_TirIndirectSurCible" );
    const MIL_DecisionalReport eRC_MunitionInterdite( "eRC_MunitionInterdite" );
    const MIL_DecisionalReport eRC_MunitionAutorise( "eRC_MunitionAutorise" );
    const MIL_DecisionalReport eRC_EmbarkmentStarted( "eRC_EmbarkmentStarted" );
    const MIL_DecisionalReport eRC_EmbarkmentInterrupted( "eRC_EmbarkmentInterrupted" );
    const MIL_DecisionalReport eRC_EmbarkmentFinished( "eRC_EmbarkmentFinished" );
    const MIL_DecisionalReport eRC_DisembarkmentStarted( "eRC_DisembarkmentStarted" );
    const MIL_DecisionalReport eRC_DisembarkmentInterrupted( "eRC_DisembarkmentInterrupted" );
    const MIL_DecisionalReport eRC_DisembarkmentFinished( "eRC_DisembarkmentFinished" );
    const MIL_DecisionalReport eRC_ProtectionActiveReussie( "eRC_ProtectionActiveReussie" );
    const MIL_DecisionalReport eRC_ProtectionActiveHardKillReussie( "eRC_ProtectionActiveHardKillReussie" );
    const MIL_DecisionalReport eRC_Empoisonne( "eRC_Empoisonne" );
    const MIL_DecisionalReport eRC_Contamine( "eRC_Contamine" );
    const MIL_DecisionalReport eRC_TirSurCampEnnemi( "eRC_TirSurCampEnnemi" );
    const MIL_DecisionalReport eRC_TireParCampEnnemi( "eRC_TireParCampEnnemi" );
    const MIL_DecisionalReport eRC_StartDeploy( "eRC_StartDeploy" );
    const MIL_DecisionalReport eRC_NotActivatedUndergroundNetwork( "eRC_NotActivatedUndergroundNetwork" );
    const MIL_DecisionalReport eRC_ExplosionSurBouchonMine( "eRC_ExplosionSurBouchonMine" );
    const MIL_DecisionalReport eRC_DamagesCausedToNeutralSide( "eRC_DamagesCausedToNeutralSide" );
    const MIL_DecisionalReport eRC_DamagesCausedByNeutralSide( "eRC_DamagesCausedByNeutralSide" );
    const MIL_DecisionalReport eRC_InfrastructureDamaged( "eRC_InfrastructureDamaged" );
    const MIL_DecisionalReport eRC_LivingAreaDamaged( "eRC_LivingAreaDamaged" );
    const MIL_DecisionalReport eRC_BorderCrossed( "eRC_BorderCrossed" );
    const MIL_DecisionalReport eRC_TacticallyDestroyed( "eRC_TacticallyDestroyed" );
    const MIL_DecisionalReport eRC_CiviliansEncountered( "eRC_CiviliansEncountered" );
    const MIL_DecisionalReport eRC_PrisonersUnsupplied( "eRC_PrisonersUnsupplied" );
    const MIL_DecisionalReport eRC_PrisonersCampFull( "eRC_PrisonersCampFull" );
    const MIL_DecisionalReport eRC_SectionUndeployed( "eRC_SectionUndeployed" );
    const MIL_DecisionalReport eRC_StartUndeploy( "eRC_StartUndeploy" );
    const MIL_DecisionalReport eRC_DifficultMovementProgression( "eRC_DifficultMovementProgression" );
    const MIL_DecisionalReport eRC_UrbanCollisionStarted( "eRC_UrbanCollisionStarted" );
    const MIL_DecisionalReport eRC_UrbanCollisionStopped( "eRC_UrbanCollisionStopped" );
    const MIL_DecisionalReport eRC_PrisSousTirIED( "eRC_PrisSousTirIED" );
    const MIL_DecisionalReport eRC_Questionning( "eRC_Questionning" );
    const MIL_DecisionalReport eRC_FragOrderReceived( "eRC_FragOrderReceived" );
    const MIL_DecisionalReport eRC_FireObserver( "eRC_FireObserver" );
    const MIL_DecisionalReport eRC_PrisSousTirEclairant( "eRC_PrisSousTirEclairant" );
    const MIL_DecisionalReport eRC_PrisSousTirFumigene( "eRC_PrisSousTirFumigene" );
    const MIL_DecisionalReport eRC_RepairEvacuationNoMeans( "eRC_RepairEvacuationNoMeans" );
    const MIL_DecisionalReport eRC_SupplierUnavailable( "eRC_SupplierUnavailable" );
    const MIL_DecisionalReport eRC_LogQuotaExceeded( "eRC_LogQuotaExceeded" );
    const MIL_DecisionalReport eRC_LogQuotaExceededForAgent( "eRC_LogQuotaExceededForAgent" );
    const MIL_DecisionalReport eRC_TransportedUnitCannotReceiveOrder( "eRC_TransportedUnitCannotReceiveOrder" );
    const MIL_DecisionalReport eRC_LogNoSuperior( "eRC_LogNoSuperior" );
    const MIL_DecisionalReport eRC_LogNoStock( "eRC_LogNoStock" );
    const MIL_DecisionalReport eRC_CloseCrowdAttitudeChanged( "eRC_CloseCrowdAttitudeChanged" );
    const MIL_DecisionalReport eRC_CloseCrowdUrbanDestruction( "eRC_CloseCrowdUrbanDestruction" );
    const MIL_DecisionalReport eRC_LogSuperiorAdded( "eRC_LogSuperiorAdded" );
    const MIL_DecisionalReport eRC_LogSuperiorRemoved( "eRC_LogSuperiorRemoved" );
    const MIL_DecisionalReport eRC_CloseCrowdDemonstration( "eRC_CloseCrowdDemonstration" );
    const MIL_DecisionalReport eRC_CorpseTransported( "eRC_CorpseTransported" );
}

#endif // MIL_DecisionalReport
