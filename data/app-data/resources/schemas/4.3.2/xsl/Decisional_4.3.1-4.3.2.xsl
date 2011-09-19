<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:output method="xml" indent="yes" encoding="UTF-8"/>
    
  <xsl:template match="node()|@*">
    <xsl:copy>
      <xsl:apply-templates select="node()|@*"/>
    </xsl:copy>
  </xsl:template>

  <xsl:template match="/decisional/operational-state-weights">
      <xsl:copy>
          <xsl:apply-templates select="node()|@*"/>
          <xsl:attribute name="threshold">0.1</xsl:attribute>
      </xsl:copy>
  </xsl:template>
  
</xsl:stylesheet>