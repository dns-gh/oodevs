<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fn="http://exslt.org/sets">
	<xsl:output method="xml" indent="yes" encoding="UTF-8"/>
	
	<xsl:template match="node()|@*">
		<xsl:copy>
			<xsl:apply-templates select="node()|@*"/>
		</xsl:copy>
	</xsl:template>

	<xsl:template match="automat/@logistic-level">
        <xsl:attribute name="logistic-level">logistic-base</xsl:attribute>
    </xsl:template>
    
    <xsl:template match="logistics/tc2">
        <xsl:element name="logistic-base">
            <xsl:apply-templates select="node()|@*"/>
		</xsl:element>
    </xsl:template>
</xsl:stylesheet>
