<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fn="http://exslt.org/sets">
	<xsl:param name="ns-prefix" select="'xsi'"/>
    <xsl:param name="ns-namespace" select="'http://www.w3.org/2001/XMLSchema-instance'"/>
    <xsl:template match="/root/*|@noNamespaceSchemaLocation">
        <!--<xsl:attribute name="{$ns-prefix}:noNamespaceSchemaLocation" namespace="{$ns-namespace}">-->
            <xsl:attribute name="noNamespaceSchemaLocation">
            <xsl:value-of select="'your value here'"/>
        </xsl:attribute>
    </xsl:template>
        
	<xsl:template match="@*|node()">
        <xsl:copy>
        <xsl:apply-templates select="@*|node()"/>
        </xsl:copy>
    </xsl:template>
    
  
   
    <xsl:template match="decisional">
		<xsl:element name="decisional-1.3">
			<xsl:apply-templates select="node()|@*"/>
		</xsl:element>
	</xsl:template>
</xsl:stylesheet>
