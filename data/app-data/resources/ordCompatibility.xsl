<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:output method="xml" indent="yes" encoding="UTF-8"/>
    
   
 <xsl:template match="parameter[@type='intelligence']"/>
 <xsl:template match="parameter[@type='Intelligence']"/>
 <xsl:template match="parameter[@type='IntelligenceList']"/>

 <xsl:template match="node()|@*">
        <xsl:copy>
            <xsl:apply-templates select="node()|@*"/>
        </xsl:copy>
    </xsl:template>
   
    <xsl:template match="@type">
        <xsl:choose>
            <xsl:when test=".='lima'"><xsl:attribute name="type">phaseline</xsl:attribute></xsl:when>
            <xsl:when test=".='Direction'"><xsl:attribute name="type">Heading</xsl:attribute></xsl:when>
            <xsl:when test=".='direction'"><xsl:attribute name="type">Heading</xsl:attribute></xsl:when>
            <xsl:when test=".='PhaseLineList'"><xsl:attribute name="type">PhaseLine</xsl:attribute></xsl:when>
            <xsl:when test=".='IntelligenceList'"><xsl:attribute name="type">Intelligence</xsl:attribute></xsl:when>
            <xsl:when test=".='Bool'"><xsl:attribute name="type">Boolean</xsl:attribute></xsl:when>
            <xsl:when test=".='AutomateBM' or .='Automate'"><xsl:attribute name="type">Automat</xsl:attribute></xsl:when>
            <xsl:when test=".='PopulationKnowledge'"><xsl:attribute name="type">CrowdKnowledge</xsl:attribute></xsl:when>
            <xsl:when test=".='DotationType'"><xsl:attribute name="type">ResourceType</xsl:attribute></xsl:when>
            <xsl:when test=".='genobject' or .='genobjectBM'"><xsl:attribute name="type">Plannedwork</xsl:attribute></xsl:when>
            <xsl:when test=".='obstacle'"><xsl:attribute name="type">Plannedwork</xsl:attribute></xsl:when>
            <xsl:when test=".='UrbanBlock' or .='UrbanBlockBM'"><xsl:attribute name="type">UrbanKnowledge</xsl:attribute></xsl:when>
            <xsl:when test=".='Numeric'"><xsl:attribute name="type">Integer</xsl:attribute></xsl:when>
            <xsl:when test="contains(.,'AreaBM')"><xsl:attribute name="type">Polygon</xsl:attribute></xsl:when>
            <xsl:when test="contains(.,'areabm')"><xsl:attribute name="type">Polygon</xsl:attribute></xsl:when>
            <xsl:when test="contains(.,'ListBM')"><xsl:attribute name="type"><xsl:value-of select="substring-before(.,'ListBM')"/></xsl:attribute></xsl:when>
            <xsl:when test="contains(.,'listbm')"><xsl:attribute name="type"><xsl:value-of select="substring-before(.,'listbm')"/></xsl:attribute></xsl:when>
            <xsl:when test="contains(.,'List')"><xsl:attribute name="type"><xsl:value-of select="substring-before(.,'List')"/></xsl:attribute></xsl:when>
            <xsl:when test=".='list'"><xsl:attribute name="type">list</xsl:attribute></xsl:when>
            <xsl:when test="contains(.,'list')"><xsl:attribute name="type"><xsl:value-of select="substring-before(.,'list')"/></xsl:attribute></xsl:when>
            <xsl:when test="contains(.,'BM')"><xsl:attribute name="type"><xsl:value-of select="substring-before(.,'BM')"/></xsl:attribute></xsl:when>
            <xsl:when test="contains(.,'bm')"><xsl:attribute name="type"><xsl:value-of select="substring-before(.,'bm')"/></xsl:attribute></xsl:when>
            <xsl:otherwise><xsl:copy></xsl:copy></xsl:otherwise>
        </xsl:choose>
    </xsl:template>
    
</xsl:stylesheet>
