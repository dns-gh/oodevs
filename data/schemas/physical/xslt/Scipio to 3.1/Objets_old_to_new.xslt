<?xml version="1.0" encoding="UTF-8"?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    version="1.0">

	<xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes" />
	
	<xsl:template match="text()"/>
	
	<xsl:template match="/">
		<objects>
			<xsl:apply-templates/>
		</objects>
	</xsl:template>
	
	<xsl:template match="ObjetsReels">
		<real-objects>
			<xsl:apply-templates/>
		</real-objects>
	</xsl:template>
	
	<xsl:template match="ObjetsVirtuels">
		<virtual-objects>
			<xsl:apply-templates/>
		</virtual-objects>
	</xsl:template>
	
	<xsl:template match="Objet">
		<object type="{@type}">
			<xsl:choose>
				<xsl:when test="PourcentagesTirParHumainParHectare">
					<xsl:for-each select="PourcentagesTirParHumainParHectare/Pourcentage">
						<shot-percentage-per-human-per-hectare volume="{@volume}" percentage="{.}"/>
					</xsl:for-each>
				</xsl:when>
				<xsl:otherwise>
					<xsl:attribute name="default-consumption-mode">
						<xsl:value-of select="ModeConsommationParDefaut"/>
					</xsl:attribute>
					<xsl:attribute name="dangerous">
						<xsl:value-of select="Dangereux"/>
						</xsl:attribute>
					<xsl:attribute name="can-be-maneuver-obstacle">
						<xsl:value-of select="PeutEtreObstacleDeManoeuvre"/>
					</xsl:attribute>
					<xsl:attribute name="can-be-developed">
						<xsl:value-of select="PeutEtreValorise"/>
					</xsl:attribute>
					<xsl:attribute name="can-be-bypassed">
						<xsl:value-of select="PeutEtreContourne"/>
					</xsl:attribute>
					<xsl:if test="DistanceEvitement">
						<xsl:attribute name="avoid-distance">
							<xsl:value-of select="DistanceEvitement"/>
						</xsl:attribute>
					</xsl:if>
					<xsl:attribute name="default-speed">
						<xsl:value-of select="VitesseParDefaut"/>
					</xsl:attribute>
					<xsl:attribute name="default-bypassed-speed">
						<xsl:value-of select="VitesseParDefautQuandContourne"/>
					</xsl:attribute>
					<xsl:attribute name="max-interaction-height">
						<xsl:value-of select="HauteurMaxInteraction"/>
					</xsl:attribute>
					<xsl:attribute name="max-animating-units">
						<xsl:value-of select="NombreAnimateursMax"/>
					</xsl:attribute>
					<xsl:attribute name="unit-speed-impact-mode">
						<xsl:value-of select="ModeImpactSurVitessePion/@type"/>
					</xsl:attribute>
					<xsl:if test="ModeImpactSurVitessePion/@pourcentageVitesseMaxAgent">
						<xsl:attribute name="max-unit-percentage-speed">
							<xsl:value-of select="ModeImpactSurVitessePion/@pourcentageVitesseMaxAgent"/>
						</xsl:attribute>
					</xsl:if>
					<xsl:if test="DensitePopulationSortante">
						<xsl:attribute name="population-density">
							<xsl:value-of select="DensitePopulationSortante"/>
						</xsl:attribute>
					</xsl:if>
					<xsl:apply-templates/>
				</xsl:otherwise>
			</xsl:choose>
		</object>
	</xsl:template>
	
	<xsl:template match="PlacementScores">
		<sensible-positions>
			<xsl:for-each select="Terrain">
				<sensible-position terrain="{@type}" value="{.}"/>
			</xsl:for-each>
		</sensible-positions>
	</xsl:template>
	
	<xsl:template match="Dotations">
		<dotations>
			<xsl:if test="Construction">
				<construction dotation="{Construction/@categorie}" category="{Construction/@type}" count="{Construction/@valeur}"/>
			</xsl:if>
			<xsl:if test="Valorisation">
				<valorization dotation="{Valorisation/@categorie}" category="{Valorisation/@type}" count="{Valorisation/@valeur}"/>
			</xsl:if>
		</dotations>
	</xsl:template>
	
	<xsl:template match="AttritionPopulation">
		<population-attrition surface="{SurfaceAttrition}" ph="{PH}"/>
	</xsl:template>
	
	<xsl:template match="Attritions">
		<unit-attritions>
		  <xsl:for-each select="./*">
		    <unit-attrition protection="{substring-after (name(), 'ProtectionType')}" destruction="{Destruction}" repairable-with-evacuation="{ReparableAvecEvacuation}" repairable-without-evacuation="{ReparableSansEvacuation}"/>
		  </xsl:for-each>
		</unit-attritions>
	</xsl:template>
	
</xsl:transform>
