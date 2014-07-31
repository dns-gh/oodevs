-------------------------------------------------------------------------------
-- Global Types : 
-- @author MGD
-- @created 2010-06-24
--
-- This file is part of a MASA library or program.
-- Refer to the included end-user license agreement for restrictions.
--
-- Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
-------------------------------------------------------------------------------

-- Echelons
eEtatEchelon_None = 0
eEtatEchelon_Second = 1
eEtatEchelon_First = 2
eEtatEchelon_Scout = 3
eEtatEchelon_Reserve = 4

-- Contexte de la mission
eContexte_Offensif = 0 
eContexte_Retrograde = 1
eContexte_Static = 2

eEtatDestruction_None = 0
eEtatDestruction_Tactique = 1
eEtatDestruction_Total = 2

-- Efficiencies

ePionEfficiencyRecon = 0
ePionEfficiencyCombatSupport = 1
ePionEfficiencyCombat = 2
ePionEfficiencyMobilitySupport = 3
ePionEfficiencyCounterMobilitySupport = 4
ePionEfficiencyProtectionSupport = 5
ePionEfficiencyEngineeringSupport = 6
ePionEfficiencyUrbanArea = 7
ePionEfficiencyCommon = 8 -- valeur par défaut pour les mission qui n'ont pas besoin d'un pion en particulier

eTypePointNormal = 1
eTypePointObservation = 2
eTypePointParticulier = 4
eTypePointBond = 8
eTypePointLima = 16
eTypePointSonde = 32
eTypePointCCT = 64

-- Attitudes des foules
eAttitudeCalme     = 0
eAttitudeAgitee    = 1
eAttitudeExcitee   = 2
eAttitudeAgressive = 3



-- Limas
eTypeLima_LD = 0
eTypeLima_LCA = 1
eTypeLima_LC = 2
eTypeLima_LI = 3
eTypeLima_LO = 4
eTypeLima_LCAR = 5
eTypeLima_LR = 6
eTypeLima_LDM = 7
eTypeLima_LFM = 8
eTypeLima_LIA = 9

-- objets : type ADN
eTypeObjectMines = "mines"
eTypeObjectLinearMinedArea = "mined area (linear)"
eTypeObjectScatteredMinedArea = "mined area (scattered)"
eTypeObjectAntiTankObstacle = "anti tank obstacle"
eTypeObjectAbatis = "abattis"
eTypeObjectBarricade = "barricade"
eTypeObjectLandslide = "landslide"
eTypeObjectRoadDestruction = "road destruction"
eTypeObjectBridgeDestruction = "bridge destruction"
eTypeObjectContinuousPontoonBridge = "continuous pontoon bridge"
eTypeObjectFiringPost = "firing post"
eTypeObjectSafeZone = "safe zone"
eTypeObjectGunArtilleryDeploymentArea = "gun artillery deployment area"
eTypeObjectCOBRADeploymentArea = "cobra deployment area"
eTypeObjectMrlsDeploymentArea = "mrls deployment area"
eTypeObjectCrossingSite = "crossing site"
eTypeObjectNBCCloud = "nbc cloud"
eTypeObjectLogisticSupportArea = "logistic support area"
eTypeObjectSiteDecontamination = "decontamination site"
eTypeObjectRota = "rota area"
eTypeObjectNBCZone = "nbc zone"
eTypeObjectJammingArea = "jamming area"
eTypeObjectLandingArea = "landing area"
eTypeObjectTrail = "trail"
eTypeObjectPlatform = "platform"
eTypeObjectMobilityEnhancedArea = "mobility enhanced area"
eTypeObjectLandingAreaHelicopter = "landing area (helicopter)"
eTypeObjectLogisticArea = "logistic area"
eTypeObjectPrisonersCamp = "prisoners camp"
eTypeObjectRefugeesCamp = "refugees camp"
eTypeObjectCheckpoint = "checkpoint"
eTypeObjectDroppingArea = "dropping area"
eTypeObjectMovementForbiddenArea = "movement forbidden area"
eTypeObjectFiringForbiddenArea = "firing forbidden area"
eTypeObjectMortarDeploymentArea = "mortar deployment area"
eTypeObjectDiscontinuousPontoonBridge = "discontinuous pontoon bridge"
eTypeObjectInstallation = "installation"
eTypeObjectUrbanBlock = "urban block"
eTypeObjectSealOffArea = "seal off area"
eTypeObjectDynamicFire = "fire"
eTypeObjectPopulationMoving = "population moving"
eTypeObjectMetroStation = "metro station"
eTypeObjectRessourceNetworkElement = "ressource network element"
eTypeObjectSewerEntry = "sewer entry"
eTypeObjectMotorway = "motorway"
eTypeObjectSmallRoad = "small road"
eTypeObjectMediumRoad = "medium road"
eTypeObjectLargeRoad = "large road"
eTypeObjectWetTerrain = "wet terrain"
eTypeObjectSnowedTerrain = "snowed terrain"
eTypeObjectIcedTerrain = "iced terrain"
eTypeObjectBorder = "border"
eTypeObjectEmergencyShelter = "emergency shelter"
eTypeObjectItineraireLogistique = "logistic route"
eTypeObjectFlood = "flood"
eTypeObjectUndergroundExit = "underground exit"
eTypeObjectFullJammingArea = "full jamming area"
eTypeObjectNBCZoneExternal = "nbc zone (external model)"
eTypeObjectNBCCLoudExternal = "nbc cloud (external model)"
eTypeObjectSurfaceFire = "surface fire"
eTypeObjectFireForbiddenArea = "fire forbidden area"
eTypeObjectDistributionPoint = "distribution point"
eTypeObjectSensorsDetectionZone = "sensorsDetectionZone"
eTypeObjectSensors = "sensors"
eTypeObjectSupplyFacility = "supply point"
eTypeObjectDirtyBomb = "dirty bomb"
eTypeObjectHospital = "hospital"
eTypeObjectWeaponCache = "weapon cache"
eTypeObjectLeaflet = "leaflet"
eTypeObjectEmergencyMeetingPlace = "emergency meeting place"
eTypeObjectUrbanStock = "stock"
eTypeObjectFirebreakArea = "firebreak area"
eTypeObjectMinedAreaLinearDestructible = "mined area (linear and destructible)"
eTypeObjectDyke = "dyke"
eTypeObjectCountryRoad = "country road"
eTypeObjectHighway = "highway"
eTypeObjectMainRoad = "main road"
eTypeObjectSecondaryRoad = "secondary road"

