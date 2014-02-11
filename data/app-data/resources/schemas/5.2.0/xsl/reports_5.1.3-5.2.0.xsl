<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fn="http://exslt.org/sets">
  <xsl:output method="xml" indent="yes" encoding="UTF-8"/>

  <xsl:template match="node()|@*">
    <xsl:copy>
      <xsl:apply-templates select="node()|@*"/>
    </xsl:copy>
  </xsl:template> 

  <xsl:template match="report">
      <xsl:variable name="report-id" select="@id"/>
      <xsl:copy>
          <xsl:attribute name="key">
            <xsl:choose>
              <xsl:when test="@id=0">eRC_RAS</xsl:when>
              <xsl:when test="@id=1">eRC_EnPosteFaceAObstacle</xsl:when>
              <xsl:when test="@id=2">eRC_EnContournementObstacle</xsl:when>
              <xsl:when test="@id=3">eRC_MissionImpossible</xsl:when>
              <xsl:when test="@id=4">eRC_ProgressionSurAxe</xsl:when>
              <xsl:when test="@id=5">eRC_ProgressionVersPosition</xsl:when>
              <xsl:when test="@id=6">eRC_ArriveSurPosition</xsl:when>
              <xsl:when test="@id=7">eRC_ReconnaissancePoint</xsl:when>
              <xsl:when test="@id=8">eRC_EnPosteFaceADirectionDangereuse</xsl:when>
              <xsl:when test="@id=9">eRC_EnJalonnement</xsl:when>
              <xsl:when test="@id=10">eRC_PreparationMission</xsl:when>
              <xsl:when test="@id=11">eRC_DebutMission</xsl:when>
              <xsl:when test="@id=12">eRC_FinMission</xsl:when>
              <xsl:when test="@id=13">eRC_DispositifInstalle</xsl:when>
              <xsl:when test="@id=14">eRC_EnPosition</xsl:when>
              <xsl:when test="@id=15">eRC_ObjectifAtteint</xsl:when>
              <xsl:when test="@id=16">eRC_Debouche</xsl:when>
              <xsl:when test="@id=17">eRC_RejointUnite</xsl:when>
              <xsl:when test="@id=18">eRC_EscorteUnite</xsl:when>
              <xsl:when test="@id=19">eRC_EnFreinage</xsl:when>
              <xsl:when test="@id=20">eRC_Assaut</xsl:when>
              <xsl:when test="@id=21">eRC_EnCouverture</xsl:when>
              <xsl:when test="@id=22">eRC_EnEclairage</xsl:when>
              <xsl:when test="@id=23">eRC_EnManoeuvreRavitaillement</xsl:when>
              <xsl:when test="@id=24">eRC_SurPIA</xsl:when>
              <xsl:when test="@id=25">eRC_PassageSurPIA</xsl:when>
              <xsl:when test="@id=26">eRC_UniteRecueillie</xsl:when>
              <xsl:when test="@id=27">eRC_EnCoursDeFranchissement</xsl:when>
              <xsl:when test="@id=28">eRC_AttenteOrdrePoursuivre</xsl:when>
              <xsl:when test="@id=29">eRC_AttenteOrdreDeboucher</xsl:when>
              <xsl:when test="@id=30">eRC_Fixe</xsl:when>
              <xsl:when test="@id=31">eRC_EnEsquive</xsl:when>
              <xsl:when test="@id=32">eRC_PriseContact</xsl:when>
              <xsl:when test="@id=33">eRC_RuptureContact</xsl:when>
              <xsl:when test="@id=34">eRC_PrecisionContact</xsl:when>
              <xsl:when test="@id=35">eRC_EnnemiDetruit</xsl:when>
              <xsl:when test="@id=36">eRC_PrisAPartieParTirDirect</xsl:when>
              <xsl:when test="@id=37">eRC_PrisSousTirArtillerie</xsl:when>
              <xsl:when test="@id=38">eRC_PointTenuParEni</xsl:when>
              <xsl:when test="@id=39">eRC_ProgressionVersEni</xsl:when>
              <xsl:when test="@id=40">eRC_Decroche</xsl:when>
              <xsl:when test="@id=41">eRC_EniSurObjectif</xsl:when>
              <xsl:when test="@id=42">eRC_Riposte</xsl:when>
              <xsl:when test="@id=43">eRC_PretPourEngagementSurEnnemi</xsl:when>
              <xsl:when test="@id=44">eRC_DebutControleZone</xsl:when>
              <xsl:when test="@id=45">eRC_FinControleZone</xsl:when>
              <xsl:when test="@id=46">eRC_DebutSurveillance</xsl:when>
              <xsl:when test="@id=47">eRC_FinSurveillance</xsl:when>
              <xsl:when test="@id=48">eRC_EnInfiltration</xsl:when>
              <xsl:when test="@id=49">eRC_EnExfiltration</xsl:when>
              <xsl:when test="@id=50">eRC_AttenteConditionFavorablePourInfiltration</xsl:when>
              <xsl:when test="@id=51">eRC_DroneLance</xsl:when>
              <xsl:when test="@id=52">eRC_DroneEnExploitation</xsl:when>
              <xsl:when test="@id=53">eRC_DroneExploite</xsl:when>
              <xsl:when test="@id=54">eRC_DroneDisponible</xsl:when>
              <xsl:when test="@id=55">eRC_DebutMiseEnOeuvreDrone</xsl:when>
              <xsl:when test="@id=56">eRC_FinMiseEnOeuvreDrone</xsl:when>
              <xsl:when test="@id=57">eRC_ActivationBrouillage</xsl:when>
              <xsl:when test="@id=58">eRC_DesactivationBrouillage</xsl:when>
              <xsl:when test="@id=59">eRC_ActivationSystemeEcoute</xsl:when>
              <xsl:when test="@id=60">eRC_DesactivationSystemeEcoute</xsl:when>
              <xsl:when test="@id=61">eRC_ActivationSystemeEcouteRadar</xsl:when>
              <xsl:when test="@id=62">eRC_DesactivationSystemeEcouteRadar</xsl:when>
              <xsl:when test="@id=63">eRC_DebutLiaison</xsl:when>
              <xsl:when test="@id=64">eRC_FinLiaison</xsl:when>
              <xsl:when test="@id=65">eRC_EtablissementLiaisonImpossible</xsl:when>
              <xsl:when test="@id=66">eRC_UniteTropDistante</xsl:when>
              <xsl:when test="@id=67">eRC_EnregistrementDonnes</xsl:when>
              <xsl:when test="@id=68">eRC_ExploitationDonnees</xsl:when>
              <xsl:when test="@id=69">eRC_ExtractionDonnes</xsl:when>
              <xsl:when test="@id=70">eRC_Releve</xsl:when>
              <xsl:when test="@id=71">eRC_SuitUnite</xsl:when>
              <xsl:when test="@id=72">eRC_PasseEnAppui</xsl:when>
              <xsl:when test="@id=73">eRC_EnAppui</xsl:when>
              <xsl:when test="@id=74">eRC_NonOperationnel</xsl:when>
              <xsl:when test="@id=75">eRC_Operationnel</xsl:when>
              <xsl:when test="@id=76">eRC_AmbianceVitesse</xsl:when>
              <xsl:when test="@id=77">eRC_AmbianceSurete</xsl:when>
              <xsl:when test="@id=78">eRC_SurLima</xsl:when>
              <xsl:when test="@id=79">eRC_DebutTravaux</xsl:when>
              <xsl:when test="@id=80">eRC_FinTravaux</xsl:when>
              <xsl:when test="@id=81">eRC_ObstacleDegage</xsl:when>
              <xsl:when test="@id=82">eRC_OrganisationChantier</xsl:when>
              <xsl:when test="@id=83">eRC_DesorganisationChantier</xsl:when>
              <xsl:when test="@id=84">eRC_ReconnaissancePositionObstacle</xsl:when>
              <xsl:when test="@id=85">eRC_ContournementRealise</xsl:when>
              <xsl:when test="@id=86">eRC_AttenteRenforcement</xsl:when>
              <xsl:when test="@id=87">eRC_DebutExploitationSiteFranchissement</xsl:when>
              <xsl:when test="@id=88">eRC_FinExploitationSiteFranchissement</xsl:when>
              <xsl:when test="@id=89">eRC_ReussiteRenforcement</xsl:when>
              <xsl:when test="@id=90">eRC_EchecRenforcement</xsl:when>
              <xsl:when test="@id=91">eRC_ObstacleEnAttenteActivation</xsl:when>
              <xsl:when test="@id=92">eRC_DebutAmenagementBerges</xsl:when>
              <xsl:when test="@id=93">eRC_FinAmenagementBerges</xsl:when>
              <xsl:when test="@id=94">eRC_RegimeMaintenanceDelaiDepasse</xsl:when>
              <xsl:when test="@id=95">eRC_SectionDeployee</xsl:when>
              <xsl:when test="@id=96">eRC_ANouveauDisponibleApresReparation</xsl:when>
              <xsl:when test="@id=97">eRC_MaterielRetourDeMaintenance</xsl:when>
              <xsl:when test="@id=98">eRC_MaterielRepareSurPlace</xsl:when>
              <xsl:when test="@id=99">eRC_DemandeRavitaillementDotations</xsl:when>
              <xsl:when test="@id=100">eRC_DemandeRavitaillementStocks</xsl:when>
              <xsl:when test="@id=101">eRC_DemandeEvacuationSanitaire</xsl:when>
              <xsl:when test="@id=102">eRC_DemandeEvacuationMateriel</xsl:when>
              <xsl:when test="@id=103">eRC_AllocationConsentieBientotEpuisee</xsl:when>
              <xsl:when test="@id=104">eRC_DepassementCapaciteStockage</xsl:when>
              <xsl:when test="@id=105">eRC_SeuilLogistiqueDotationDepasse</xsl:when>
              <xsl:when test="@id=106">eRC_SeuilLogistiqueStockDepasse</xsl:when>
              <xsl:when test="@id=107">eRC_RavitaillementDotationsEffectue</xsl:when>
              <xsl:when test="@id=108">eRC_RavitaillementStockEffectue</xsl:when>
              <xsl:when test="@id=109">eRC_RavitaillementDotationsAnnule</xsl:when>
              <xsl:when test="@id=110">eRC_RavitaillementStockAnnule</xsl:when>
              <xsl:when test="@id=111">eRC_HumainRetourDeSante</xsl:when>
              <xsl:when test="@id=112">eRC_AlerteDisponibiliteMoyensReleve</xsl:when>
              <xsl:when test="@id=113">eRC_AlerteDisponibiliteMoyensRamassage</xsl:when>
              <xsl:when test="@id=114">eRC_AlerteDisponibiliteMedecins</xsl:when>
              <xsl:when test="@id=115">eRC_AlerteDisponibiliteRemorqueurs</xsl:when>
              <xsl:when test="@id=116">eRC_AlerteDisponibiliteReparateurs</xsl:when>
              <xsl:when test="@id=117">eRC_AlerteDisponibiliteVecteurs</xsl:when>
              <xsl:when test="@id=118">eRC_PretMaterielEnCours</xsl:when>
              <xsl:when test="@id=119">eRC_PretMaterielAnnule</xsl:when>
              <xsl:when test="@id=120">eRC_PretMaterielEffectue</xsl:when>
              <xsl:when test="@id=121">eRC_PretMaterielImpossible</xsl:when>
              <xsl:when test="@id=122">eRC_PretMaterielPartiellementEffectue</xsl:when>
              <xsl:when test="@id=123">eRC_RecuperationMaterielPreteImpossible</xsl:when>
              <xsl:when test="@id=124">eRC_RecuperationMaterielPreteEffectuee</xsl:when>
              <xsl:when test="@id=125">eRC_RecuperationMaterielPretePartiellementEffectuee</xsl:when>
              <xsl:when test="@id=126">eRC_MaterielPrete</xsl:when>
              <xsl:when test="@id=127">eRC_MaterielRendu</xsl:when>
              <xsl:when test="@id=128">eRC_Convoi_DeplacementVersPointChargement</xsl:when>
              <xsl:when test="@id=129">eRC_Convoi_ChargementEnCours</xsl:when>
              <xsl:when test="@id=130">eRC_Convoi_ChargementEffectue</xsl:when>
              <xsl:when test="@id=131">eRC_Convoi_DeplacementVersPointDechargement</xsl:when>
              <xsl:when test="@id=132">eRC_Convoi_DechargementEnCours</xsl:when>
              <xsl:when test="@id=133">eRC_Convoi_DechargementEffectue</xsl:when>
              <xsl:when test="@id=134">eRC_Convoi_Retour</xsl:when>
              <xsl:when test="@id=135">eRC_EnCoursRavitaillement</xsl:when>
              <xsl:when test="@id=136">eRC_RavitaillementTermine</xsl:when>
              <xsl:when test="@id=137">eRC_HelicoptereEnObservation</xsl:when>
              <xsl:when test="@id=138">eRC_PerteInfoGuidage</xsl:when>
              <xsl:when test="@id=139">eRC_TransportUnitePasPrete</xsl:when>
              <xsl:when test="@id=140">eRC_TransportUnitePrete</xsl:when>
              <xsl:when test="@id=141">eRC_TransportEnCours</xsl:when>
              <xsl:when test="@id=142">eRC_TransportEmbarquement</xsl:when>
              <xsl:when test="@id=143">eRC_TransportDebarquement</xsl:when>
              <xsl:when test="@id=144">eRC_TransportImpossiblePasDeMoyens</xsl:when>
              <xsl:when test="@id=145">eRC_VehiculeLaisseSurPosition</xsl:when>
              <xsl:when test="@id=146">eRC_RejointPositionEmbarquement</xsl:when>
              <xsl:when test="@id=147">eRC_EnAttenteTransporteurs</xsl:when>
              <xsl:when test="@id=148">eRC_ContamineNBC</xsl:when>
              <xsl:when test="@id=149">eRC_DecontamineNBC</xsl:when>
              <xsl:when test="@id=150">eRC_TenueProtectionNBCMise</xsl:when>
              <xsl:when test="@id=151">eRC_TenueProtectionNBCEnlevee</xsl:when>
              <xsl:when test="@id=152">eRC_AlerteNBCNiveau4</xsl:when>
              <xsl:when test="@id=153">eRC_FinAlerteNBCNiveau4</xsl:when>
              <xsl:when test="@id=154">eRC_SiteDecontaminationAnime</xsl:when>
              <xsl:when test="@id=155">eRC_SiteDecontaminationDesactive</xsl:when>
              <xsl:when test="@id=156">eRC_DemandeDecontamination</xsl:when>
              <xsl:when test="@id=157">eRC_ZoneReconnue</xsl:when>
              <xsl:when test="@id=158">eRC_ZoneDecontaminee</xsl:when>
              <xsl:when test="@id=159">eRC_SiteROTAReconnu</xsl:when>
              <xsl:when test="@id=160">eRC_IncidentNBCTermine</xsl:when>
              <xsl:when test="@id=161">eRC_UniteDecontaminee</xsl:when>
              <xsl:when test="@id=162">eRC_SiteSature</xsl:when>
              <xsl:when test="@id=163">eRC_DebutMesure</xsl:when>
              <xsl:when test="@id=164">eRC_FinMesure</xsl:when>
              <xsl:when test="@id=165">eRC_ReconnaissanceTerminee</xsl:when>
              <xsl:when test="@id=166">eRC_ReconnaissanceInitialeTerminee</xsl:when>
              <xsl:when test="@id=167">eRC_RejointPlotRavitaillement</xsl:when>
              <xsl:when test="@id=168">eRC_PremierCoupParti</xsl:when>
              <xsl:when test="@id=169">eRC_TirExecute</xsl:when>
              <xsl:when test="@id=170">eRC_TirHorsDePortee</xsl:when>
              <xsl:when test="@id=171">eRC_TirImpossiblePlusDeMunitions</xsl:when>
              <xsl:when test="@id=172">eRC_TirAnnule</xsl:when>
              <xsl:when test="@id=173">eRC_ChangementDePosition</xsl:when>
              <xsl:when test="@id=174">eRC_DebutInstallation</xsl:when>
              <xsl:when test="@id=175">eRC_FinInstallation</xsl:when>
              <xsl:when test="@id=176">eRC_PretPourConduiteTir</xsl:when>
              <xsl:when test="@id=177">eRC_ObservationTirIndirect</xsl:when>
              <xsl:when test="@id=178">eRC_ZoneImplantationOccupee</xsl:when>
              <xsl:when test="@id=179">eRC_InstallationSurZoneImplantation</xsl:when>
              <xsl:when test="@id=180">eRC_ReceptionOrdreDeFeu</xsl:when>
              <xsl:when test="@id=181">eRC_PretAuTir</xsl:when>
              <xsl:when test="@id=182">eRC_PresencePopulation</xsl:when>
              <xsl:when test="@id=183">eRC_OrientationPopulationVersCampRefugies</xsl:when>
              <xsl:when test="@id=184">eRC_PasDeCampRefugiesDisponible</xsl:when>
              <xsl:when test="@id=185">eRC_PresenceEnnemiRendu</xsl:when>
              <xsl:when test="@id=186">eRC_OrientationEnnemiRenduVersCampPrisonniers</xsl:when>
              <xsl:when test="@id=187">eRC_PasDeCampPrisonniersDisponible</xsl:when>
              <xsl:when test="@id=188">eRC_Rendu</xsl:when>
              <xsl:when test="@id=189">eRC_RedditionAnnulee</xsl:when>
              <xsl:when test="@id=190">eRC_TirDansZoneInterdite</xsl:when>
              <xsl:when test="@id=191">eRC_TirSurCampAmi</xsl:when>
              <xsl:when test="@id=192">eRC_TireParCampAmi</xsl:when>
              <xsl:when test="@id=193">eRC_TirSurCampNeutre</xsl:when>
              <xsl:when test="@id=194">eRC_TireParCampNeutre</xsl:when>
              <xsl:when test="@id=195">eRC_TirSurCivil</xsl:when>
              <xsl:when test="@id=196">eRC_TireParCivil</xsl:when>
              <xsl:when test="@id=197">eRC_TirIndirectFratricide</xsl:when>
              <xsl:when test="@id=198">eRC_TirIndirectSurPopulation</xsl:when>
              <xsl:when test="@id=199">eRC_DestructionPC</xsl:when>
              <xsl:when test="@id=200">eRC_DecesBlesse</xsl:when>
              <xsl:when test="@id=201">eRC_DecesBlessePendantTransport</xsl:when>
              <xsl:when test="@id=202">eRC_DecesBlessePendantHospitalisation</xsl:when>
              <xsl:when test="@id=203">eRC_MissionImpossibleUnitesSubordonneesNonOperationnelles</xsl:when>
              <xsl:when test="@id=204">eRC_MissionImpossibleLimaInvalide</xsl:when>
              <xsl:when test="@id=205">eRC_MissionImpossibleZoneHorsFuseau</xsl:when>
              <xsl:when test="@id=206">eRC_RapportDeForceDefavorable</xsl:when>
              <xsl:when test="@id=207">eRC_TerrainDifficile</xsl:when>
              <xsl:when test="@id=208">eRC_PlusDeCarburant</xsl:when>
              <xsl:when test="@id=209">eRC_DispositifDisperse</xsl:when>
              <xsl:when test="@id=210">eRC_DebutNeutralisation</xsl:when>
              <xsl:when test="@id=211">eRC_FinNeutralisation</xsl:when>
              <xsl:when test="@id=212">eRC_DebutSilenceRadio</xsl:when>
              <xsl:when test="@id=213">eRC_FinSilenceRadio</xsl:when>
              <xsl:when test="@id=214">eRC_DebutSilenceRadar</xsl:when>
              <xsl:when test="@id=215">eRC_FinSilenceRadar</xsl:when>
              <xsl:when test="@id=216">eRC_TirRestreint</xsl:when>
              <xsl:when test="@id=217">eRC_TirInterdit</xsl:when>
              <xsl:when test="@id=218">eRC_TirLibre</xsl:when>
              <xsl:when test="@id=219">eRC_Trace</xsl:when>
              <xsl:when test="@id=220">eRC_TraceID</xsl:when>
              <xsl:when test="@id=221">eRC_EmploiForceInterdit</xsl:when>
              <xsl:when test="@id=222">eRC_MaintienADistanceParMoyensNonLetaux</xsl:when>
              <xsl:when test="@id=223">eRC_ArmesLetalesAutorisees</xsl:when>
              <xsl:when test="@id=224">eRC_PopulationDetectee</xsl:when>
              <xsl:when test="@id=225">eRC_PriseAPartieParPopulation</xsl:when>
              <xsl:when test="@id=226">eRC_RiposteContrePopulation</xsl:when>
              <xsl:when test="@id=227">eRC_PopulationDangereuseAProximite</xsl:when>
              <xsl:when test="@id=228">eRC_EsquiveFaceAPopulation</xsl:when>
              <xsl:when test="@id=229">eRC_DecrochageFaceAPopulation</xsl:when>
              <xsl:when test="@id=230">eRC_MiseEnPlaceControleDeFoule</xsl:when>
              <xsl:when test="@id=231">eRC_MiseEnPlaceFiltrage</xsl:when>
              <xsl:when test="@id=232">eRC_AnimationBarrage</xsl:when>
              <xsl:when test="@id=233">eRC_AnimationCanalisation</xsl:when>
              <xsl:when test="@id=234">eRC_CollecteRenseignementsSurPopulations</xsl:when>
              <xsl:when test="@id=235">eRC_PopulationReconnue</xsl:when>
              <xsl:when test="@id=236">eRC_SituationDifficilementTenableFaceAPopulation</xsl:when>
              <xsl:when test="@id=237">eRC_UtilisationArmesNonLetalesFaceAPopulation</xsl:when>
              <xsl:when test="@id=238">eRC_UtilisationArmementsFaceAPopulation</xsl:when>
              <xsl:when test="@id=239">eRC_DebutInterventionFaceAPopulation</xsl:when>
              <xsl:when test="@id=240">eRC_PopulationVictimeAffrontements</xsl:when>
              <xsl:when test="@id=241">eRC_AttitudePopulation</xsl:when>
              <xsl:when test="@id=242">eRC_AggravationAttitude</xsl:when>
              <xsl:when test="@id=243">eRC_AffrontementAvecForcesEnnemies</xsl:when>
              <xsl:when test="@id=244">eRC_PopulationTemporairementControlee</xsl:when>
              <xsl:when test="@id=245">eRC_PopulationTemporairementRepoussee</xsl:when>
              <xsl:when test="@id=246">eRC_RepriseDesAffrontements</xsl:when>
              <xsl:when test="@id=247">eRC_AgressionDePopulation</xsl:when>
              <xsl:when test="@id=248">eRC_AgressionParPopulation</xsl:when>
              <xsl:when test="@id=249">eRC_RiposteContreAgressionParPopulation</xsl:when>
              <xsl:when test="@id=250">eRC_AttaqueInstallation</xsl:when>
              <xsl:when test="@id=251">eRC_EnStationnement</xsl:when>
              <xsl:when test="@id=252">eRC_Bloquee</xsl:when>
              <xsl:when test="@id=253">eRC_Filtree</xsl:when>
              <xsl:when test="@id=254">eRC_AttentatTerroristeDansPopulation</xsl:when>
              <xsl:when test="@id=255">eRC_PopulationVictimeExplosionMines</xsl:when>
              <xsl:when test="@id=256">eRC_ExecutionAttentat</xsl:when>
              <xsl:when test="@id=257">eRC_BM_DebutMission</xsl:when>
              <xsl:when test="@id=258">eRC_BM_FinMission</xsl:when>
              <xsl:when test="@id=259">eRC_BM_DebutPhase</xsl:when>
              <xsl:when test="@id=260">eRC_BM_FinPhase</xsl:when>
              <xsl:when test="@id=261">eRC_ActivationObstacleDeManoeuvre</xsl:when>
              <xsl:when test="@id=262">eRC_DebutAnimationObjet</xsl:when>
              <xsl:when test="@id=263">eRC_TirDirectSurCible</xsl:when>
              <xsl:when test="@id=264">eRC_FinAnimationObjet</xsl:when>
              <xsl:when test="@id=265">eRC_IllumineCible</xsl:when>
              <xsl:when test="@id=266">eRC_DebutDegagement</xsl:when>
              <xsl:when test="@id=267">eRC_FinDegagement</xsl:when>
              <xsl:when test="@id=268">eRC_TirDirectImpossible</xsl:when>
              <xsl:when test="@id=269">eRC_TirIndirectSurCible</xsl:when>
              <xsl:when test="@id=270">eRC_TirSansCapacite</xsl:when>
              <xsl:when test="@id=271">eRC_TirIndirectMunitionInvalide</xsl:when>
              <xsl:when test="@id=272">eRC_TirIndirectImpossible</xsl:when>
              <xsl:when test="@id=273">eRC_CibleNonPercue</xsl:when>
              <xsl:when test="@id=274">eRC_EnnemiTactiquementDetruit</xsl:when>
              <xsl:when test="@id=275">eRC_VisionRadarVerrouilleeSur</xsl:when>
              <xsl:when test="@id=276">eRC_DebutReconnaissanceBlocUrbain</xsl:when>
              <xsl:when test="@id=277">eRC_FinReconnaissanceBlocUrbain</xsl:when>
              <xsl:when test="@id=278">eRC_DebutReconnaissancePoint</xsl:when>
              <xsl:when test="@id=279">eRC_FinReconnaissancePoint</xsl:when>
              <xsl:when test="@id=280">eRC_FinTirDirectSurCible</xsl:when>
              <xsl:when test="@id=281">eRC_DebutTravauxObjet</xsl:when>
              <xsl:when test="@id=282">eRC_FinTravauxObjet</xsl:when>
              <xsl:when test="@id=283">eRC_FinTirIndirectSurCible</xsl:when>
              <xsl:when test="@id=284">eRC_DebutNeutralisationCible</xsl:when>
              <xsl:when test="@id=285">eRC_FinNeutralisationCible</xsl:when>
              <xsl:when test="@id=286">eRC_VisionVerrouilleeSur</xsl:when>
              <xsl:when test="@id=287">eRC_DebutConstructionObjet</xsl:when>
              <xsl:when test="@id=288">eRC_ObjetDejaOccupe</xsl:when>
              <xsl:when test="@id=289">eRC_DebutSilencePartielRadio</xsl:when>
              <xsl:when test="@id=290">eRC_DebutReconnaissanceZone</xsl:when>
              <xsl:when test="@id=291">eRC_FinReconnaissanceZone</xsl:when>
              <xsl:when test="@id=292">eRC_ConstructionObjetImpossible</xsl:when>
              <xsl:when test="@id=293">eRC_PasDotationConstructionObjet</xsl:when>
              <xsl:when test="@id=294">eRC_Embarquement</xsl:when>
              <xsl:when test="@id=295">eRC_Debarquement</xsl:when>
              <xsl:when test="@id=296">eRC_DebutFouilleBlocUrbain</xsl:when>
              <xsl:when test="@id=297">eRC_FinFouilleBlocUrbain</xsl:when>
              <xsl:when test="@id=298">eRC_DebutFouilleZone</xsl:when>
              <xsl:when test="@id=299">eRC_FinFouilleZone</xsl:when>
              <xsl:when test="@id=300">eRC_MunitionInterdite</xsl:when>
              <xsl:when test="@id=301">eRC_MunitionAutorise</xsl:when>
              <xsl:when test="@id=302">eRC_DebutBouclageZone</xsl:when>
              <xsl:when test="@id=303">eRC_FinBouclageZone</xsl:when>
              <xsl:when test="@id=304">eRC_DebutAppui</xsl:when>
              <xsl:when test="@id=305">eRC_FinAppui</xsl:when>
              <xsl:when test="@id=306">eRC_ObjetNonContournable</xsl:when>
              <xsl:when test="@id=307">eRC_ObjetNonSupprimable</xsl:when>
              <xsl:when test="@id=308">eRC_TirIndirectTargetNotIlluminated</xsl:when>
              <xsl:when test="@id=309">eRC_TirIndirectForbiddenAmmo</xsl:when>
              <xsl:when test="@id=310">eRC_MissionImpossibleReason</xsl:when>
              <xsl:when test="@id=311">eRC_CouvertureActive</xsl:when>
              <xsl:when test="@id=312">eRC_CouvertureDesactive</xsl:when>
              <xsl:when test="@id=313">eRC_DebutControleBlocUrbain</xsl:when>
              <xsl:when test="@id=314">eRC_FinControleBlocUrbain</xsl:when>
              <xsl:when test="@id=315">eRC_DebutControlPoint</xsl:when>
              <xsl:when test="@id=316">eRC_FinControlPoint</xsl:when>
              <xsl:when test="@id=317">eRC_TirIndirectNoCapacity</xsl:when>
              <xsl:when test="@id=318">eRC_DebutSilenceRadioPartiel</xsl:when>
              <xsl:when test="@id=319">eRC_EmbarkmentStarted</xsl:when>
              <xsl:when test="@id=320">eRC_EmbarkmentInterrupted</xsl:when>
              <xsl:when test="@id=321">eRC_EmbarkmentResumed</xsl:when>
              <xsl:when test="@id=322">eRC_EmbarkmentFinished</xsl:when>
              <xsl:when test="@id=323">eRC_DisembarkmentStarted</xsl:when>
              <xsl:when test="@id=324">eRC_DisembarkmentInterrupted</xsl:when>
              <xsl:when test="@id=325">eRC_DisembarkmentResumed</xsl:when>
              <xsl:when test="@id=326">eRC_DisembarkmentFinished</xsl:when>
              <xsl:when test="@id=327">eRC_ProtectionActiveReussie</xsl:when>
              <xsl:when test="@id=328">eRC_ProtectionActiveHardKillReussie</xsl:when>
              <xsl:when test="@id=329">eRC_Empoisonne</xsl:when>
              <xsl:when test="@id=330">eRC_Contamine</xsl:when>
              <xsl:when test="@id=331">eRC_TirSurCampEnnemi</xsl:when>
              <xsl:when test="@id=332">eRC_TireParCampEnnemi</xsl:when>
              <xsl:when test="@id=333">eRC_DroneDoitEtreMisEnOeuvre</xsl:when>
              <xsl:when test="@id=334">eRC_ReussiteRenforcementPion</xsl:when>
              <xsl:when test="@id=335">eRC_EchecRenforcementPion</xsl:when>
              <xsl:when test="@id=336">eRC_LeaveLima</xsl:when>
              <xsl:when test="@id=337">eRC_ArrivedOnExtinguishableObject</xsl:when>
              <xsl:when test="@id=338">eRC_ObjectExtinguished</xsl:when>
              <xsl:when test="@id=339">eRC_ArrivedOnAlertZone</xsl:when>
              <xsl:when test="@id=340">eRC_ZoneAlerted</xsl:when>
              <xsl:when test="@id=341">eRC_ArrivedOnGaragePosition</xsl:when>
              <xsl:when test="@id=342">eRC_ExtractFromStock_Arrived</xsl:when>
              <xsl:when test="@id=343">eRC_ExtractFromStock_Extracted</xsl:when>
              <xsl:when test="@id=344">eRC_SupplyStock_Supplied</xsl:when>
              <xsl:when test="@id=345">eRC_SupplyStock_Arrived</xsl:when>
              <xsl:when test="@id=346">eRC_SupplyStock_Distributed</xsl:when>
              <xsl:when test="@id=347">eRC_ZoneConfined</xsl:when>
              <xsl:when test="@id=348">eRC_ArrivedOnConfinedZone</xsl:when>
              <xsl:when test="@id=349">eRC_PasDeDroneDisponible</xsl:when>
              <xsl:when test="@id=350">eRC_FirstTourOfZoneConfinementDone</xsl:when>
              <xsl:when test="@id=351">eRC_ZoneConfinementInProgress</xsl:when>
              <xsl:when test="@id=352">eRC_ZoneEvacuated</xsl:when>
              <xsl:when test="@id=353">eRC_ArrivedOnEvacuateZone</xsl:when>
              <xsl:when test="@id=354">eRC_ZoneEvacuationInProgress</xsl:when>
              <xsl:when test="@id=355">eRC_DebutValorisation</xsl:when>
              <xsl:when test="@id=356">eRC_FinValorisation</xsl:when>
              <xsl:when test="@id=357">eRC_EchecRelever</xsl:when>
              <xsl:when test="@id=358">eRC_criticalIntelligence</xsl:when>
              <xsl:when test="@id=359">eRC_criticalIntelligenceNone</xsl:when>
              <xsl:when test="@id=360">eRC_FinTravauxBlocUrbain</xsl:when>
              <xsl:when test="@id=361">eRC_BUNonTraficable</xsl:when>
              <xsl:when test="@id=362">eRC_StartDeploy</xsl:when>
              <xsl:when test="@id=363">eRC_DebutFouillePoint</xsl:when>
              <xsl:when test="@id=364">eRC_FinFouillePoint</xsl:when>
              <xsl:when test="@id=365">eRC_NotDeployedIndirectFireImpossible</xsl:when>
              <xsl:when test="@id=366">eRC_AgressionImpossible</xsl:when>
              <xsl:when test="@id=367">eRC_NoLogisticSuperior</xsl:when>
              <xsl:when test="@id=368">eRC_CrossingSiteNotRecon</xsl:when>
              <xsl:when test="@id=369">eRC_InvalidObject</xsl:when>
              <xsl:when test="@id=370">eRC_TerroristCaptured</xsl:when>
              <xsl:when test="@id=371">eRC_MiseEnPlaceProducePoint</xsl:when>
              <xsl:when test="@id=372">eRC_ProduceImpossible</xsl:when>
              <xsl:when test="@id=373">eRC_FinProducePoint</xsl:when>
              <xsl:when test="@id=374">eRC_TimeInUndergroundNetwork</xsl:when>
              <xsl:when test="@id=375">eRC_BadUndergroundNetwork</xsl:when>
              <xsl:when test="@id=376">eRC_NotActivatedUndergroundNetwork</xsl:when>
              <xsl:when test="@id=377">eRC_ImpossibleAtteindreObjectif</xsl:when>
              <xsl:when test="@id=378">eRC_ExplosionSurBouchonMine</xsl:when>
              <xsl:when test="@id=379">eRC_PathfindWarning</xsl:when>
              <xsl:when test="@id=380">eRC_DotationIndisponible</xsl:when>
              <xsl:when test="@id=381">eRC_ObservationEffetTirIndirect</xsl:when>
              <xsl:when test="@id=382">eRC_DamagesCausedToNeutralSide</xsl:when>
              <xsl:when test="@id=383">eRC_DamagesCausedByNeutralSide</xsl:when>
              <xsl:when test="@id=384">eRC_InfrastructureDamaged</xsl:when>
              <xsl:when test="@id=385">eRC_LivingAreaDamaged</xsl:when>
              <xsl:when test="@id=386">eRC_BorderCrossed</xsl:when>
              <xsl:when test="@id=387">eRC_TacticallyDestroyed</xsl:when>
              <xsl:when test="@id=388">eRC_CiviliansEncountered</xsl:when>
              <xsl:when test="@id=389">eRC_PrisonersUnsupplied</xsl:when>
              <xsl:when test="@id=390">eRC_PrisonersCampFull</xsl:when>
              <xsl:when test="@id=391">eRC_NoPositionsToDestroyTargets</xsl:when>
              <xsl:when test="@id=392">eRC_NoPositionsToNeutralizeTargets</xsl:when>
              <xsl:when test="@id=393">eRC_NoPositionsToGetCoverFromEntities</xsl:when>
              <xsl:when test="@id=394">eRC_NoPositionsToObserveTargets</xsl:when>
              <xsl:when test="@id=395">eRC_NoPositionsToReachTargets</xsl:when>
              <xsl:when test="@id=396">eRC_TheTargetCannotBeReconnoitre</xsl:when>
              <xsl:when test="@id=397">eRC_TargetIsNotAccessible</xsl:when>
              <xsl:when test="@id=398">eRC_NoKeyPositionsFoundInArea</xsl:when>
              <xsl:when test="@id=399">eRC_Engaged</xsl:when>
              <xsl:when test="@id=400">eRC_Disengaged</xsl:when>
              <xsl:when test="@id=401">eRC_AlreadyEngaged</xsl:when>
              <xsl:when test="@id=402">eRC_AlreadyDisengaged</xsl:when>
              <xsl:when test="@id=403">eRC_NoPEInAutomat</xsl:when>
              <xsl:when test="@id=404">eRC_SectionUndeployed</xsl:when>
              <xsl:when test="@id=405">eRC_StartUndeploy</xsl:when>
              <xsl:when test="@id=406">eRC_DifficultMovementProgression</xsl:when>
              <xsl:when test="@id=407">eRC_LimaParameterNotPresent</xsl:when>
              <xsl:when test="@id=408">eRC_UrbanCollisionStarted</xsl:when>
              <xsl:when test="@id=409">eRC_UrbanCollisionStopped</xsl:when>
              <xsl:when test="@id=410">eRC_DecrochageFaceAIncendie</xsl:when>
              <xsl:when test="@id=411">eRC_Capture</xsl:when>
              <xsl:when test="@id=412">eRC_TakeHostage</xsl:when>
              <xsl:when test="@id=413">eRC_AttenteOrdrePoursuivreConvoiOuEscorte</xsl:when>
              <xsl:when test="@id=414">eRC_DebutDevalorisation</xsl:when>
              <xsl:when test="@id=415">eRC_FinDevalorisation</xsl:when>
              <xsl:when test="@id=416">eRC_TrafficablePositionFindingInZone</xsl:when>
              <xsl:when test="@id=417">eRC_PrisSousTirIED</xsl:when>
              <xsl:when test="@id=418">eRC_NotInSameUndergroundNetwork</xsl:when>
              <xsl:when test="@id=419">eRC_TerroristDropped</xsl:when>
              <xsl:when test="@id=420">eRC_Questionning</xsl:when>
              <xsl:when test="@id=421">eRC_criticalIntelligenceImpossible</xsl:when>
              <xsl:when test="@id=422">eRC_InvalidTarget</xsl:when>
              <xsl:when test="@id=423">eRC_FragOrderReceived</xsl:when>
              <xsl:when test="@id=424">eRC_FurtifModeOn</xsl:when>
              <xsl:when test="@id=425">eRC_FurtifModeOff</xsl:when>
              <xsl:when test="@id=426">eRC_ObstacleNonActivable</xsl:when>
              <xsl:when test="@id=427">eRC_FireObserver</xsl:when>
              <xsl:when test="@id=428">eRC_ImpossibleActivateIED</xsl:when>
              <xsl:when test="@id=429">eRC_Jammed</xsl:when>
              <xsl:when test="@id=430">eRC_PCJammed</xsl:when>
              <xsl:when test="@id=431">eRC_IEDNotReady</xsl:when>
              <xsl:when test="@id=432">eRC_IEDReadyForFiring</xsl:when>
              <xsl:when test="@id=433">eRC_FollowingUnitIsDead</xsl:when>
              <xsl:when test="@id=434">eRC_CombatSupportUnitIsDead</xsl:when>
              <xsl:when test="@id=435">eRC_NoUnitsInParameter</xsl:when>
              <xsl:when test="@id=436">eRC_DetectedUnit</xsl:when>
              <xsl:when test="@id=437">eRC_FriendUnitRecognized</xsl:when>
              <xsl:when test="@id=438">eRC_FriendUnitIdentified</xsl:when>
              <xsl:when test="@id=439">eRC_EnemyUnitRecognized</xsl:when>
              <xsl:when test="@id=440">eRC_EnemyUnitIdentified</xsl:when>
              <xsl:when test="@id=441">eRC_NeutralUnitRecognized</xsl:when>
              <xsl:when test="@id=442">eRC_NeutralUnitIdentified</xsl:when>
              <xsl:when test="@id=443">eRC_CaptureEnd</xsl:when>
              <xsl:when test="@id=444">eRC_PrisSousTirEclairant</xsl:when>
              <xsl:when test="@id=445">eRC_PrisSousTirFumigene</xsl:when>
              <xsl:when test="@id=446">eRC_DetectedObject</xsl:when>
              <xsl:when test="@id=447">eRC_OfficersKilled</xsl:when>
              <xsl:when test="@id=448">eRC_UnitNotCaptured</xsl:when>
              <xsl:when test="@id=449">eRC_RepairEvacuationNoMeans</xsl:when>
              <xsl:when test="@id=450">eRC_SupplierUnavailable</xsl:when>
              <xsl:when test="@id=451">eRC_LogQuotaExceeded</xsl:when>
              <xsl:when test="@id=452">eRC_LogQuotaExceededForAgent</xsl:when>
              <xsl:when test="@id=453">eRC_TransportedUnitCannotReceiveOrder</xsl:when>
              <xsl:when test="@id=454">eRC_LogNoSuperior</xsl:when>
              <xsl:when test="@id=455">eRC_LogNoStock</xsl:when>
              <xsl:when test="@id=456">eRC_ResourceNodeDisabled</xsl:when>
              <xsl:when test="@id=457">eRC_ObjectiveIsNoLongerAlerted</xsl:when>
              <xsl:when test="@id=458">eRC_ObjectiveConfined</xsl:when>
              <xsl:when test="@id=459">eRC_FireIsBeingExtinguished</xsl:when>
              <xsl:when test="@id=460">eRC_ImpossibleToOperateObject</xsl:when>
              <xsl:when test="@id=461">eRC_NoUrbanBlocksWithinArea</xsl:when>
              <xsl:when test="@id=462">eRC_VictimsExtracted</xsl:when>
              <xsl:when test="@id=463">eRC_ResourceReductionStopped</xsl:when>
              <xsl:when test="@id=464">eRC_EntityCannotFollow</xsl:when>
              <xsl:when test="@id=465">eRC_CannotDismount</xsl:when>
              <xsl:when test="@id=466">eRC_ObjectiveAlerted</xsl:when>
              <xsl:when test="@id=467">eRC_ObjectiveIsNoLongerConfined</xsl:when>
              <xsl:when test="@id=468">eRC_ObjectiveEvacuated</xsl:when>
              <xsl:when test="@id=469">eRC_ObjectiveIsNoLongerEvacuated</xsl:when>
              <xsl:when test="@id=470">eRC_StartingToProduceResource</xsl:when>
              <xsl:when test="@id=471">eRC_ResourceProductionStopped</xsl:when>
              <xsl:when test="@id=472">eRC_ResourceNodeProductionEnhanced</xsl:when>
              <xsl:when test="@id=473">eRC_ResourceNodeProductionDamaged</xsl:when>
              <xsl:when test="@id=474">eRC_ResourceNodeEnabled</xsl:when>
              <xsl:when test="@id=475">eRC_NoResourceNodesWithinArea</xsl:when>
              <xsl:when test="@id=476">eRC_NoCrowdsWithinArea</xsl:when>
              <xsl:when test="@id=477">eRC_VictimsEvacuated</xsl:when>
              <xsl:when test="@id=478">eRC_VictimsCannotBeEvacuated</xsl:when>
              <xsl:when test="@id=479">eRC_VictimsCannotBeExtracted</xsl:when>
              <xsl:when test="@id=480">eRC_StartingToReduceResourceProduction</xsl:when>
              <xsl:when test="@id=481">eRC_NoWoundedVictims</xsl:when>
              <xsl:when test="@id=482">eRC_ElementCannotBeLoaded</xsl:when>
              <xsl:when test="@id=483">eRC_ElementCannotBeUnloaded</xsl:when>
              <xsl:when test="@id=484">eRC_ElementLoaded</xsl:when>
              <xsl:when test="@id=485">eRC_ElementUnloaded</xsl:when>
              <xsl:when test="@id=486">eRC_NoPopulationInUrbanBlock</xsl:when>
              <xsl:when test="@id=487">eRC_ImpossibleToExtinguishFire</xsl:when>
              <xsl:when test="@id=488">eRC_NoFiresWithinArea</xsl:when>
              <xsl:when test="@id=489">eRC_StartingToDecontaminate</xsl:when>
              <xsl:when test="@id=490">eRC_DecontaminationDone</xsl:when>
              <xsl:when test="@id=491">eRC_StartingToHealCrowd</xsl:when>
              <xsl:when test="@id=492">eRC_WoundedPersonsHealed</xsl:when>
              <xsl:when test="@id=493">eRC_StartingToExtractVictimsFromCrowd</xsl:when>
              <xsl:when test="@id=494">eRC_WoundedCannotBeHealed</xsl:when>
              <xsl:when test="@id=495">eRC_ControlPointEstablished</xsl:when>
              <xsl:when test="@id=496">eRC_SystemIsBeingOperated</xsl:when>
              <xsl:when test="@id=497">eRC_EntityHasBeenOrderedToFollow</xsl:when>
              <xsl:when test="@id=498">eRC_StartingToCreateResourceLink</xsl:when>
              <xsl:when test="@id=499">eRC_ResouceLinkCreated</xsl:when>
              <xsl:when test="@id=503">eRC_AttenteOrdrePoursuivreConvoi</xsl:when>
              <xsl:when test="@id=504">eRC_TirIndirectLessInterventionType</xsl:when>
              <xsl:when test="@id=506">eRC_CloseCrowdAttitudeChanged</xsl:when>
              <xsl:when test="@id=507">eRC_CloseCrowdUrbanDestruction</xsl:when>
              <xsl:when test="@id=508">eRC_AgentNeutralizedIndirectFireImpossible</xsl:when>
              <xsl:when test="@id=509">eRC_ObjetNonDevalorisable</xsl:when>
              <xsl:when test="@id=510">eRC_SurrenderedUnitRecognized</xsl:when>
              <xsl:when test="@id=511">eRC_SurrenderedUnitIdentified</xsl:when>
              <xsl:when test="@id=769">eRC_CorpseTransported</xsl:when>
              <xsl:when test="@id=781">eRC_IdentifiedUnitSurrendered</xsl:when>
              <xsl:when test="@id=782">eRC_RecognizedUnitSurrendered</xsl:when>
              <xsl:otherwise/>
            </xsl:choose>
          </xsl:attribute>
          <xsl:apply-templates select="node()|@*"/>
      </xsl:copy>
  </xsl:template>

</xsl:stylesheet>
