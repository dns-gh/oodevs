<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fn="http://exslt.org/sets">
  <xsl:output method="xml" indent="yes" encoding="UTF-8"/>
  <!-- matches descendant-or-self:node() -->

  <xsl:template match="/models">
    <xsl:copy>
      <xsl:apply-templates select="node()|@*"/>
      <xsl:if test="count( ./translation-queries ) = 0">
        <xsl:element name="translation-queries">
          <xsl:element name="translation-query">
            <xsl:attribute name="query">/models/units/unit/string(@name)</xsl:attribute>
            <xsl:attribute name="context">units</xsl:attribute>
          </xsl:element>
          <xsl:element name="translation-query">
            <xsl:attribute name="query">/models/automats/automat/string(@name)</xsl:attribute>
            <xsl:attribute name="context">automats</xsl:attribute>
          </xsl:element>
          <xsl:element name="translation-query">
            <xsl:attribute name="query">/models/crowd/crowd/string(@role)</xsl:attribute>
            <xsl:attribute name="context">crowd</xsl:attribute>
          </xsl:element>
      </xsl:if>
    </xsl:copy>
  </xsl:template>

  <xsl:template match="node()|@*">
    <xsl:copy>
      <xsl:apply-templates select="node()|@*"/>
    </xsl:copy>
  </xsl:template>
</xsl:stylesheet>
