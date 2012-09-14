<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:output method="xml" indent="yes" encoding="UTF-8"/>

  <xsl:template match="category[@hidden='true']">
      <xsl:copy>
          <xsl:if test="count( ./@hidden ) = 1">
              <xsl:if test="./@name = 'Taches' or ./@name = 'Tasks'">
                    <xsl:attribute name="id">tasks</xsl:attribute>
                    <xsl:attribute name="name"><xsl:value-of select="./@name"/></xsl:attribute>
                    <xsl:copy-of select="description"/>
                    <xsl:copy-of select="template"/>
              </xsl:if>
              <xsl:if test="./@name = 'Objets tactiques graphiques' or ./@name = 'Objects tactiques graphiques' or ./@name = 'Tactical graphics'">
                    <xsl:attribute name="id">graphics</xsl:attribute>
                    <xsl:attribute name="name"><xsl:value-of select="./@name"/></xsl:attribute>
                    <xsl:copy-of select="description"/>
                    <xsl:copy-of select="template"/>
              </xsl:if>
          </xsl:if>
      </xsl:copy>
  </xsl:template>

  <xsl:template match="node()|@*">
    <xsl:copy>
      <xsl:apply-templates select="node()|@*"/>
    </xsl:copy>
  </xsl:template>
  
</xsl:stylesheet>
