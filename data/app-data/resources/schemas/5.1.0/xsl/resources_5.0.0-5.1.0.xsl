<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fn="http://exslt.org/sets">
	<xsl:output method="xml" indent="yes" encoding="UTF-8"/>
	<xsl:template match="node()|@*">
		<xsl:copy>
			<xsl:apply-templates select="node()|@*"/>
		</xsl:copy>
	</xsl:template>

    <xsl:template match="resource/@category">
        <xsl:choose>
            <xsl:when test=".='energie'"><xsl:attribute name="network-usable">true</xsl:attribute></xsl:when>
            <xsl:otherwise><xsl:attribute name="network-usable">false</xsl:attribute></xsl:otherwise>
        </xsl:choose>
        <xsl:copy>
            <xsl:apply-templates select="node()|@*"/>
        </xsl:copy>
    </xsl:template>

    <xsl:template match="indirect-fire">
      <xsl:element name="indirect-fires">
          <xsl:attribute name="intervention-type"><xsl:value-of select="./@intervention-type"/></xsl:attribute>
          <xsl:attribute name="x-dispersion"><xsl:value-of select="./@x-dispersion"/></xsl:attribute>
          <xsl:attribute name="y-dispersion"><xsl:value-of select="./@y-dispersion"/></xsl:attribute>
          <xsl:attribute name="detection-range">
              <xsl:choose>
                  <xsl:when test="@detection-range"><xsl:value-of select="./@detection-range"/></xsl:when>
                  <xsl:otherwise>5000</xsl:otherwise>
              </xsl:choose>
          </xsl:attribute>
          <xsl:element name="indirect-fire">
            <xsl:if test="./@type = 'explosif' or ./@type = 'grenade' or ./@type = 'aced'">
              <xsl:attribute name="type">explosif</xsl:attribute>
            </xsl:if>
            <xsl:if test="./@type = 'fumigene' or ./@type = 'effect' or ./@type = 'mine' or ./@type = 'eclairant'">
              <xsl:attribute name="type"><xsl:value-of select="./@type"/></xsl:attribute>
            </xsl:if>
            <xsl:if test="./@type = 'fumigene' or ./@type = 'eclairant'">
              <xsl:attribute name="setup-time"><xsl:value-of select="./@setup-time"/></xsl:attribute>
              <xsl:attribute name="life-time"><xsl:value-of select="./@life-time"/></xsl:attribute>
            </xsl:if>
            <xsl:if test="./@type = 'effect'">
              <xsl:attribute name="object-type"><xsl:value-of select="./@object-type"/></xsl:attribute>
              <xsl:attribute name="life-time"><xsl:value-of select="./@life-time"/></xsl:attribute>
            </xsl:if>
            <xsl:if test="./@type = 'mine'">
              <xsl:attribute name="mine-count"><xsl:value-of select="./@mine-count"/></xsl:attribute>
            </xsl:if>
            <xsl:if test="./@type = 'explosif' or ./@type = 'grenade' or ./@type = 'aced'">
               <xsl:attribute name="neutralization-ratio"><xsl:value-of select="./@neutralization-ratio"/></xsl:attribute>
               <xsl:attribute name="dispersion-factor">0</xsl:attribute>
               <xsl:copy-of select="ph"/>
            </xsl:if>
          </xsl:element>
      </xsl:element>
    </xsl:template>

</xsl:stylesheet>
