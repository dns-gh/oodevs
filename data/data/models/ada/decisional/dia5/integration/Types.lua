-------------------------------------------------------------------------------
-- Global Types : 
-- @author MGD
-- @created 2010-06-24
--
-- This file is part of a MASA library or program.
-- Refer to the included end-user license agreement for restrictions.
--
-- Copyright (c) 2010 Math�matiques Appliqu�es SA (MASA)
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
ePionEfficiencyCommon = 8 -- valeur par d�faut pour les mission qui n'ont pas besoin d'un pion en particulier

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


eTypeObjectBouchonMines = 0
eTypeObjectZoneMineeLineaire = 1
eTypeObjectZoneMineeParDispersion = 2
eTypeObjectFosseAntiChar = 3
eTypeObjectAbatti = 4
eTypeObjectBarricade = 5
eTypeObjectEboulement = 6
eTypeObjectDestructionRoute = 7
eTypeObjectDestructionPont = 8
eTypeObjectPontFlottantContinu = 9
eTypeObjectPosteTir = 10
eTypeObjectZoneProtegee = 11
eTypeObjectZoneImplantationCanon = 12
eTypeObjectZoneImplantationCOBRA = 13
eTypeObjectZoneImplantationLRM = 14
eTypeObjectSiteFranchissement = 15
eTypeObjectNuageNBC = 16
eTypeObjectPlotRavitaillement = 17
eTypeObjectSiteDecontamination = 18
eTypeObjectZoneBrouillage = 19
eTypeObjectRota = 20
eTypeObjectZoneNBC = 21
eTypeObjectAirePoser = 22
eTypeObjectPiste = 23
eTypeObjectPlateForme = 24
eTypeObjectZoneMobiliteAmelioree = 25
eTypeObjectZonePoserHelicoptere = 26
eTypeObjectAireLogistique = 27
eTypeObjectItineraireLogistique = 28
eTypeObjectCampPrisonniers = 29
eTypeObjectCampRefugies = 30
eTypeObjectPosteControle = 31
eTypeObjectTerrainLargage = 32
eTypeObjectZoneInterditeDeplacement = 33
eTypeObjectZoneInterditeTir = 34
eTypeObjectZoneImplantationMortier = 35
eTypeObjectPontFlottantDiscontinu = 36
eTypeObjectInstallation = 37
eTypeObjectDynamicFire = 38
eTypeObjectEmergencyShelter = 39
eTypeObjectSupplyFacility = 40

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
eEtatROEPopulation_EmploiForceInterdit = 0
eEtatROEPopulation_MaintienADistanceParMoyensNonLetaux = 1
eEtatROEPopulation_ArmesLetalesAutorisees = 2

-- Il faut 45 min pour passer de 0 � 100%
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