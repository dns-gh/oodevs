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
            <xsl:when test=".='Direction' or .='direction'"><xsl:attribute name="type">heading</xsl:attribute></xsl:when>
            <xsl:when test=".='PhaseLineList' or .='phaselinelist'"><xsl:attribute name="type">phaseLine</xsl:attribute></xsl:when>
            <xsl:when test=".='IntelligenceList' or .='intelligencelist'"><xsl:attribute name="type">intelligence</xsl:attribute></xsl:when>
            <xsl:when test=".='Bool' or .='bool'"><xsl:attribute name="type">boolean</xsl:attribute></xsl:when>
            <xsl:when test=".='AutomateBM' or .='Automate' or .='automatebm' or .='automate'"><xsl:attribute name="type">automat</xsl:attribute></xsl:when>
            <xsl:when test=".='PopulationKnowledge' or .='populationknowledge'"><xsl:attribute name="type">crowdKnowledge</xsl:attribute></xsl:when>
            <xsl:when test=".='DotationType' or .='dotationtype'"><xsl:attribute name="type">resourceType</xsl:attribute></xsl:when>
            <xsl:when test=".='genobject' or .='GenObjectList' or .='genobjectBM' or .='genobjectbm'"><xsl:attribute name="type">plannedwork</xsl:attribute></xsl:when>
            <xsl:when test=".='obstacle'"><xsl:attribute name="type">plannedwork</xsl:attribute></xsl:when>
            <xsl:when test=".='UrbanBlock' or .='UrbanBlockBM' or .='urbanblock' or .='urbanblockbm'"><xsl:attribute name="type">urbanKnowledge</xsl:attribute></xsl:when>
            <xsl:when test="contains(.,'AreaBM')"><xsl:attribute name="type">polygon</xsl:attribute></xsl:when>
            <xsl:when test="contains(.,'areabm')"><xsl:attribute name="type">polygon</xsl:attribute></xsl:when>
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
