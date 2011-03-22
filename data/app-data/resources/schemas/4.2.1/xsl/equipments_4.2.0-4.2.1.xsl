<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fn="http://exslt.org/sets">
	<xsl:output method="xml" indent="yes" encoding="UTF-8"/>
	<xsl:template match="node()|@*">
		<xsl:copy>
			<xsl:apply-templates select="node()|@*"/>
		</xsl:copy>
	</xsl:template>
	
	<xsl:template match="elements">
		<xsl:element name="equipments">
            <xsl:apply-templates select="node()"/>
            <!-- Xalan is bugged, and copy the root attributes (xsi..) to childs -->
		</xsl:element>
	</xsl:template>
	
	<xsl:template match="element">
		<xsl:element name="equipment">
            <xsl:apply-templates select="node()|@*"/>
		</xsl:element>
	</xsl:template>
	
	<xsl:template match="dotation">
		<xsl:element name="resource">
            <xsl:apply-templates select="node()|@*"/>
		</xsl:element>
	</xsl:template>
</xsl:stylesheet>
