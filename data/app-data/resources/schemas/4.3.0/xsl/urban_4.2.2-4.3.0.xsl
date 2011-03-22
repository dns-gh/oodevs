<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fn="http://exslt.org/sets">
	<xsl:output method="xml" indent="yes" encoding="UTF-8"/>
	
	<xsl:template match="accomodations">
	    <xsl:element name="accommodations">
            <xsl:apply-templates select="node()|@*"/>
		</xsl:element>
    </xsl:template>
    
	<xsl:template match="accomodation">
	    <xsl:element name="accommodation">
	        <xsl:apply-templates select="node()|@*"/>
        </xsl:element>
    </xsl:template>
	
	<xsl:template match="accomodation/@capacity">
        <xsl:attribute name="max-capacity"><xsl:value-of select="."/></xsl:attribute>
        <xsl:attribute name="nominal-capacity"><xsl:value-of select="."/></xsl:attribute>
    </xsl:template>
    
	<xsl:template match="node()|@*">
		<xsl:copy>
			<xsl:apply-templates select="node()|@*"/>
		</xsl:copy>
	</xsl:template>
</xsl:stylesheet>
