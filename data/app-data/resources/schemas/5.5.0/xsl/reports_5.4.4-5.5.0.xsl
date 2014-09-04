<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fn="http://exslt.org/sets">
  <xsl:output method="xml" indent="yes" encoding="UTF-8"/>

  <xsl:template match="node()|@*">
    <xsl:copy>
      <xsl:apply-templates select="node()|@*"/>
    </xsl:copy>
  </xsl:template> 

  <xsl:template match="report[@key='eRC_FireObserver']">
    <xsl:copy>
      <xsl:copy-of select="@*"/>
      <parameter type="AgentKnowledge"/>
      <parameter type="String"/>
      <parameter type="Integer"/>
      <parameter type="Integer"/>
      <parameter type="Integer"/>
    </xsl:copy>
  </xsl:template>

  <xsl:template match="report[@id='259' or @id='260']">
    <xsl:copy>
      <xsl:apply-templates select="@*"/>
      <xsl:element name="parameter">
        <xsl:attribute name="type">Stage</xsl:attribute>
      </xsl:element>
    </xsl:copy>
  </xsl:template>
</xsl:stylesheet>
