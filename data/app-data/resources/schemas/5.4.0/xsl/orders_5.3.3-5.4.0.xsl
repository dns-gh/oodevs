<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fn="http://exslt.org/sets">

	<xsl:template match="parameter[@name='Threshold']">
        <xsl:copy>
            <xsl:apply-templates select="node()"/>
            <xsl:attribute name="name">Low threshold</xsl:attribute>            
            <xsl:apply-templates select="@type"/>
            <xsl:apply-templates select="@value"/>
        </xsl:copy>
        <parameter name="High threshold" type="numeric" value="100"/>
    </xsl:template>
    
 	<xsl:template match="node()|@*">
        <xsl:copy>
            <xsl:apply-templates select="node()|@*"/>
        </xsl:copy>
    </xsl:template>

</xsl:stylesheet>
