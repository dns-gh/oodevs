<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fn="http://exslt.org/sets">
	<xsl:output method="xml" indent="yes" encoding="UTF-8"/>
	<!-- matches descendant-or-self:node() -->    
    <xsl:template match="population">
        <xsl:copy>
            <xsl:apply-templates select="node()|@*"/>
            <xsl:if test="count( ./consumption ) = 0">
                <xsl:element name="consumption"/>
            </xsl:if>
                        
            <xsl:if test="count( ./health-need ) = 0">
                <xsl:element name="health-need">
                    <xsl:attribute name="people-per-facility">1000</xsl:attribute>
                </xsl:element>
            </xsl:if>
        </xsl:copy>
    </xsl:template>

    <xsl:template match="repartition/@male">
        <xsl:if test=". > 1">
            <xsl:attribute name="male"><xsl:value-of select=". div 100"/></xsl:attribute>
        </xsl:if>
    </xsl:template>
    <xsl:template match="repartition/@female">
        <xsl:if test=". > 1">
            <xsl:attribute name="female"><xsl:value-of select=". div 100"/></xsl:attribute>
        </xsl:if>
    </xsl:template>
    <xsl:template match="repartition/@children">
        <xsl:if test=". > 1">
            <xsl:attribute name="children"><xsl:value-of select=". div 100"/></xsl:attribute>
        </xsl:if>
    </xsl:template>    
    
	<xsl:template match="security-level">
		<xsl:element name="safety-level">
			<xsl:apply-templates select="node()|@*"/>
		</xsl:element>
	</xsl:template>
	
	<xsl:template match="node()|@*">
		<xsl:copy>
			<xsl:apply-templates select="node()|@*"/>
		</xsl:copy>
	</xsl:template>
</xsl:stylesheet>
