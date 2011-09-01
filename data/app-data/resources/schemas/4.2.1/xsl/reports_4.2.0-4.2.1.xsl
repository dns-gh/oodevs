<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fn="http://exslt.org/sets">
    <xsl:output method="xml" indent="yes" encoding="UTF-8"/>
    <xsl:template match="node()|@*">
        <xsl:copy>
            <xsl:apply-templates select="node()|@*"/>
        </xsl:copy>
    </xsl:template>

    <xsl:template match="report">
        <xsl:element name="report">
            <xsl:attribute name="category">operational</xsl:attribute>
            <xsl:apply-templates select="node()|@*"/>
        </xsl:element>
    </xsl:template>        

    <xsl:template match="@type">
        <xsl:choose>
            <xsl:when test=".='Numeric'"><xsl:attribute name="type">Integer</xsl:attribute></xsl:when>
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
