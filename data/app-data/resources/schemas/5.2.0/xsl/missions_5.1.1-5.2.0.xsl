<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fn="http://exslt.org/sets">
  <xsl:output method="xml" indent="yes" encoding="UTF-8"/>

  <!-- matches descendant-or-self:node() -->
  <xsl:template match="parameter">
      <xsl:copy>
          <xsl:apply-templates select="node()|@*"/>
          <xsl:if test="./@dia-name = 'dangerDirection_'">
              <xsl:attribute name="is-context">true</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@dia-name = 'phaseLines_'">
              <xsl:attribute name="is-context">true</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@dia-name = 'boundaryLimit1_'">
              <xsl:attribute name="is-context">true</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@dia-name = 'boundaryLimit2_'">
              <xsl:attribute name="is-context">true</xsl:attribute>
          </xsl:if>
      </xsl:copy>
  </xsl:template>

  <xsl:template match="node()|@*">
    <xsl:copy>
      <xsl:apply-templates select="node()|@*"/>
    </xsl:copy>
  </xsl:template>
  
</xsl:stylesheet>
