<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:output method="xml" indent="yes" encoding="UTF-8"/>

 <xsl:template match="node()|@*">
        <xsl:copy>
            <xsl:apply-templates select="node()|@*"/>
        </xsl:copy>
    </xsl:template>

  <xsl:template match="populations">
    <xsl:copy>
      <xsl:apply-templates select="node()|@*"/>
      <xsl:choose>
        <xsl:when test="not(/time-between-nbc-applications)">
          <xsl:element name="time-between-nbc-applications">1h</xsl:element>
        </xsl:when>
        <xsl:otherwise>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:copy> 
  </xsl:template>

  <xsl:template match="population">
    <xsl:copy>
      <xsl:choose>
        <xsl:when test="not(@armed-individuals)">
          <xsl:attribute name="armed-individuals">0</xsl:attribute>
        </xsl:when>
        <xsl:otherwise>
        </xsl:otherwise>
      </xsl:choose>
      
      <xsl:apply-templates select="node()|@*"/>
      
      <xsl:choose>
        <xsl:when test="not(/repartition)">
          <xsl:element name="repartition">
            <xsl:attribute name="children">0</xsl:attribute>
            <xsl:attribute name="female">0</xsl:attribute>
            <xsl:attribute name="male">1</xsl:attribute>
          </xsl:element>
        </xsl:when>
        <xsl:otherwise>
        </xsl:otherwise>
      </xsl:choose>
     
    </xsl:copy>
  </xsl:template>
    
</xsl:stylesheet>
