<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fn="http://exslt.org/sets">
  <xsl:output method="xml" indent="yes" encoding="UTF-8"/>
    <xsl:template match="node()|@*">
      <xsl:copy>
        <xsl:apply-templates select="node()|@*"/>
      </xsl:copy>
    </xsl:template>

    <xsl:template match="resource">
      <xsl:copy>
          <xsl:apply-templates select="text()|@*"/>
          <xsl:attribute name="low-threshold"><xsl:value-of select="@logistic-threshold"/></xsl:attribute>
          <xsl:attribute name="high-threshold">100</xsl:attribute>
      </xsl:copy>
    </xsl:template>
</xsl:stylesheet>
