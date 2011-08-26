<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:output method="xml" indent="yes" encoding="UTF-8"/>
    
  <xsl:template match="node()|@*">
    <xsl:copy>
      <xsl:apply-templates select="node()|@*"/>
    </xsl:copy>
  </xsl:template>

  <xsl:template match="/weather/theater">
      <xsl:copy>
          <xsl:apply-templates select="node()|@*"/>
          <xsl:if test="count( ./temperature ) = 0">
              <xsl:element name="temperature">
                  <xsl:attribute name="value">20</xsl:attribute>
              </xsl:element>
          </xsl:if>
      </xsl:copy>
  </xsl:template>
  
  <xsl:template match="/weather/local-weather">
    <xsl:copy>
      <xsl:for-each select="./local">
        <xsl:copy>
          <xsl:apply-templates select="node()|@*"/>
          <xsl:if test="count( ./temperature ) = 0">
            <xsl:element name="temperature">
              <xsl:attribute name="value">20</xsl:attribute>
            </xsl:element>
          </xsl:if>
        </xsl:copy>
      </xsl:for-each>
    </xsl:copy>
  </xsl:template> 

</xsl:stylesheet>