eForceRatioStateNone = 0
eForceRatioStateNeutral = 1
eForceRatioStateFavorable = 2
eForceRatioStateUnfavorable = 3

eEtatDec_RAS = 0
eEtatDec_Continu = 1
eEtatDec_Sauvegarde = 2

eAttitudePopulation_Calme = 0
eAttitudePopulation_Agitee = 1
eAttitudePopulation_Excitee = 2
eAttitudePopulation_Agressive = 3

-- ROE states
eRoeStateFreeFire = 0
eRoeStateRestrictedFire = 1
eRoeStateFireByOrder = 2

-- ROE crowd states
eEtatROEPopulation_EmploiForceInterdit = 1
eEtatROEPopulation_MaintienADistanceParMoyensNonLetaux = 2
eEtatROEPopulation_ArmesLetalesAutorisees = 3

-- Il faut 45 min pour passer de 0 à 100%
g_NaturalEvolutionDomination = 0.0027 / 0.75

eTypeItiMouvement = 0
eTypeItiReconnaissance = 1
eTypeItiInfiltration = 2
eTypeItiAttaque = 3
eTypeItiRepli = 4
eTypeItiAppui = 5
eTypeItiDeminage = 6
eTypeItiLogistique = 7
eTypeItiCriminal = 8
eTypeItiNBC = 9

eEtatActionDeplacement_EnCours = 0
eEtatActionDeplacement_Termine = 1
eEtatActionDeplacement_Pause = 2
eEtatActionDeplacement_ManqueCarburant = 3
eEtatActionDeplacement_NonAutorise = 4
eEtatActionDeplacement_DejaEnDeplacement = 5
eEtatActionDeplacement_ItineraireDoitEtreRejoint = 6
eEtatActionDeplacement_CheminPartiel = 7
eEtatActionDeplacement_BloqueParObjet = 8
eEtatActionDeplacement_Teleporte = 9

--- IndirectFireState
eIndirectFireState_Running = 0
eIndirectFireState_NoCapacity = 1
eIndirectFireState_Finished = 2
eIndirectFireState_Impossible = 3
eIndirectFireState_NoAmmo = 4

--- IndirectFire disponibility
eFireAvailability_PretAuTir = 1
eFireAvailability_Indisponible = 2

--Transport
eActionTransport_Running = 0
eActionTransport_Finished = 1
eActionTransport_Impossible = 2

-- safeMode enumeration
eEtatAmbiance_Surete = 0
eEtatAmbiance_Vitesse = 1

-- Asymetric integration
eTirDirectNormal = 0
eTirDirectLibre = 1

eActionTirDirect_Impossible = 0
eActionTirDirect_EnemyDestroyed = 1
eActionTirDirect_NoCapacity = 2
eActionTirDirect_NoAmmo = 3
eActionTirDirect_Running = 4
eActionTirDirect_Finished = 5
eActionTirDirect_TemporarilyBlocked = 6
eActionTirDirect_None = 7

