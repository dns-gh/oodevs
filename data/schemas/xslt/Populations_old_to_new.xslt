<?xml version="1.0" encoding="UTF-8"?>
<xsl:transform version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:fn="http://www.w3.org/2004/10/xpath-functions" 
xmlns:xdt="http://www.w3.org/2004/10/xpath-datatypes">

	<xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes"/>
	
	<xsl:template match="text()"/>
	
	<xsl:template match="/">
		<populations>
			<xsl:apply-templates/>
		</populations>
	</xsl:template>
	
	<xsl:template match="Effets/TempsRechargement">
		<reloading-time-effect population-density="{@densitePopulation}" modifier="{@modificateur}"/>
	</xsl:template>
	
	<xsl:template match="Population">
		<population name="{@nom}" id="{MosID}" decisional-model="{ModeleDecisionnel}" concentration-density="{DensiteConcentration}" moving-base-density="{DensiteNominaleDeplacement}" moving-speed="{VitesseDeplacement}">
			<xsl:apply-templates/>
		</population>
	</xsl:template>
	
	<xsl:template match="Effets/Ralentissement/Attitudes">
		<slowing-effects>
			<xsl:for-each select="Attitude">
				<slowing-effect population-attitude="{@nom}">
					<xsl:for-each select="VolumesPions/VolumePion">
						<unit population-density="{@densitePopulation}" unit-size="{@nom}" max-speed="{@vitesseMaximale}"/>
					</xsl:for-each>
				</slowing-effect>
			</xsl:for-each>
		</slowing-effects>
	</xsl:template>
	
	<xsl:template match="Effets/Tir/Tireur/Attitudes">
		<attrition-effects>
			<xsl:for-each select="Attitude">
				<attrition-effect population-attitude="{@nom}" population-density="{Intensite/@densitePopulation}" intensity="{Intensite/@intensite}">
					<xsl:for-each select="ProtectionsPions/ProtectionPion">
						<unit protection="{@nom}" destruction="{Destruction}" repairable-with-evacuation="{ReparableAvecEvacuation}" repairable-without-evacuation="{ReparableSansEvacuation}"/>
					</xsl:for-each>
				</attrition-effect>
			</xsl:for-each>
		</attrition-effects>
	</xsl:template>
	
	<xsl:template match="Effets/Tir/Cible/ReglesEngagementTireur">
		<unit-fire-effects>
			<xsl:for-each select="RegleEngagementTireur">
				<unit rule-of-engagment="{@nom}" attrition-surface="{SurfaceAttrition}" ph="{PH}"/>
			</xsl:for-each>
		</unit-fire-effects>
	</xsl:template>

</xsl:transform>
