<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fn="http://exslt.org/sets">
	<xsl:output method="xml" indent="yes" encoding="UTF-8"/>
	<xsl:template match="decisional">
		<xsl:copy>
			<xsl:apply-templates select="node()|@*"/>
			<xsl:element name="critical-intelligence-delay">
				<xsl:attribute name="maximum-affinity">30m</xsl:attribute>
				<xsl:attribute name="minimum-affinity">2h</xsl:attribute>
				<xsl:attribute name="neutral-affinity">1h</xsl:attribute>
			</xsl:element>
			<xsl:element name="wound-effects-modificators">
				<xsl:attribute name="available">1</xsl:attribute>
				<xsl:attribute name="unavailable">0</xsl:attribute>
				<xsl:attribute name="repairable">0.5</xsl:attribute>
				<xsl:attribute name="repairing">0.8</xsl:attribute>
				<xsl:attribute name="captured">0</xsl:attribute>
			</xsl:element>
		</xsl:copy>
	</xsl:template>
	<xsl:template match="node()|@*">
		<xsl:copy>
			<xsl:apply-templates select="node()|@*"/>
		</xsl:copy>
	</xsl:template>
</xsl:stylesheet>
