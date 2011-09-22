<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fn="http://exslt.org/sets">
  <xsl:output method="xml" indent="yes" encoding="UTF-8"/>    
  <xsl:template match="/physical">
      <xsl:copy>
          <xsl:apply-templates select="node()|@*"/>
          <xsl:if test="count( ./urbanTemplates ) = 0">
              <xsl:element name="urbanTemplates">
                  <xsl:attribute name="file">UrbanTemplates.xml</xsl:attribute>
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
