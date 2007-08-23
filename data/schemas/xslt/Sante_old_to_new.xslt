<?xml version="1.0" encoding="UTF-8"?>
<xsl:transform version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:fn="http://www.w3.org/2004/10/xpath-functions" xmlns:xdt="http://www.w3.org/2004/10/xpath-datatypes">

	<xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes"/>
	
	<xsl:template match="text()"/>
	
	<xsl:template match="/">
		<health>
			<xsl:apply-templates/>
		</health>
	</xsl:template>
	
	<xsl:template match="TempsDiagnostique">
		<times diagnosis-time="{.}" sorting-time="{../TempsTri}"/>
	</xsl:template>
	
	<xsl:template match="CategoriesBlesse">
		<injuries>
			<xsl:for-each select="*">
				<injury category="{name()}" life-expectancy="{@esperanceVie}" percentage="{@pourcentage}" resting-time="{@tempsRepos}" caring-time="{@tempsSoin}"/>
			</xsl:for-each>
			<injury category="mental" percentage="{../ReacMental/@pourcentage}" resting-time="{../ReacMental/@tempsRepos}" caring-time="{../ReacMental/@tempsSoin}"/>
			<injury category="contaminated" resting-time="{../Contamines/@tempsRepos}" caring-time="{../Contamines/@tempsSoin}"/>
		</injuries>
	</xsl:template>
	
	<xsl:template match="AlertesDisponibiliteMoyens">
		<resource-availability-alerts>
			<xsl:for-each select="./*/*">
				<resource-availability-alert availability-threshold="{@pourcentageMoyensDisponibles}">
					<xsl:attribute name="resource">
						<xsl:if test="name()='AlerteDisponibiliteMoyensReleve'">
							<xsl:text>relieve</xsl:text>
						</xsl:if>
						<xsl:if test="name()='AlerteDisponibiliteMoyensRamassage'">
							<xsl:text>collection</xsl:text>
						</xsl:if>
						<xsl:if test="name()='AlerteDisponibiliteMedecins'">
							<xsl:text>doctor</xsl:text>
						</xsl:if>
					</xsl:attribute>
				</resource-availability-alert>
			</xsl:for-each>
		</resource-availability-alerts>
	</xsl:template>
	
	
</xsl:transform>
