<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fn="http://exslt.org/sets">
  <xsl:output method="xml" indent="yes" encoding="UTF-8"/>

  <xsl:template match="settings">
      <xsl:copy>
          <xsl:apply-templates select="node()|@*"/>
          <xsl:if test="count( setting[ @name = 'infinite-dotation' ] )= 0 ">
              <xsl:element name="setting">
                  <xsl:attribute name="name">infinite-dotation</xsl:attribute>
                  <xsl:attribute name="type">bool</xsl:attribute>
                  <xsl:attribute name="value">false</xsl:attribute>
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