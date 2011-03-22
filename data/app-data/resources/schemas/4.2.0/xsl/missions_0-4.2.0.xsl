<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fn="http://exslt.org/sets">
	<xsl:output method="xml" indent="yes" encoding="UTF-8"/>
	<xsl:template match="node()|@*">
		<xsl:copy>
			<xsl:apply-templates select="node()|@*"/>
		</xsl:copy>
	</xsl:template>
	
	<!-- Delete node context -->
	<xsl:template match="context">
    </xsl:template>
    
	<!--<xsl:template match="populations">
                <xsl:copy-of select="."/>
	</xsl:template>-->
	    
	<!-- Duplicate old context into each mission, if context existed ... -->	    
    <xsl:template match="mission/@name">
        <xsl:attribute name="name">
            <xsl:value-of select="."/>
        </xsl:attribute>        
        <xsl:if test="count(../../context) > 0">
            <xsl:element name="parameter">
                <xsl:attribute name="dia-name">dangerDirection_</xsl:attribute>
                <xsl:attribute name="name">direction dangereuse</xsl:attribute>
                <xsl:attribute name="optional">false</xsl:attribute>
                <xsl:attribute name="type">Heading</xsl:attribute>
            </xsl:element>
            <xsl:element name="parameter">
                <xsl:attribute name="dia-name">phaseLines_</xsl:attribute>
                <xsl:attribute name="name">Limas</xsl:attribute>
                <xsl:attribute name="optional">true</xsl:attribute>
                <xsl:attribute name="max-occurs">unbounded</xsl:attribute>
                <xsl:attribute name="min-occurs">1</xsl:attribute>
                <xsl:attribute name="type">PhaseLine</xsl:attribute>
            </xsl:element>
            <xsl:element name="parameter">
                <xsl:attribute name="dia-name">boundaryLimit1_</xsl:attribute>
                <xsl:attribute name="name">Limit 1</xsl:attribute>
                <xsl:attribute name="optional">true</xsl:attribute>
                <xsl:attribute name="type">Limit</xsl:attribute>
            </xsl:element>
            <xsl:element name="parameter">
                <xsl:attribute name="dia-name">boundaryLimit2_</xsl:attribute>
                <xsl:attribute name="name">Limit 2</xsl:attribute>
                <xsl:attribute name="optional">true</xsl:attribute>
                <xsl:attribute name="type">Limit</xsl:attribute>
            </xsl:element>
            <xsl:element name="parameter">
                <xsl:attribute name="dia-name">intelligences_</xsl:attribute>
                <xsl:attribute name="name">Renseignements</xsl:attribute>
                <xsl:attribute name="optional">true</xsl:attribute>
                <xsl:attribute name="max-occurs">unbounded</xsl:attribute>
                <xsl:attribute name="min-occurs">1</xsl:attribute>
                <xsl:attribute name="type">Intelligence</xsl:attribute>
            </xsl:element>
        </xsl:if>
    </xsl:template>

    <xsl:template match="@type">
        <xsl:choose>
            <xsl:when test=".='AreaListBM'">
                <xsl:attribute name="max-occurs">unbounded</xsl:attribute>
                <xsl:attribute name="min-occurs">1</xsl:attribute>
                <xsl:attribute name="type">Polygon</xsl:attribute>
            </xsl:when>
            <xsl:when test="contains(.,'List')">
                <xsl:attribute name="max-occurs">unbounded</xsl:attribute>
                <xsl:attribute name="min-occurs">1</xsl:attribute>
                <xsl:attribute name="type">
                    <xsl:value-of select="substring-before(.,'List')"/>
                </xsl:attribute>
            </xsl:when>
            <xsl:when test="contains(.,'list')">
                <xsl:attribute name="max-occurs">unbounded</xsl:attribute>
                <xsl:attribute name="min-occurs">1</xsl:attribute>
                <xsl:attribute name="type">
                    <xsl:value-of select="substring-before(.,'list')"/>
                </xsl:attribute>
            </xsl:when>            
            <xsl:when test=".='Direction'"><xsl:attribute name="type">Heading</xsl:attribute></xsl:when>            
            <xsl:when test=".='Direction'"><xsl:attribute name="type">Heading</xsl:attribute></xsl:when>            
            <xsl:when test=".='DirectionBM'"><xsl:attribute name="type">Heading</xsl:attribute></xsl:when>            
            <xsl:when test=".='UrbanBlockBM'"><xsl:attribute name="type">UrbanKnowledge</xsl:attribute></xsl:when>            
            <xsl:when test="contains(.,'AreaBM')"><xsl:attribute name="type">Polygon</xsl:attribute></xsl:when>
            <xsl:when test="contains(.,'BM')"><xsl:attribute name="type"><xsl:value-of select="substring-before(.,'BM')"/></xsl:attribute></xsl:when>
            <xsl:otherwise>
                <xsl:attribute name="type">
                    <xsl:value-of select="."/>
                </xsl:attribute>
            </xsl:otherwise>
        </xsl:choose>
    </xsl:template>

</xsl:stylesheet>
