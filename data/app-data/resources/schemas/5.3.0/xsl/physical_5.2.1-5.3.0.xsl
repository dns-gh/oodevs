<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fn="http://exslt.org/sets">
  <xsl:output method="xml" indent="yes" encoding="UTF-8"/>
  <!-- matches descendant-or-self:node() -->    
  <xsl:template match="/physical">
      <xsl:copy>
          <xsl:apply-templates select="node()|@*"/>
          <xsl:if test="count( ./languages ) = 0">
              <xsl:element name="languages">
                  <xsl:attribute name="file">Languages.xml</xsl:attribute>
              </xsl:element>
          </xsl:if>
          <xsl:if test="count( ./locales-directory ) = 0">
              <xsl:element name="locales-directory">
                  <xsl:attribute name="path">Locales</xsl:attribute>
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
