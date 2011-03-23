<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fn="http://exslt.org/sets">
	<xsl:output method="xml" indent="yes" encoding="UTF-8"/>
	
	<xsl:template match="node()|@*">
		<xsl:copy>
			<xsl:apply-templates select="node()|@*"/>
		</xsl:copy>
	</xsl:template>
	
	<xsl:template match="population/@humans"/>
	
    <xsl:template match="population">
        <xsl:element name="population">
            <xsl:apply-templates select="node()|@*"/>
            <xsl:if test="./@humans">
                <xsl:element name="composition">
                    <xsl:attribute name="healthy"><xsl:value-of select="./@humans"/></xsl:attribute>
                    <xsl:attribute name="contaminated">0</xsl:attribute>
                    <xsl:attribute name="dead">0</xsl:attribute>
                    <xsl:attribute name="wounded">0</xsl:attribute>
                </xsl:element>            
                </xsl:if>
		</xsl:element>
    </xsl:template>
</xsl:stylesheet>
