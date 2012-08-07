<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fn="http://exslt.org/sets">
  <xsl:output method="xml" indent="yes" encoding="UTF-8"/>
  <!-- matches descendant-or-self:node() -->    
  <xsl:template match="/physical">
      <xsl:copy>
          <xsl:apply-templates select="node()|@*"/>
          <xsl:if test="count( ./units-mission-sheets-directory ) = 0">
              <xsl:element name="units-mission-sheets-directory">
                  <xsl:attribute name="path">Mission sheets/units</xsl:attribute>
              </xsl:element>
          </xsl:if>
          <xsl:if test="count( ./units-mission-sheets-directory ) = 0">
              <xsl:element name="automata-mission-sheets-directory">
                  <xsl:attribute name="path">Mission sheets/automata</xsl:attribute>
              </xsl:element>
          </xsl:if>
          <xsl:if test="count( ./units-mission-sheets-directory ) = 0">
              <xsl:element name="crowds-mission-sheets-directory">
                  <xsl:attribute name="path">Mission sheets/crowds</xsl:attribute>
              </xsl:element>
          </xsl:if>
          <xsl:if test="count( ./units-mission-sheets-directory ) = 0">
              <xsl:element name="fragorders-mission-sheets-directory">
                  <xsl:attribute name="path">Mission sheets/fragmentary orders</xsl:attribute>
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
