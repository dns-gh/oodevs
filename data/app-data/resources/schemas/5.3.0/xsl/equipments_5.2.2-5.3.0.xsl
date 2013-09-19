<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fn="http://exslt.org/sets">
  <xsl:output method="xml" indent="yes" encoding="UTF-8"/>
  <!-- matches descendant-or-self:node() -->

  <xsl:template match="/equipments">
    <xsl:copy>
      <xsl:apply-templates select="node()|@*"/>
      <xsl:if test="count( ./translation-queries ) = 0">
        <xsl:element name="translation-queries">
          <xsl:element name="translation-query">
            <xsl:attribute name="query">/equipments/equipment/string(@name)</xsl:attribute>
            <xsl:attribute name="context">equipments</xsl:attribute>
          </xsl:element>
          <xsl:element name="translation-query">
            <xsl:attribute name="query">/equipments/equipment/string(@comment)</xsl:attribute>
            <xsl:attribute name="context">comment</xsl:attribute>
          </xsl:element>
          <xsl:element name="translation-query">
            <xsl:attribute name="query">/equipments/equipment/operational-information/string(@native-country)</xsl:attribute>
            <xsl:attribute name="context">native-country</xsl:attribute>
          </xsl:element>
          <xsl:element name="translation-query">
            <xsl:attribute name="query">/equipments/equipment/operational-information/string(@information-origin)</xsl:attribute>
            <xsl:attribute name="context">information-origin</xsl:attribute>
          </xsl:element>
          <xsl:element name="translation-query">
            <xsl:attribute name="query">/equipments/equipment/equipment-category/string()</xsl:attribute>
            <xsl:attribute name="context">equipment-category</xsl:attribute>
          </xsl:element>          
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
