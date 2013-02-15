<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fn="http://exslt.org/sets">
    <xsl:output method="xml" indent="yes" encoding="UTF-8"/>
    <xsl:template match="node()|@*">
      <xsl:copy>
        <xsl:apply-templates select="node()|@*"/>
      </xsl:copy>
    </xsl:template>

    <xsl:template match="nbc">
      <xsl:copy>
          <xsl:attribute name="suit">level5</xsl:attribute>
          <xsl:apply-templates select="text()|@*"/>
      </xsl:copy>
    </xsl:template>

    <xsl:template match="posture">
      <xsl:copy>
          <xsl:attribute name="tear-down-time">0s</xsl:attribute>
          <xsl:apply-templates select="text()|@*"/>
      </xsl:copy>
    </xsl:template>
</xsl:stylesheet>
