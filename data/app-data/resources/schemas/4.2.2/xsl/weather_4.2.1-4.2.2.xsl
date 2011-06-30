<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:output method="xml" indent="yes" encoding="UTF-8"/>
    
  <xsl:template match="node()|@*">
    <xsl:copy>
      <xsl:apply-templates select="node()|@*"/>
    </xsl:copy>
  </xsl:template>

  <xsl:template match="local-weather/local">
    <xsl:element name="local">
      <xsl:attribute name="start-time"><xsl:value-of select="/weather/exercise-date/@value"/></xsl:attribute>
      <xsl:attribute name="end-time">20990101T000001</xsl:attribute>
      <xsl:apply-templates select="node()|@*"/>
    </xsl:element>
  </xsl:template>
    
</xsl:stylesheet>