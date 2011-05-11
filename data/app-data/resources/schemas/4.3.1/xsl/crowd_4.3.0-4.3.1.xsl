<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:output method="xml" indent="yes" encoding="UTF-8"/>
    
    <xsl:template match="unit-fire-effects/unit">
		<xsl:choose>
			<xsl:when test="@rule-of-engagment='dispersion par moyens de defense actifs'"></xsl:when>
			<xsl:otherwise><xsl:copy><xsl:apply-templates select="node()|@*"/></xsl:copy></xsl:otherwise>	
		</xsl:choose>
	</xsl:template>
    
	<xsl:template match="node()|@*">
		<xsl:copy>
			<xsl:apply-templates select="node()|@*"/>
		</xsl:copy>
	</xsl:template>
</xsl:stylesheet>
