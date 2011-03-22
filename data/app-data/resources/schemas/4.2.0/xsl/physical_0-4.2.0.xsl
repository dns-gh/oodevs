<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fn="http://exslt.org/sets">
	<xsl:output method="xml" indent="yes" encoding="UTF-8"/>
	
    <!-- matches descendant-or-self:node() -->    
    <xsl:template match="//*">
        <xsl:copy>
            <xsl:copy-of select="./*|@*"/>
            <xsl:if test="count( ./resource-networks ) = 0">
                <xsl:element name="resource-networks">
                    <xsl:attribute name="file">ResourceNetworks.xml</xsl:attribute>
                </xsl:element>
            </xsl:if>
        </xsl:copy>
    </xsl:template>
</xsl:stylesheet>
