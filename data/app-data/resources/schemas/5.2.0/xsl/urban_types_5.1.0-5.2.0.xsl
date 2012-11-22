<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fn="http://exslt.org/sets">
  <xsl:output method="xml" indent="yes" encoding="UTF-8"/>

  <!-- matches descendant-or-self:node() -->
  <xsl:template match="infrastructure">
      <xsl:copy>
          <xsl:if test="count( ./capacities/medical ) > 0">
              <xsl:attribute name="medical">true</xsl:attribute>
          </xsl:if>
          <xsl:apply-templates select="text()|@*"/>
      </xsl:copy>
  </xsl:template>

  <xsl:template match="facade-types"/>

  <xsl:template match="node()|@*">
    <xsl:copy>
      <xsl:apply-templates select="node()|@*"/>
    </xsl:copy>
  </xsl:template>
</xsl:stylesheet>
