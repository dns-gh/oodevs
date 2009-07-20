<?xml version="1.0" encoding="UTF-8"?>
<xsl:transform version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:fn="http://www.w3.org/2004/10/xpath-functions" xmlns:xdt="http://www.w3.org/2004/10/xpath-datatypes">

	<xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes"/>
	
	<xsl:template match="text()"/>
	
	<xsl:template match="/">
		<supply>
			<xsl:apply-templates/>
		</supply>
	</xsl:template>
	
	<xsl:template match="Convois">
		<convoys unit-type="{TypeUnite/@nom}" mission="{Mission/@nom}">
			<xsl:apply-templates/>
		</convoys>
	</xsl:template>
	
	<xsl:template match="TempsConstitution">
		<constitution-times>
			<xsl:for-each select="Temps">
				<unit-time truck-count="{@nbCamions}" time="{.}"/>
			</xsl:for-each>
		</constitution-times>
	</xsl:template>
	
	<xsl:template match="TempsChargement">
		<loading-times>
			<xsl:for-each select="Temps">
				<unit-time truck-count="{@nbCamions}" time="{.}"/>
			</xsl:for-each>
		</loading-times>
	</xsl:template>
	
	<xsl:template match="TempsDechargement">
		<unloading-times>
			<xsl:for-each select="Temps">
				<unit-time truck-count="{@nbCamions}" time="{.}"/>
			</xsl:for-each>
		</unloading-times>
	</xsl:template>
	
	<xsl:template match="CoefModificationVitesse">
		<speed-modifiers>
			<xsl:for-each select="Coef">
				<speed-modifier truck-count="{@nbCamions}" value="{.}"/>
			</xsl:for-each>
		</speed-modifiers>
	</xsl:template>
	
	<xsl:template match="AlertesDisponibiliteMoyens">
		<resource-availability-alerts>
			<resource-availability-alert availability-threshold="{AlertesDisponibiliteVecteurs/AlerteDisponibiliteVecteurs/@pourcentageMoyensDisponibles}"/>
		</resource-availability-alerts>
	</xsl:template>
	
</xsl:transform>