<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fn="http://exslt.org/sets">
  <xsl:output method="xml" indent="yes" encoding="UTF-8"/>

  <!-- matches descendant-or-self:node() -->
  <xsl:template match="indirect-fire">
      <xsl:copy>
          <xsl:apply-templates select="text()|@*"/>
          <xsl:if test="./@type = 'mine'">
              <xsl:attribute name="type">effect</xsl:attribute>
              <xsl:attribute name="object-type">mined area (scattered)</xsl:attribute>
              <xsl:attribute name="life-time">0s</xsl:attribute>
          </xsl:if>
      </xsl:copy>
  </xsl:template>

  <xsl:template match="node()|@*">
    <xsl:copy>
      <xsl:apply-templates select="node()|@*"/>
    </xsl:copy>
  </xsl:template>
</xsl:stylesheet>
