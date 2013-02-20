<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fn="http://exslt.org/sets">
  <xsl:output method="xml" indent="yes" encoding="UTF-8"/>
  <!-- matches descendant-or-self:node() -->    
  <xsl:template match="/physical">
      <xsl:copy>
          <xsl:apply-templates select="node()|@*"/>
          <xsl:if test="count( ./disasters ) = 0">
              <xsl:element name="disasters">
                  <xsl:attribute name="file">Disasters.xml</xsl:attribute>
              </xsl:element>
          </xsl:if>
          <xsl:if test="count( ./symbols-directory ) = 0">
              <xsl:element name="symbols-directory">
                  <xsl:attribute name="path">Symbols</xsl:attribute>
              </xsl:element>
          </xsl:if>
          <xsl:if test="count( ./mission-sheets-xsl ) = 0">
              <xsl:element name="mission-sheets-xsl">
                  <xsl:attribute name="file">missionSheets.xslt</xsl:attribute>
              </xsl:element>
          </xsl:if>
          <xsl:if test="count( ./stages ) = 0">
              <xsl:element name="stages">
                  <xsl:attribute name="file">Stages.xml</xsl:attribute>
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
