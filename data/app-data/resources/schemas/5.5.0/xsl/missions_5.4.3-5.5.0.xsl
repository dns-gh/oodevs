<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fn="http://exslt.org/sets">
    <xsl:output method="xml" indent="yes" encoding="UTF-8"/>

  <xsl:template match="parameter">
    <xsl:copy>
      <xsl:apply-templates select="node()|@*"/>
      <xsl:if test="(./@type = 'ObjectKnowledge' or ./@type ='PlannedWork') and not(objects)">
        <xsl:element name="objects">
          <xsl:attribute name="all">true</xsl:attribute>
        </xsl:element>
      </xsl:if>
      <xsl:if test="./@type = 'PhaseLine' and not(objects)">
        <xsl:element name="objects"/>
      </xsl:if>
    </xsl:copy>
  </xsl:template>

    <xsl:template match="node()|@*">
        <xsl:copy>
            <xsl:apply-templates select="node()|@*"/>
        </xsl:copy>
    </xsl:template>
</xsl:stylesheet>
