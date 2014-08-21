// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
#include "MIL_DecisionalReport.h"

#define QUOTE_(X) # X
#define QUOTE(X) QUOTE_(X)

#define MAKE_REPORT(X) const MIL_DecisionalReport X (QUOTE(X));

namespace report
{
    MAKE_REPORT( eRC_AlerteDisponibiliteMedecins )
    MAKE_REPORT( eRC_AlerteDisponibiliteMoyensRamassage )
    MAKE_REPORT( eRC_AlerteDisponibiliteMoyensReleve )
    MAKE_REPORT( eRC_AlerteDisponibiliteRemorqueurs )
    MAKE_REPORT( eRC_AlerteDisponibiliteReparateurs )
    MAKE_REPORT( eRC_AlerteDisponibiliteVecteurs )
    MAKE_REPORT( eRC_AllocationConsentieBientotEpuisee )
    MAKE_REPORT( eRC_ANouveauDisponibleApresReparation )
    MAKE_REPORT( eRC_ArmedCiviliansDetected )
    MAKE_REPORT( eRC_AttentatTerroristeDansPopulation )
    MAKE_REPORT( eRC_AttitudePopulation )
    MAKE_REPORT( eRC_Bloquee )
    MAKE_REPORT( eRC_BorderCrossed )
    MAKE_REPORT( eRC_CiviliansEncountered )
    MAKE_REPORT( eRC_CloseCrowdAttitudeChanged )
    MAKE_REPORT( eRC_CloseCrowdDemonstration )
    MAKE_REPORT( eRC_CloseCrowdUrbanDestruction )
    MAKE_REPORT( eRC_Contamine )
    MAKE_REPORT( eRC_CorpseTransported )
    MAKE_REPORT( eRC_DamagesCausedByNeutralSide )
    MAKE_REPORT( eRC_DamagesCausedToNeutralSide )
    MAKE_REPORT( eRC_DebutInterventionFaceAPopulation )
    MAKE_REPORT( eRC_DebutNeutralisation )
    MAKE_REPORT( eRC_DecesBlesse )
    MAKE_REPORT( eRC_DecesBlessePendantHospitalisation )
    MAKE_REPORT( eRC_DecesBlessePendantTransport )
    MAKE_REPORT( eRC_DemandeEvacuationMateriel )
    MAKE_REPORT( eRC_DemandeEvacuationSanitaire )
    MAKE_REPORT( eRC_DemandeRavitaillementDotations )
    MAKE_REPORT( eRC_DemandeRavitaillementStocks )
    MAKE_REPORT( eRC_DepassementCapaciteStockage )
    MAKE_REPORT( eRC_DestructionPC )
    MAKE_REPORT( eRC_DetectedUnit  )
    MAKE_REPORT( eRC_DifficultMovementProgression )
    MAKE_REPORT( eRC_DisembarkmentFinished )
    MAKE_REPORT( eRC_DisembarkmentInterrupted )
    MAKE_REPORT( eRC_DisembarkmentStarted )
    MAKE_REPORT( eRC_EmbarkmentFinished )
    MAKE_REPORT( eRC_EmbarkmentInterrupted )
    MAKE_REPORT( eRC_EmbarkmentStarted )
    MAKE_REPORT( eRC_Empoisonne )
    MAKE_REPORT( eRC_EnemyUnitIdentified )
    MAKE_REPORT( eRC_EnemyUnitRecognized )
    MAKE_REPORT( eRC_ExplosionSurBouchonMine )
    MAKE_REPORT( eRC_Filtree )
    MAKE_REPORT( eRC_FinNeutralisation )
    MAKE_REPORT( eRC_FireObserver )
    MAKE_REPORT( eRC_FragOrderReceived )
    MAKE_REPORT( eRC_FriendUnitIdentified )
    MAKE_REPORT( eRC_FriendUnitRecognized )
    MAKE_REPORT( eRC_HumainRetourDeSante )
    MAKE_REPORT( eRC_InfrastructureDamaged )
    MAKE_REPORT( eRC_Jammed )
    MAKE_REPORT( eRC_LivingAreaDamaged )
    MAKE_REPORT( eRC_LogNoStock )
    MAKE_REPORT( eRC_LogNoSuperior )
    MAKE_REPORT( eRC_LogQuotaExceeded )
    MAKE_REPORT( eRC_LogQuotaExceededForAgent )
    MAKE_REPORT( eRC_LogSuperiorAdded )
    MAKE_REPORT( eRC_LogSuperiorRemoved )
    MAKE_REPORT( eRC_MaterielPrete )
    MAKE_REPORT( eRC_MaterielRendu )
    MAKE_REPORT( eRC_MaterielRepareSurPlace )
    MAKE_REPORT( eRC_MaterielRetourDeMaintenance )
    MAKE_REPORT( eRC_MilitaryEquipmentPlundered )
    MAKE_REPORT( eRC_MissionImpossible )
    MAKE_REPORT( eRC_MunitionAutorise )
    MAKE_REPORT( eRC_MunitionInterdite )
    MAKE_REPORT( eRC_NeutralUnitIdentified )
    MAKE_REPORT( eRC_NeutralUnitRecognized )
    MAKE_REPORT( eRC_NotActivatedUndergroundNetwork )
    MAKE_REPORT( eRC_ObservationTirIndirect )
    MAKE_REPORT( eRC_PlusDeCarburant )
    MAKE_REPORT( eRC_PopulationVictimeAffrontements )
    MAKE_REPORT( eRC_PopulationVictimeExplosionMines )
    MAKE_REPORT( eRC_PretMaterielAnnule )
    MAKE_REPORT( eRC_PretMaterielEffectue )
    MAKE_REPORT( eRC_PretMaterielEnCours )
    MAKE_REPORT( eRC_PretMaterielImpossible )
    MAKE_REPORT( eRC_PretMaterielPartiellementEffectue )
    MAKE_REPORT( eRC_PriseAPartieParPopulation )
    MAKE_REPORT( eRC_PrisonersCampFull )
    MAKE_REPORT( eRC_PrisonersUnsupplied )
    MAKE_REPORT( eRC_PrisSousTirArtillerie )
    MAKE_REPORT( eRC_PrisSousTirEclairant )
    MAKE_REPORT( eRC_PrisSousTirFumigene )
    MAKE_REPORT( eRC_PrisSousTirIED )
    MAKE_REPORT( eRC_ProtectionActiveHardKillReussie )
    MAKE_REPORT( eRC_ProtectionActiveReussie )
    MAKE_REPORT( eRC_Questionning )
    MAKE_REPORT( eRC_RavitaillementDotationsAnnule )
    MAKE_REPORT( eRC_RavitaillementDotationsEffectue )
    MAKE_REPORT( eRC_RavitaillementStockAnnule )
    MAKE_REPORT( eRC_RavitaillementStockEffectue )
    MAKE_REPORT( eRC_RecuperationMaterielPreteEffectuee )
    MAKE_REPORT( eRC_RecuperationMaterielPreteImpossible )
    MAKE_REPORT( eRC_RecuperationMaterielPretePartiellementEffectuee )
    MAKE_REPORT( eRC_RedditionAnnulee )
    MAKE_REPORT( eRC_RegimeMaintenanceDelaiDepasse )
    MAKE_REPORT( eRC_Rendu )
    MAKE_REPORT( eRC_RepairEvacuationNoMeans )
    MAKE_REPORT( eRC_SectionDeployee )
    MAKE_REPORT( eRC_SectionUndeployed )
    MAKE_REPORT( eRC_SeuilLogistiqueDotationDepasse )
    MAKE_REPORT( eRC_SeuilLogistiqueStockDepasse )
    MAKE_REPORT( eRC_StartDeploy )
    MAKE_REPORT( eRC_StartUndeploy )
    MAKE_REPORT( eRC_SupplierUnavailable )
    MAKE_REPORT( eRC_SurrenderedUnitIdentified )
    MAKE_REPORT( eRC_SurrenderedUnitRecognized )
    MAKE_REPORT( eRC_TacticallyDestroyed )
    MAKE_REPORT( eRC_TerrainDifficile )
    MAKE_REPORT( eRC_TirDansZoneInterdite )
    MAKE_REPORT( eRC_TireParCampAmi )
    MAKE_REPORT( eRC_TireParCampEnnemi )
    MAKE_REPORT( eRC_TireParCampNeutre )
    MAKE_REPORT( eRC_TireParCivil )
    MAKE_REPORT( eRC_TirIndirectFratricide )
    MAKE_REPORT( eRC_TirIndirectSurCible )
    MAKE_REPORT( eRC_TirIndirectSurPopulation )
    MAKE_REPORT( eRC_TirSurCampAmi )
    MAKE_REPORT( eRC_TirSurCampEnnemi )
    MAKE_REPORT( eRC_TirSurCampNeutre )
    MAKE_REPORT( eRC_TirSurCivil )
    MAKE_REPORT( eRC_TransportedUnitCannotReceiveOrder )
    MAKE_REPORT( eRC_UrbanCollisionStarted )
    MAKE_REPORT( eRC_UrbanCollisionStopped )
    MAKE_REPORT( eRC_CrossedLima )
    MAKE_REPORT( eRC_IdentifiedUnitSurrendered )
    MAKE_REPORT( eRC_RecognizedUnitSurrendered )
    MAKE_REPORT( eRC_IEDHeard )
    MAKE_REPORT( eRC_InFireObject )
}