eFragOrderIsIncorrect = 0
eFragOrderIsUnknown = 1
eFragOrderIsIndirectFire = 2

-- Actions Objets
eActionObjetEnCours = 0
eActionObjetTerminee = 1
eActionObjetImpossible = 2
eActionObjetManqueDotation = 3
eActionObjetPasDeCapacite = 4

-- Actions
eActionRunning = 0
eActionFinished = 1
eActionNotAllowed = 2

-- Reseaux souterrains
eUndergroundNetworkValid = 0
eUndergroundNetworkInvalid = 1

-- Temps d'attente (min)
eTempsReportInfo = 2

-- Logistique
eConvoyerAction_None = 0
eConvoyerAction_Setup = 1
eConvoyerAction_Load = 2
eConvoyerAction_Unload = 3
eConvoyerAction_MoveToSupplier = 4
eConvoyerAction_MoveToSupplyRecipient = 5
eConvoyerAction_MoveToTransportersProvider = 6

-- ASA
eEtatDeploiement_omnidirectionnel = 0
eEtatDeploiement_directionnel = 1

-- Missile
eMunitionClasse_Obus = 0
eMunitionClasse_MissileSol = 1
eMunitionClasse_MissileAir = 2
eMunitionClasse_Mitraille = 3

-- MISTRAL
M_MODELE_VITESSE_MISSILE_MISTRAL = 700
MISTRAL_ANGLE_OUVERTURE_DIRECTIONEL = 70

-- Mode de deploiement
eDeploiementEn_carre = 0
eDeploiementEn_triangle = 1
eDeploiementNasse_trois_sections = 2
eDeploiementNasse_quatre_sections = 3
eDeploiementDouble_rideau = 4
eDeploiementSimple_rideau = 5

M_POLY_DOCTRINE_DISTANCE_MAX_ENTRE_SECTIONS = 7000
M_POLY_DOCTRINE_DISTANCE_POINT_A_DEFENDRE = 2000
M_POLY_DOCTRINE_DISTANCE_ENTRE_RIDEAUX = 5000


-- Radar type enumeration
eRadarType_Radar = 0
eRadarType_Ecoute = 1
eRadarType_EcouteRadar = 2

eEtatRadar_Silence = 0
eEtatRadar_Ouvert = 1

-- Niveau d'installation
eNiveauInstallation_Mouvement = 0
eNiveauInstallation_MouvementDiscret = 1
eNiveauInstallation_Arret = 2
eNiveauInstallation_PosteReflexe = 3
eNiveauInstallation_Poste = 4
eNiveauInstallation_PosteAmenage = 5
eNiveauInstallation_PostePrepareGenie = 6

-- Dotations tir indirect
eObus_Explosif = 0
eObus_Grenade = 1
eObus_ACED = 2
eObus_Fumigenes = 3
eObus_Eclairant = 4
eObus_Mine = 5
eObus_effect = 6

-- Random style
eRandomCurious = 0
eRandomFlee = 1
eRandomStay = 2

-- Ambiance
eAmbianceMission_Safety = 0
eAmbianceMission_Speed = 1
eAmbianceMission_None = 2

-- firing type enumeration
eDirectFires = 0
eIndirectFires = 1

-- Dotation type
eDotation_Ammunition = 0
eDotation_Fuel = 1
eDotation_Explosives = 2
eDotation_Mine = 3
eDotation_BarbedWire = 4
eDotation_Piece = 5
eDotation_Ration = 6
eNbrDotationTypes = 7

-- Aviation range
eAviationRange_ShortRange = 0
eAviationRange_MediumRange = 1
eAviationRange_LongRange = 2
eAviationRange_NoAmmunition = 3

-- body types
eBodyAgent = 0
eBodyCommander = 1
eBodyCrowd = 2

-- NBC alert
eEtatNbc_Niv0 = 0
eEtatNbc_Niv4 = 1

-- Radio state
eEtatRadio_Ouverte = 0
eEtatRadio_Silence = 1
eEtatRadio_Silence_Partiel = 2

-- tri state enumeration
eTristate_True = 0
eTristate_False = 1
eTristate_DontKnow = 2
 
-- Illumination
eActionIllumination_Impossible = 0
eActionIllumination_EnemyDestroyed = 1
eActionIllumination_NoCapacity = 2
eActionIllumination_NoAmmo = 3
eActionIllumination_Running = 4
eActionIllumination_Finished = 5
eActionIllumination_None = 6

-- Fire condition -- Scipio compatibility
PH_TIR_ZURB = 0.4
PH_TIR_ZO = 0.5