<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:fn="http://www.w3.org/2004/10/xpath-functions" xmlns:xdt="http://www.w3.org/2004/10/xpath-datatypes">
	<xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes"/>
	
	<xsl:template match="/">
		<communications>
			<xsl:apply-templates/>
		</communications>
	</xsl:template>
	
	<xsl:template match="ImpactCoupureCommunications">
		<communication-breakdown>
			<xsl:attribute name="speed-modifier">
				<xsl:value-of select="CoefModificationVitesse"/>
			</xsl:attribute>
			<xsl:attribute name="reloading-time-modifier">
				<xsl:value-of select="CoefModificationTempsRechargement"/>
			</xsl:attribute>
		</communication-breakdown>
	</xsl:template>
	
</xsl:stylesheet>
