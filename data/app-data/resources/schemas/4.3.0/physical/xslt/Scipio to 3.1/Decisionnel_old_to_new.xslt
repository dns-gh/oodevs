<?xml version="1.0" encoding="UTF-8"?>
<xsl:transform  version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:fn="http://www.w3.org/2004/10/xpath-functions" xmlns:xdt="http://www.w3.org/2004/10/xpath-datatypes">

	<xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes"/>
	
	<xsl:template match="text()"/>
	
	<xsl:template match="/">
		<decisonal>
			<xsl:apply-templates/>
		</decisonal>
	</xsl:template>
	
	<xsl:template match="Dangerosite">
		<dangerosity-modifiers max-accuracy="{DegradationMaxParPertinence}" max-operational-state="{DegradationMaxParEtatOps}" max-neutralized-state="{DegradationMaxParEtatNeutralise}"/>
	</xsl:template>
	
	<xsl:template match="EtatOps">
		<operational-state-weights component="{PoidsComposantesNonMajeures}" major-component="{PoidsComposantesMajeures}" crew="{PoidsPersonnel}"/>
	</xsl:template>
	
	<xsl:template match="RapportDeForce">
		<force-ratio default-feedback-time="{TempsDeRemonteeParDefaut}"/>
	</xsl:template>
	
	
	
</xsl:transform>
