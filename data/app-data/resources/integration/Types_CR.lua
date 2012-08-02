-------------------------------------------------------------------------------
-- KnowledgeManager :
-- Manage query and destroy knowledge when task is finish
-- @author MGD
-- @created 2010-03-03
-- @modified MGD 2010-03-03
--
-- This file is part of a MASA library or program.
-- Refer to the included end-user license agreement for restrictions.
--
-- Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
-------------------------------------------------------------------------------

eRC_RAS = 0
eRC_EnPosteFaceAObstacle = 1
eRC_EnContournementObstacle = 2
eRC_MissionImpossible = 3
eRC_ProgressionSurAxe = 4
eRC_ProgressionVersPosition = 5
eRC_ArriveSurPosition = 6
eRC_ReconnaissancePoint = 7
eRC_EnPosteFaceADirectionDangereuse = 8
eRC_EnJalonnement = 9
eRC_PreparationMission = 10
eRC_DebutMission = 11
eRC_FinMission = 12
eRC_DispositifInstalle = 13
eRC_EnPosition = 14
eRC_ObjectifAtteint = 15
eRC_Debouche = 16
eRC_RejointUnite = 17
eRC_EscorteUnite = 18
eRC_EnFreinage = 19
eRC_Assaut = 20
eRC_EnCouverture = 21
eRC_EnEclairage = 22
eRC_EnManoeuvreRavitaillement = 23
eRC_SurPIA = 24
eRC_PassageSurPIA = 25
eRC_UniteRecueillie = 26
eRC_EnCoursDeFranchissement = 27
eRC_AttenteOrdrePoursuivre = 28
eRC_AttenteOrdreDeboucher = 29
eRC_Fixe = 30
eRC_EnEsquive = 31
eRC_PriseContact = 32
eRC_RuptureContact = 33
eRC_PrecisionContact = 34
eRC_EnnemiDetruit = 35
eRC_PrisAPartieParTirDirect = 36
eRC_PrisSousTirArtillerie = 37
eRC_PointTenuParEni = 38
eRC_ProgressionVersEni = 39
eRC_Decroche = 40
eRC_EniSurObjectif = 41
eRC_Riposte = 42
eRC_PretPourEngagementSurEnnemi = 43
eRC_DebutControleZone = 44
eRC_FinControleZone = 45
eRC_DebutSurveillance = 46
eRC_FinSurveillance = 47
eRC_EnInfiltration = 48
eRC_EnExfiltration = 49
eRC_AttenteConditionFavorablePourInfiltration = 50
eRC_DroneLance = 51
eRC_DroneEnExploitation = 52
eRC_DroneExploite = 53
eRC_DroneDisponible = 54
eRC_DebutMiseEnOeuvreDrone = 55
eRC_FinMiseEnOeuvreDrone = 56
eRC_ActivationBrouillage = 57
eRC_DesactivationBrouillage = 58
eRC_ActivationSystemeEcoute = 59
eRC_DesactivationSystemeEcoute = 60
eRC_ActivationSystemeEcouteRadar = 61
eRC_DesactivationSystemeEcouteRadar = 62
eRC_DebutLiaison = 63
eRC_FinLiaison = 64
eRC_EtablissementLiaisonImpossible = 65
eRC_UniteTropDistante = 66
eRC_EnregistrementDonnes = 67
eRC_ExploitationDonnees = 68
eRC_ExtractionDonnes = 69
eRC_Releve = 70
eRC_SuitUnite = 71
eRC_PasseEnAppui = 72
eRC_EnAppui = 73
eRC_NonOperationnel = 74
eRC_Operationnel = 75
eRC_AmbianceVitesse = 76
eRC_AmbianceSurete = 77
eRC_SurLima = 78
eRC_DebutTravaux = 79
eRC_FinTravaux = 80
eRC_ObstacleDegage = 81
eRC_OrganisationChantier = 82
eRC_DesorganisationChantier = 83
eRC_ReconnaissancePositionObstacle = 84
eRC_ContournementRealise = 85
eRC_AttenteRenforcement = 86
eRC_DebutExploitationSiteFranchissement = 87
eRC_FinExploitationSiteFranchissement = 88
eRC_ReussiteRenforcement = 89
eRC_EchecRenforcement = 90
eRC_ObstacleEnAttenteActivation = 91
eRC_DebutAmenagementBerges = 92
eRC_FinAmenagementBerges = 93
eRC_RegimeMaintenanceDelaiDepasse = 94
eRC_SectionDeployee = 95
eRC_ANouveauDisponibleApresReparation = 96
eRC_MaterielRetourDeMaintenance = 97
eRC_MaterielRepareSurPlace = 98
eRC_DemandeRavitaillementDotations = 99
eRC_DemandeRavitaillementStocks = 100
eRC_DemandeEvacuationSanitaire = 101
eRC_DemandeEvacuationMateriel = 102
eRC_AllocationConsentieBientotEpuisee = 103
eRC_DepassementCapaciteStockage = 104
eRC_SeuilLogistiqueDotationDepasse = 105
eRC_SeuilLogistiqueStockDepasse = 106
eRC_RavitaillementDotationsEffectue = 107
eRC_RavitaillementStockEffectue = 108
eRC_RavitaillementDotationsAnnule = 109
eRC_RavitaillementStockAnnule = 110
eRC_HumainRetourDeSante = 111
eRC_AlerteDisponibiliteMoyensReleve = 112
eRC_AlerteDisponibiliteMoyensRamassage = 113
eRC_AlerteDisponibiliteMedecins = 114
eRC_AlerteDisponibiliteRemorqueurs = 115
eRC_AlerteDisponibiliteReparateurs = 116
eRC_AlerteDisponibiliteVecteurs = 117
eRC_PretMaterielEnCours = 118
eRC_PretMaterielAnnule = 119
eRC_PretMaterielEffectue = 120
eRC_PretMaterielImpossible = 121
eRC_PretMaterielPartiellementEffectue = 122
eRC_RecuperationMaterielPreteImpossible = 123
eRC_RecuperationMaterielPreteEffectuee = 124
eRC_RecuperationMaterielPretePartiellementEffectuee = 125
eRC_MaterielPrete = 126
eRC_MaterielRendu = 127
eRC_Convoi_DeplacementVersPointChargement = 128
eRC_Convoi_ChargementEnCours = 129
eRC_Convoi_ChargementEffectue = 130
eRC_Convoi_DeplacementVersPointDechargement = 131
eRC_Convoi_DechargementEnCours = 132
eRC_Convoi_DechargementEffectue = 133
eRC_Convoi_Retour = 134
eRC_EnCoursRavitaillement = 135
eRC_RavitaillementTermine = 136
eRC_HelicoptereEnObservation = 137
eRC_PerteInfoGuidage = 138
eRC_TransportUnitePasPrete = 139
eRC_TransportUnitePrete = 140
eRC_TransportEnCours = 141
eRC_TransportEmbarquement = 142
eRC_TransportDebarquement = 143
eRC_TransportImpossiblePasDeMoyens = 144
eRC_VehiculeLaisseSurPosition = 145
eRC_RejointPositionEmbarquement = 146
eRC_EnAttenteTransporteurs = 147
eRC_ContamineNBC = 148
eRC_DecontamineNBC = 149
eRC_TenueProtectionNBCMise = 150
eRC_TenueProtectionNBCEnlevee = 151
eRC_AlerteNBCNiveau4 = 152
eRC_FinAlerteNBCNiveau4 = 153
eRC_SiteDecontaminationAnime = 154
eRC_SiteDecontaminationDesactive = 155
eRC_DemandeDecontamination = 156
eRC_ZoneReconnue = 157
eRC_ZoneDecontaminee = 158
eRC_SiteROTAReconnu = 159
eRC_IncidentNBCTermine = 160
eRC_UniteDecontaminee = 161
eRC_SiteSature = 162
eRC_DebutMesure = 163
eRC_FinMesure = 164
eRC_ReconnaissanceTerminee = 165
eRC_ReconnaissanceInitialeTerminee = 166
eRC_RejointPlotRavitaillement = 167
eRC_PremierCoupParti = 168
eRC_TirExecute = 169
eRC_TirHorsDePortee = 170
eRC_TirImpossiblePlusDeMunitions = 171
eRC_TirAnnule = 172
eRC_ChangementDePosition = 173
eRC_DebutInstallation = 174
eRC_FinInstallation = 175
eRC_PretPourConduiteTir = 176
eRC_ObservationTirIndirect = 177
eRC_ZoneImplantationOccupee = 178
eRC_InstallationSurZoneImplantation = 179
eRC_ReceptionOrdreDeFeu = 180
eRC_PretAuTir = 181
eRC_PresencePopulation = 182
eRC_OrientationPopulationVersCampRefugies = 183
eRC_PasDeCampRefugiesDisponible = 184
eRC_PresenceEnnemiRendu = 185
eRC_OrientationEnnemiRenduVersCampPrisonniers = 186
eRC_PasDeCampPrisonniersDisponible = 187
eRC_Rendu = 188
eRC_RedditionAnnulee = 189
eRC_TirDansZoneInterdite = 190
eRC_TirSurCampAmi = 191
eRC_TireParCampAmi = 192
eRC_TirSurCampNeutre = 193
eRC_TireParCampNeutre = 194
eRC_TirSurCivil = 195
eRC_TireParCivil = 196
eRC_TirIndirectFratricide = 197
eRC_TirIndirectSurPopulation = 198
eRC_DestructionPC = 199
eRC_DecesBlesse = 200
eRC_DecesBlessePendantTransport = 201
eRC_DecesBlessePendantHospitalisation = 202
eRC_MissionImpossibleUnitesSubordonneesNonOperationnelles = 203
eRC_MissionImpossibleLimaInvalide = 204
eRC_MissionImpossibleZoneHorsFuseau = 205
eRC_RapportDeForceDefavorable = 206
eRC_TerrainDifficile = 207
eRC_PlusDeCarburant = 208
eRC_DispositifDisperse = 209
eRC_DebutNeutralisation = 210
eRC_FinNeutralisation = 211
eRC_DebutSilenceRadio = 212
eRC_FinSilenceRadio = 213
eRC_DebutSilenceRadar = 214
eRC_FinSilenceRadar = 215
eRC_TirRestreint = 216
eRC_TirInterdit = 217
eRC_TirLibre = 218
eRC_Trace = 219
eRC_TraceID = 220
eRC_EmploiForceInterdit = 221
eRC_MaintienADistanceParMoyensNonLetaux = 222
eRC_ArmesLetalesAutorisees = 223
eRC_PopulationDetectee = 224
eRC_PriseAPartieParPopulation = 225
eRC_RiposteContrePopulation = 226
eRC_PopulationDangereuseAProximite = 227
eRC_EsquiveFaceAPopulation = 228
eRC_DecrochageFaceAPopulation = 229
eRC_MiseEnPlaceControleDeFoule = 230
eRC_MiseEnPlaceFiltrage = 231
eRC_AnimationBarrage = 232
eRC_AnimationCanalisation = 233
eRC_CollecteRenseignementsSurPopulations = 234
eRC_PopulationReconnue = 235
eRC_SituationDifficilementTenableFaceAPopulation = 236
eRC_UtilisationArmesNonLetalesFaceAPopulation = 237
eRC_UtilisationArmementsFaceAPopulation = 238
eRC_DebutInterventionFaceAPopulation = 239
eRC_PopulationVictimeAffrontements = 240
eRC_AttitudePopulation = 241
eRC_AggravationAttitude = 242
eRC_AffrontementAvecForcesEnnemies = 243
eRC_PopulationTemporairementControlee = 244
eRC_PopulationTemporairementRepoussee = 245
eRC_RepriseDesAffrontements = 246
eRC_AgressionDePopulation = 247
eRC_AgressionParPopulation = 248
eRC_RiposteContreAgressionParPopulation = 249
eRC_AttaqueInstallation = 250
eRC_EnStationnement = 251
eRC_Bloquee = 252
eRC_Filtree = 253
eRC_AttentatTerroristeDansPopulation = 254
eRC_PopulationVictimeExplosionMines = 255
eRC_ExecutionAttentat = 256
eRC_BM_DebutMission = 257
eRC_BM_FinMission = 258
eRC_BM_DebutPhase = 259
eRC_BM_FinPhase = 260
eRC_ActivationObstacleDeManoeuvre = 261
eRC_DebutAnimationObjet = 262
eRC_TirDirectSurCible = 263
eRC_FinAnimationObjet = 264
eRC_IllumineCible = 265
eRC_DebutDegagement = 266
eRC_FinDegagement = 267
eRC_TirDirectImpossible = 268
eRC_TirIndirectSurCible = 269
eRC_TirSansCapacite = 270
eRC_TirIndirectMunitionInvalide = 271
eRC_TirIndirectImpossible = 272
eRC_CibleNonPercue = 273
eRC_EnnemiTactiquementDetruit = 274
eRC_VisionRadarVerrouilleeSur = 275
eRC_DebutReconnaissanceBlocUrbain = 276
eRC_FinReconnaissanceBlocUrbain = 277
eRC_DebutReconnaissancePoint = 278
eRC_FinReconnaissancePoint = 279
eRC_FinTirDirectSurCible = 280
eRC_DebutTravauxObjet = 281
eRC_FinTravauxObjet = 282
eRC_FinTirIndirectSurCible = 283
eRC_DebutNeutralisationCible = 284
eRC_FinNeutralisationCible = 285
eRC_VisionVerrouilleeSur = 286
eRC_DebutConstructionObjet = 287
eRC_ObjetDejaOccupe = 288
eRC_DebutSilencePartielRadio = 289
eRC_DebutReconnaissanceZone = 290
eRC_FinReconnaissanceZone = 291
eRC_ConstructionObjetImpossible = 292
eRC_PasDotationConstructionObjet = 293
eRC_Embarquement = 294
eRC_Debarquement = 295
eRC_DebutFouilleBlocUrbain = 296
eRC_FinFouilleBlocUrbain = 297
eRC_DebutFouilleZone = 298
eRC_FinFouilleZone = 299
eRC_MunitionInterdite = 300
eRC_MunitionAutorise = 301
eRC_DebutBouclageZone = 302
eRC_FinBouclageZone = 303
eRC_DebutAppui = 304
eRC_FinAppui = 305
eRC_ObjetNonContournable = 306
eRC_ObjetNonSupprimable = 307
eRC_TirIndirectTargetNotIlluminated = 308
eRC_TirIndirectForbiddenAmmo = 309
eRC_MissionImpossibleReason = 310
eRC_CouvertureActive = 311
eRC_CouvertureDesactive = 312
eRC_DebutControleBlocUrbain = 313
eRC_FinControleBlocUrbain = 314
eRC_DebutControlPoint = 315
eRC_FinControlPoint = 316
eRC_TirIndirectNoCapacity = 317
eRC_DebutSilenceRadioPartiel = 318
eRC_EmbarkmentStarted = 319
eRC_EmbarkmentInterrupted = 320
eRC_EmbarkmentResumed = 321
eRC_EmbarkmentFinished = 322
eRC_DisembarkmentStarted = 323
eRC_DisembarkmentInterrupted = 324
eRC_DisembarkmentResumed = 325
eRC_DisembarkmentFinished = 326
eRC_ProtectionActiveReussie = 327
eRC_ProtectionActiveHardKillReussie = 328
eRC_Empoisonne = 329
eRC_Contamine = 330
eRC_TirSurCampEnnemi = 331
eRC_TireParCampEnnemi = 332
eRC_DroneDoitEtreMisEnOeuvre = 333
eRC_ReussiteRenforcementPion = 334
eRC_EchecRenforcementPion = 335
eRC_LeaveLima = 336
eRC_ArrivedOnExtinguishableObject = 337
eRC_ObjectExtinguished = 338
eRC_ArrivedOnAlertZone = 339
eRC_ZoneAlerted = 340
eRC_ArrivedOnGaragePosition = 341
eRC_ExtractFromStock_Arrived = 342
eRC_ExtractFromStock_Extracted = 343
eRC_SupplyStock_Supplied = 344
eRC_SupplyStock_Arrived = 345
eRC_SupplyStock_Distributed = 346
eRC_ZoneConfined = 347
eRC_ArrivedOnConfinedZone = 348
eRC_PasDeDroneDisponible = 349
eRC_FirstTourOfZoneConfinementDone = 350
eRC_ZoneConfinementInProgress = 351
eRC_ZoneEvacuated = 352
eRC_ArrivedOnEvacuateZone = 353
eRC_ZoneEvacuationInProgress = 354
eRC_DebutValorisation = 355
eRC_FinValorisation = 356
eRC_EchecRelever = 357
eRC_criticalIntelligence = 358
eRC_criticalIntelligenceNone = 359
eRC_FinTravauxBlocUrbain = 360
eRC_BUNonTraficable = 361
eRC_StartDeploy = 362
eRC_DebutFouillePoint = 363
eRC_FinFouillePoint = 364
eRC_NotDeployedIndirectFireImpossible = 365
eRC_AgressionImpossible = 366
eRC_NoLogisticSuperior = 367
eRC_CrossingSiteNotRecon = 368
eRC_InvalidObject = 369
eRC_TerroristCaptured = 370
eRC_MiseEnPlaceProducePoint = 371
eRC_ProduceImpossible = 372
eRC_FinProducePoint = 373
eRC_TimeInUndergroundNetwork = 374
eRC_BadUndergroundNetwork = 375
eRC_NotActivatedUndergroundNetwork = 376
eRC_ImpossibleAtteindreObjectif = 377
eRC_ExplosionSurBouchonMine = 378
eRC_PathfindWarning = 379
eRC_DotationIndisponible = 380
eRC_ObservationEffetTirIndirect = 381
eRC_DamagesCausedToNeutralSide = 382
eRC_DamagesCausedByNeutralSide = 383
eRc_InfrastructureDamaged = 384
eRc_LivingAreaDamaged = 385
eRc_BorderCrossed = 386
eRc_TacticallyDestroyed = 387
eRc_CiviliansEncountered = 388
eRc_PrisonersUnsupplied = 389
eRc_PrisonersCampFull = 390
eRC_NoPositionsToDestroyTargets = 391
eRC_NoPositionsToNeutralizeTargets = 392
eRC_NoPositionsToGetCoverFromEntities = 393
eRC_NoPositionsToObserveTargets = 394
eRC_NoPositionsToReachTargets = 395
eRC_TheTargetCannotBeReconnoitre = 396
eRC_TargetIsNotAccessible = 397
eRC_NoKeyPositionsFoundInArea = 398
eRC_Engaged = 399
eRC_Disengaged = 400
eRC_AlreadyEngaged = 401
eRC_AlreadyDisengaged = 402
eRC_NoPEInAutomat = 403
eRC_SectionUndeployed = 404
eRC_StartUndeploy = 405

