<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:fn="http://www.w3.org/2004/10/xpath-functions" xmlns:xdt="http://www.w3.org/2004/10/xpath-datatypes">
	<xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes"/>
	
	<xsl:template match="text()"/>
	
	<xsl:template match="/">
		<elements>
			<xsl:apply-templates/>
		</elements>
	</xsl:template>
	
	<xsl:template match="Composante">
		<element comment="{@Commentaire}" name="{@nom}" id="{MosID}" codeEMAT6="{CodeEMAT6}" codeEMAT8="{CodeEMAT8}" codeLFRIL="{CodeLFRIL}" codeNNO="{CodeNNO}" protection="{Protection}" size="{Volume}" weight="{Poids}">
			<xsl:if test="DeniveleMaximum">
				<xsl:attribute name="max-slope">
					<xsl:value-of select="DeniveleMaximum"/>
				</xsl:attribute>
			</xsl:if>
			<xsl:apply-templates/>
		</element>
	</xsl:template>
	
	<xsl:template match="Vitesses">
		<speeds max="{@maximum}">
			<xsl:apply-templates/>
		</speeds>
	</xsl:template>
	
	<xsl:template match="Terrain">
		<speed terrain="{@type}" value="{.}"/>
	</xsl:template>

	<xsl:template match="Contenance">
		<composition>
			<xsl:for-each select="Dotation">
				<category><xsl:attribute name="name">
						<xsl:value-of select="@nom"/>
					</xsl:attribute>
					<xsl:for-each select="Categories/Categorie">
						<dotation normalized-consumption="{@consommationNormalisee}" capacity="{@contenance}" name="{@nom}" logistic-threshold="{@seuilLogistique}"/>
					</xsl:for-each>
				</category>			
			</xsl:for-each>
		</composition>
	</xsl:template>
	
	<xsl:template match="Armements">
		<weapon-systems>
			<xsl:for-each select="Armement">
				<weapon-system launcher="{@lanceur}" munition="{@munition}">
					<xsl:if test="@majeur">
						<xsl:attribute name="major">
							<xsl:value-of select="@majeur"/>
						</xsl:attribute>
					</xsl:if>
				</weapon-system>
			</xsl:for-each>
		</weapon-systems>
	</xsl:template>
	
	<xsl:template match="Transport">
		<transports>
			<xsl:if test="Personnel/Temps">
				<crew man-boarding-time="{Personnel/Temps/TempsEmbarquementParHomme}" man-unloading-time="{Personnel/Temps/TempsDebarquementParHomme}"/>
			</xsl:if>
			<xsl:if test="Pion">
				<unit capacity="{Pion/Capacite}" ton-loading-time="{Pion/Temps/TempsChargementParTonne}" ton-unloading-time="{Pion/Temps/TempsDechargementParTonne}"/>
			</xsl:if>
		</transports>
	</xsl:template>
	
	<xsl:template match="Senseurs">
		<sensors>
			<xsl:for-each select="Senseur">
				<sensor height="{@hauteur}" type="{.}"/>
			</xsl:for-each>
		</sensors>
	</xsl:template>
	
	<xsl:template match="Radars">
		<radars><xsl:for-each select="Radar">
				<radar type="{.}"/>
			</xsl:for-each>
		</radars>
	</xsl:template>
	
	<xsl:template match="Consommations">
		<consumptions>
			<xsl:for-each select="./*">
				<xsl:if test="Dotation/@nom">
					<consumption status="{name()}">
						<xsl:for-each select="Dotation/Categories/Categorie">
							<dotation category="{../../@nom}" name="{@nom}" value="{.}"/>
						</xsl:for-each>
					</consumption>
				</xsl:if>
			</xsl:for-each>
		</consumptions>
	</xsl:template>
	
	<xsl:template match="Objets">
		<objects>
			<xsl:for-each select="Objet">
				<object type="{@type}">
					<xsl:if test="TempsInitialConstruction">
						<xsl:attribute name="initial-construction-time">
							<xsl:value-of select="TempsInitialConstruction"/>
						</xsl:attribute>
					</xsl:if>
					<xsl:if test="TempsInitialDestruction">
						<xsl:attribute name="initial-destruction-time">
							<xsl:value-of select="TempsInitialDestruction"/>
						</xsl:attribute>
					</xsl:if>
					<xsl:if test="TempsConstructionCoef">
						<xsl:attribute name="construction-time-factor">
							<xsl:value-of select="TempsConstructionCoef"/>
						</xsl:attribute>
					</xsl:if>
					<xsl:if test="TempsDestructionCoef">
						<xsl:attribute name="destruction-time-factor">
							<xsl:value-of select="TempsDestructionCoef"/>
						</xsl:attribute>
					</xsl:if>
					<xsl:if test="TempsValorisation">
						<xsl:attribute name="valorization-time">
							<xsl:value-of select="TempsValorisation"/>
						</xsl:attribute>
					</xsl:if>
					<xsl:if test="TempsDevalorisation">
						<xsl:attribute name="devalorization-time">
							<xsl:value-of select="TempsDevalorisation"/>
						</xsl:attribute>
					</xsl:if>
					<xsl:if test="CoefGainContournement">
						<xsl:attribute name="bypass-gain-factor">
							<xsl:value-of select="CoefGainContournement"/>
						</xsl:attribute>
					</xsl:if>
					<xsl:if test="VitesseNonContourne">
						<xsl:attribute name="non-bypassed-speed">
							<xsl:value-of select="VitesseNonContourne"/>
						</xsl:attribute>
					</xsl:if>
					<xsl:if test="VitesseContourne">
						<xsl:attribute name="bypassed-speed">
							<xsl:value-of select="VitesseContourne"/>
						</xsl:attribute>
					</xsl:if>
					<xsl:if test="ModeConsommation">
						<xsl:attribute name="consumption-mode">
							<xsl:value-of select="ModeConsommation"/>
						</xsl:attribute>
					</xsl:if>
				</object>
			</xsl:for-each>
		</objects>
	</xsl:template>
	
	<xsl:template match="FonctionsLogistiques">
		<logistic-functions>
			<xsl:if test="Maintenance">
				<maintenance-functions>
					<xsl:if test="Maintenance/Remorqueur">
						<towing capacity="{Maintenance/Remorqueur/Capacite}" loading-time="{Maintenance/Remorqueur/TempsChargement}" unloading-time="{Maintenance/Remorqueur/TempsDechargement}"/>
					</xsl:if>
					<xsl:for-each select="./Maintenance/*">	
						<xsl:if test="name() != 'Remorqueur'">
							<repairing category="{name()}">
								<xsl:if test="@tempsMaxReparation">
									<xsl:attribute name="max-reparation-time">
										<xsl:value-of select="@tempsMaxReparation"/>
									</xsl:attribute>
								</xsl:if>
								<xsl:variable name="breakdownTypes">
									<xsl:for-each select="*">
										<xsl:if test="position() > 1">
											<xsl:text>,</xsl:text>
										</xsl:if>
										<xsl:value-of select="name()"/>
									</xsl:for-each>
								</xsl:variable>
								<xsl:attribute name="type">
									<xsl:value-of select="$breakdownTypes"/>
								</xsl:attribute>
							</repairing>
						</xsl:if>
					</xsl:for-each>
				</maintenance-functions>
			</xsl:if>
			<xsl:if test="Sante">
				<health-functions>
					<xsl:if test="Sante/AmbulanceReleve">
						<relieving capacity="{Sante/AmbulanceReleve/Capacite}" man-loading-time="{Sante/AmbulanceReleve/TempsChargementParHumain}" man-unloading-time="{Sante/AmbulanceReleve/TempsDechargementParHumain}"><xsl:if test="Sante/AmbulanceReleve/TransportBlessures">
								<xsl:attribute name="wounded-transport">
									<xsl:for-each select="Sante/AmbulanceReleve/TransportBlessures/*">
										<xsl:value-of select="name()"/>
										<xsl:text>,</xsl:text>
									</xsl:for-each>
								</xsl:attribute>
							</xsl:if>
							<xsl:if test="Sante/AmbulanceReleve/TransportNBC">
								<xsl:attribute name="nbc-transport">
									<xsl:text>true</xsl:text>
								</xsl:attribute>
							</xsl:if>
							<xsl:if test="Sante/AmbulanceReleve/TransportReacMental">
								<xsl:attribute name="reac-mental-transport">
									<xsl:text>true</xsl:text>
								</xsl:attribute>
							</xsl:if>
						</relieving>
					</xsl:if>
					<xsl:if test="Sante/AmbulanceRamassage">
						<collecting capacity="{Sante/AmbulanceRamassage/Capacite}" man-loading-time="{Sante/AmbulanceRamassage/TempsChargementParHumain}" man-unloading-time="{Sante/AmbulanceRamassage/TempsDechargementParHumain}"><xsl:if test="Sante/AmbulanceRamassage/TransportBlessures">
								<xsl:attribute name="wounded-transport">
									<xsl:for-each select="Sante/AmbulanceRamassage/TransportBlessures/*">
										<xsl:value-of select="name()"/>
										<xsl:text>,</xsl:text>
									</xsl:for-each>
								</xsl:attribute>
							</xsl:if>
							<xsl:if test="Sante/AmbulanceRamassage/TransportNBC">
								<xsl:attribute name="nbc-transport">
									<xsl:text>true</xsl:text>
								</xsl:attribute>
							</xsl:if>
							<xsl:if test="Sante/AmbulanceRamassage/TransportReacMental">
								<xsl:attribute name="reac-mental-transport">
									<xsl:text>true</xsl:text>
								</xsl:attribute>
							</xsl:if>
						</collecting>
					</xsl:if>
					<xsl:if test="Sante/Medecin">
						<caring><xsl:if test="Sante/Medecin/Tri">
								<xsl:attribute name="sorting">
									<xsl:text>true</xsl:text>
								</xsl:attribute>
							</xsl:if>
							<xsl:if test="Sante/Medecin/SoinNBC">
								<xsl:attribute name="nbc">
									<xsl:text>true</xsl:text>
								</xsl:attribute>
							</xsl:if>
							<xsl:if test="Sante/Medecin/SoinBlessures">
								<xsl:attribute name="caring">
									<xsl:for-each select="Sante/Medecin/SoinBlessures/*">
										<xsl:value-of select="name()"/>
										<xsl:text>,</xsl:text>
									</xsl:for-each>
								</xsl:attribute>
							</xsl:if>
							<xsl:if test="Sante/Medecin/SoinReacMental">
								<xsl:attribute name="psychiatry">
									<xsl:text>true</xsl:text>
								</xsl:attribute>
							</xsl:if>
						</caring>
					</xsl:if>
				</health-functions>
			</xsl:if>
			<xsl:if test="Ravitaillement">
				<supply-functions>
					<xsl:for-each select="Ravitaillement/Transporteur">
						<carrying mass="{Capacite/Masse}" volume="{Capacite/Volume}" nature="{NatureTransportee}"/>
					</xsl:for-each>
				</supply-functions>
			</xsl:if>
		</logistic-functions>
	</xsl:template>

	<xsl:template match="Pannes">
		<breakdowns>
			<xsl:for-each select="PannesAleatoires/Panne">
				<breakdown origin="random" type="{@nom}" percentage="{@pourcentage}"/>
			</xsl:for-each>
			<xsl:for-each select="PannesAttritions/Panne">
				<breakdown origin="attrition" type="{@nom}" percentage="{@pourcentage}"/>
			</xsl:for-each>
		</breakdowns>
	</xsl:template>
	
	
	
	
	
	
	
	
	
	
	
	
	
</xsl:stylesheet>
