<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fn="http://exslt.org/sets">
	<xsl:output method="xml" indent="yes" encoding="UTF-8"/>
    <xsl:template match="population">
        <xsl:copy>
            <xsl:if test="not( @armed-individuals )">
                <xsl:attribute name="armed-individuals">0</xsl:attribute>
            </xsl:if>
            <xsl:apply-templates select="@*"/>
            <xsl:if test="not( ./repartition )">
                <xsl:element name="repartition">
                    <xsl:attribute name="children">0</xsl:attribute>
                    <xsl:attribute name="female">0</xsl:attribute>
                    <xsl:attribute name="male">1</xsl:attribute>
                </xsl:element>
            </xsl:if>
            <xsl:apply-templates select="node()"/>
        </xsl:copy>
    </xsl:template>
	
	<xsl:template match="attrition-effect/unit">
		<xsl:element name="protection">
			<xsl:attribute name="name">
				<xsl:value-of select="@protection"/>
			</xsl:attribute>
			<xsl:element name="unarmed">
				<xsl:attribute name="destruction"><xsl:value-of select="@destruction"/></xsl:attribute>
				<xsl:attribute name="repairable-with-evacuation"><xsl:value-of select="@repairable-with-evacuation"/></xsl:attribute>
				<xsl:attribute name="repairable-without-evacuation"><xsl:value-of select="@repairable-without-evacuation"/></xsl:attribute>
			</xsl:element>
			<xsl:element name="armed">
				<xsl:attribute name="destruction"><xsl:value-of select="@destruction"/></xsl:attribute>
				<xsl:attribute name="repairable-with-evacuation"><xsl:value-of select="@repairable-with-evacuation"/></xsl:attribute>
				<xsl:attribute name="repairable-without-evacuation"><xsl:value-of select="@repairable-without-evacuation"/></xsl:attribute>
			</xsl:element>			
		</xsl:element>		
	</xsl:template>
    
	<xsl:template match="node()|@*">
		<xsl:copy>
			<xsl:apply-templates select="node()|@*"/>
		</xsl:copy>
	</xsl:template>
</xsl:stylesheet>
