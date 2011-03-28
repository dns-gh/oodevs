<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fn="http://exslt.org/sets">
	<xsl:output method="xml" indent="yes" encoding="UTF-8"/>
	<xsl:template match="decisional">
		<xsl:copy>
			<xsl:element name="critical-intelligence-delay">
				<xsl:attribute name="maximum-affinity">30m</xsl:attribute>
				<xsl:attribute name="minimum-affinity">2h</xsl:attribute>
				<xsl:attribute name="neutral-affinity">1h</xsl:attribute>
			</xsl:element>
			<xsl:apply-templates select="node()|@*"/>
		</xsl:copy>
	</xsl:template>
	<xsl:template match="node()|@*">
		<xsl:copy>
			<xsl:apply-templates select="node()|@*"/>
		</xsl:copy>
	</xsl:template>
</xsl:stylesheet>
