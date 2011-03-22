<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:fn="http://www.w3.org/2004/10/xpath-functions" xmlns:xdt="http://www.w3.org/2004/10/xpath-datatypes">
	<xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes"/>
	
	<xsl:template match="/">
		<knowledge-groups>
			<xsl:apply-templates/>
		</knowledge-groups>
	</xsl:template>
	
	<xsl:template match="GroupeConnaissance">
		<knowledge-group name="{@nom}">
			<xsl:apply-templates/>
		</knowledge-group>
	</xsl:template>
	
	<xsl:template match="ConnaissancesAgent">
		<unit-knowledge max-lifetime="{DureeDeVieMax}" max-unit-to-knowledge-distance="{DistanceMaxEntreUniteReelleEtUniteConnue}">
			<xsl:if test="TempsInterpolation">
				<xsl:attribute name="interpolation-time">
					<xsl:value-of select="TempsInterpolation"/>
				</xsl:attribute>
			</xsl:if>
		</unit-knowledge>
	</xsl:template>
	
	<xsl:template match="ConnaissancesPopulation">
		<population-knowledge max-lifetime="{DureeDeVieMax}"/>
	</xsl:template>
	
</xsl:stylesheet>
