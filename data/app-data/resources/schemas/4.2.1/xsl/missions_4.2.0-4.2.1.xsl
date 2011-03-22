<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fn="http://exslt.org/sets">
	<xsl:output method="xml" indent="yes" encoding="UTF-8"/>
	<xsl:template match="node()|@*">
		<xsl:copy>
			<xsl:apply-templates select="node()|@*"/>
		</xsl:copy>
	</xsl:template>

	<!-- Parametres à virer -->
    <xsl:template match="parameter">
        <xsl:choose>
            <xsl:when test=".|@type='IntelligenceList'"></xsl:when>
            <xsl:when test=".|@type='Intelligence'"></xsl:when>
            <xsl:otherwise>
                <xsl:copy>
		            <xsl:apply-templates select="node()|@*"/>
	            </xsl:copy>
	        </xsl:otherwise>
        </xsl:choose>
    </xsl:template>
    
    <xsl:template match="parameter/@type">
        <xsl:choose>
            <xsl:when test=".='Bool'"><xsl:attribute name="type">Boolean</xsl:attribute></xsl:when>
            <xsl:when test=".='Numeric'"><xsl:attribute name="type">Integer</xsl:attribute></xsl:when>
            <xsl:when test=".='Objective'"><xsl:attribute name="type">Point</xsl:attribute></xsl:when>
            <xsl:when test=".='Location'"><xsl:attribute name="type">Polygon</xsl:attribute></xsl:when>
            <xsl:when test=".='GenObject' or .='GenObjectBM'"><xsl:attribute name="type">Plannedwork</xsl:attribute></xsl:when>
            <xsl:when test=".='Automate'"><xsl:attribute name="type">Automat</xsl:attribute></xsl:when>
            <xsl:when test=".='DotationType'"><xsl:attribute name="type">ResourceType</xsl:attribute></xsl:when>
            <xsl:when test=".='PopulationKnowledge'"><xsl:attribute name="type">CrowdKnowledge</xsl:attribute></xsl:when>
            <xsl:otherwise>
                <xsl:copy>
		            <xsl:apply-templates select="node()|@*"/>
	            </xsl:copy>
	        </xsl:otherwise>
        </xsl:choose>
    </xsl:template>
</xsl:stylesheet>
