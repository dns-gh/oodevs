<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fn="http://exslt.org/sets">
  <xsl:output method="xml" indent="yes" encoding="UTF-8"/>
  <!-- matches descendant-or-self:node() -->    
  <xsl:template match="/physical">
      <xsl:copy>
          <xsl:apply-templates select="node()|@*"/>
          <xsl:if test="count( ./funeral ) = 0">
              <xsl:element name="funeral">
                  <xsl:attribute name="file">Funeral.xml</xsl:attribute>
              </xsl:element>
          </xsl:if>
          <xsl:if test="count( ./logistic-supply-classes ) = 0">
              <xsl:element name="logistic-supply-classes">
                  <xsl:attribute name="file">LogisticSupplyClasses.xml</xsl:attribute>
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