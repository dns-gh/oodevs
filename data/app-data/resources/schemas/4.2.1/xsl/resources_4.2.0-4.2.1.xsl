<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fn="http://exslt.org/sets">
	<xsl:output method="xml" indent="yes" encoding="UTF-8"/>
	<xsl:template match="node()|@*">
		<xsl:copy>
			<xsl:apply-templates select="node()|@*"/>
		</xsl:copy>
	</xsl:template>
	
	<xsl:template match="dotations">
		<xsl:element name="resources">
            <xsl:apply-templates select="node()"/>
            <!-- Xalan is bugged, and copy the root attributes (xsi..) to childs -->
		</xsl:element>
	</xsl:template>
	
	<xsl:template match="dotation">
		<xsl:element name="resource">
            <xsl:apply-templates select="node()|@*"/>
		</xsl:element>
	</xsl:template>
	
	<xsl:template match="dotation/@category">
		<xsl:attribute name="name">
            <xsl:value-of select="."/>
		</xsl:attribute>
	</xsl:template>
	
	<xsl:template match="dotation/@name">
		<xsl:attribute name="category">
            <xsl:value-of select="."/>
		</xsl:attribute>
	</xsl:template>
	
	<xsl:template match="urbanModifiers">
		<xsl:element name="urban-modifiers">
            <xsl:apply-templates select="node()|@*"/>
		</xsl:element>
	</xsl:template>	
	
	<xsl:template match="urbanModifier">
		<xsl:element name="urban-modifier">
            <xsl:apply-templates select="node()|@*"/>
		</xsl:element>
	</xsl:template>	
</xsl:stylesheet>
