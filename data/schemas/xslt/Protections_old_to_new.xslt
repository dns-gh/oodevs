<?xml version="1.0" encoding="UTF-8"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    version="1.0">

	<xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes"/>
	
	<xsl:template match="text()"/>
	
	<xsl:template match="Protections">
		<protections>
			<xsl:apply-templates/>
		</protections>
	</xsl:template>
	
	<xsl:template match="Protection">
		<protection name="{@nom}" type="{@type}">
			<xsl:apply-templates/>
		</protection>
	</xsl:template>
	
	<xsl:template match="Neutralisation">
		<neutralization average-time="{@tempsMoyen}" variance="{@variance}"/>
	</xsl:template>
	
	<xsl:template match="ProbabilitePanneAleatoire">
		<random-breakdown-probability eva="{@EVA}" neva="{@NEVA}"/>
	</xsl:template>
	
	<xsl:template match="EffetsAttritionSurHumains">
		<attrition-effects>
			<xsl:for-each select="EffetAttritionSurHumains">
				<attrition-effect equipment-state="{@etatEquipement}" injured-percentage="{@pourcentageBlesses}" dead-percentage="{@pourcentageMorts}"/>
			</xsl:for-each>
		</attrition-effects>
	</xsl:template>
	
</xsl:transform>