<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fn="http://exslt.org/sets">
  <xsl:output method="xml" indent="yes" encoding="UTF-8"/>
  <xsl:template match="node()|@*">
    <xsl:copy>
      <xsl:apply-templates select="node()|@*"/>
    </xsl:copy>
  </xsl:template>
  
  <xsl:template match="carrying">
    <xsl:element name="carrying">
      <xsl:apply-templates select="node()|@*"/>
      <xsl:attribute name="min-mass">0</xsl:attribute>
      <xsl:attribute name="max-mass"><xsl:value-of select="@mass"/></xsl:attribute>
      <xsl:attribute name="min-volume">0</xsl:attribute>
      <xsl:attribute name="max-volume"><xsl:value-of select="@volume"/></xsl:attribute>
    </xsl:element>
  </xsl:template>
</xsl:stylesheet>
