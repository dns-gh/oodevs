<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fn="http://exslt.org/sets">
  <xsl:output method="xml" indent="yes" encoding="UTF-8"/>

  <xsl:template match="node()|@*">
    <xsl:copy>
      <xsl:apply-templates select="node()|@*"/>
    </xsl:copy>
  </xsl:template>

  <xsl:template match="humans-factors">
    <xsl:copy>
      <xsl:apply-templates select="node()|@*"/>
      <xsl:element name="automatic-evolution">
        <xsl:element name="tiredness-evolution">
          <xsl:attribute name="first-threshold">100</xsl:attribute>
          <xsl:attribute name="second-threshold">200</xsl:attribute>
          <xsl:attribute name="engine-running">0</xsl:attribute>
          <xsl:attribute name="engine-stopped">-10</xsl:attribute>
          <xsl:attribute name="moving">10</xsl:attribute>
          <xsl:attribute name="working">20</xsl:attribute>
        </xsl:element>
        <xsl:element name="stress-evolution">
          <xsl:attribute name="first-threshold">10</xsl:attribute>
          <xsl:attribute name="second-threshold">50</xsl:attribute>
          <xsl:attribute name="inc-per-shot">10</xsl:attribute>
          <xsl:attribute name="dec-per-hour">-20</xsl:attribute>
        </xsl:element>
      </xsl:element>
    </xsl:copy>
    </xsl:template>
    
</xsl:stylesheet>
    