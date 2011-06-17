<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fn="http://exslt.org/sets">
  <xsl:output method="xml" indent="yes" encoding="UTF-8"/>
  <xsl:template match="humans-factors">
    <xsl:copy>
      <xsl:apply-templates select="node()|@*"/>
		<xsl:element name="stress-factor">
		  <xsl:element name="modifier">
			<xsl:attribute name="loading-time">1</xsl:attribute>
			<xsl:attribute name="max-speed">1</xsl:attribute>
			<xsl:attribute name="posture-setup-time">1</xsl:attribute>
			<xsl:attribute name="sensor-distance">1</xsl:attribute>
			<xsl:attribute name="ph">1</xsl:attribute>
			<xsl:attribute name="state">Calm</xsl:attribute>
		  </xsl:element>
		  <xsl:element name="modifier">
			<xsl:attribute name="loading-time">1</xsl:attribute>
			<xsl:attribute name="max-speed">1</xsl:attribute>
			<xsl:attribute name="posture-setup-time">1</xsl:attribute>
			<xsl:attribute name="sensor-distance">1</xsl:attribute>
			<xsl:attribute name="ph">1</xsl:attribute>
			<xsl:attribute name="state">Worried</xsl:attribute>
		  </xsl:element>
		  <xsl:element name="modifier">
			<xsl:attribute name="loading-time">1</xsl:attribute>
			<xsl:attribute name="max-speed">1</xsl:attribute>
			<xsl:attribute name="posture-setup-time">1</xsl:attribute>
			<xsl:attribute name="sensor-distance">1</xsl:attribute>
			<xsl:attribute name="ph">1</xsl:attribute>
			<xsl:attribute name="state">Stressed</xsl:attribute>
		  </xsl:element>
		</xsl:element>
    </xsl:copy>
  </xsl:template>
  <xsl:template match="node()|@*">
    <xsl:copy>
      <xsl:apply-templates select="node()|@*"/>
    </xsl:copy>
  </xsl:template>
</xsl:stylesheet>