eRC_NoPopulationInUrbanBlock = 406
eRC_ObjectiveAlerted = 407
eRC_ObjectiveIsNoLongerAlerted = 408
eRC_ObjectiveConfined = 409
eRC_ObjectiveIsNoLongerConfined = 410 
eRC_ObjectiveEvacuated = 411
eRC_ObjectiveIsNoLongerEvacuated = 412
eRC_StartingToProduceResource = 413
eRC_ResourceProductionStopped = 414
eRC_ResourceNodeProductionEnhanced = 415
eRC_ResourceNodeProductionDamaged = 416
eRC_ResourceNodeDisabled = 417
eRC_ResourceNodeEnabled = 418
eRC_NoResourceNodesWithinArea = 419
eRC_NoUrbanBlocksWithinArea = 420
eRC_NoCrowdsWithinArea = 421
eRC_VictimsEvacuated = 422
eRC_VictimsExtracted = 423
eRC_VictimsCannotBeEvacuated = 424
eRC_VictimsCannotBeExtracted = 425
eRC_StartingToReduceResourceProduction = 426
eRC_ResourceReductionStopped = 427
eRC_NoWoundedVictims = 428
eRC_ElementCannotBeLoaded = 429
eRC_ElementCannotBeUnloaded = 430
eRC_ElementLoaded = 431
eRC_ElementUnloaded = 432
eRC_DifficultMovementProgression = 433
eRC_ImpossibleToExtinguishFire = 434
eRC_NoFiresWithinArea = 435
eRC_StartingToDecontaminate = 436
eRC_DecontaminationDone = 437
eRC_StartingToHealCrowd = 438
eRC_WoundedPersonsHealed = 439
eRC_StartingToExtractVictimsFromCrowd = 440
eRC_WoundedCannotBeHealed = 441
eRC_ControlPointEstablished = 442
eRC_SystemIsBeingOperated = 443
eRC_LimaParameterNotPresent = 444
eRC_EntityCannotFollow = 445
eRC_EntityHasBeenOrderedToFollow = 446
eRC_StartingToCreateResourceLink = 447
eRC_ResouceLinkCreated = 448
eRC_CannotDismount = 449
eRC_DecrochageFaceAIncendie = 450
eRC_Capture = 451
eRC_TakeHostage = 452
eRC_AttenteOrdrePoursuivreConvoi = 453
eRC_DebutDevalorisation = 454
eRC_FinDevalorisation = 455
eRC_TrafficablePositionFindingInZone = 456
eRC_PrisSousTirMunition = 457
eRC_UrbanCollisionStarted = 458
eRC_UrbanCollisionStopped = 459
eRC_NotInSameUndergroundNetwork = 460
eRC_FireIsBeingExtinguished = 461
eRC_TerroristDropped = 462
eRC_ImpossibleToOperateObject = 463
eRC_Questionning = 464
eRC_criticalIntelligenceImpossible = 465
eRC_InvalidTarget = 466
eRC_FragOrderReceived = 467
eRC_FurtifModeOn = 468
eRC_FurtifModeOff = 469
eRC_ObstacleNonActivable = 470
eRC_FireObserver = 471
eRC_ImpossibleActivateIED = 472
eRC_Jammed = 473
eRC_PCJammed = 474
eRC_IEDNotReady = 475
eRC_IEDReadyForFiring = 476
eRC_FollowingUnitIsDead = 477
eRC_CombatSupportUnitIsDead = 478
eRC_NoUnitsInParameter = 479
eRC_CaptureEnd = 480
eRC_PrisSousTirEclairant = 481
eRC_PrisSousTirFumigene = 482
eNbr = 483
