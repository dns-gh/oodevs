<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fn="http://exslt.org/sets">
	<xsl:output method="xml" indent="yes" encoding="UTF-8"/>
	<xsl:template match="node()|@*">
		<xsl:copy>
			<xsl:apply-templates select="node()|@*"/>
		</xsl:copy>
	</xsl:template>

    <xsl:template match="resource/@category">
        <xsl:if test="count( @logistic-supply-class ) = 0">
            <xsl:choose>
                <xsl:when test=".='munition'">
                    <xsl:choose>
                        <xsl:when test="count( ../@d-type ) = 0">
                            <xsl:attribute name="logistic-supply-class">Unite non tranche D</xsl:attribute>
                        </xsl:when>
                        <xsl:otherwise>
                            <xsl:attribute name="logistic-supply-class">Unite tranche D</xsl:attribute>
                        </xsl:otherwise>
                    </xsl:choose>
                </xsl:when>
                <xsl:when test=".='carburant'"><xsl:attribute name="logistic-supply-class">Unite essence</xsl:attribute></xsl:when>
                <xsl:when test=".='mine'"><xsl:attribute name="logistic-supply-class">Unite non tranche D</xsl:attribute></xsl:when>
                <xsl:when test=".='explosif'"><xsl:attribute name="logistic-supply-class">Unite non tranche D</xsl:attribute></xsl:when>
                <xsl:when test=".='barbele'"><xsl:attribute name="logistic-supply-class">Unite non tranche D</xsl:attribute></xsl:when>
                <xsl:when test=".='piece'"><xsl:attribute name="logistic-supply-class">Pieces</xsl:attribute></xsl:when>
                <xsl:when test=".='ration'"><xsl:attribute name="logistic-supply-class">Unite vivre</xsl:attribute></xsl:when>
                <xsl:when test=".='agent extincteur'"><xsl:attribute name="logistic-supply-class">Unite vivre</xsl:attribute></xsl:when>
                <xsl:when test=".='energie'"><xsl:attribute name="logistic-supply-class">Unite vivre</xsl:attribute></xsl:when>
            </xsl:choose>
        </xsl:if>        
        <xsl:copy>
            <xsl:apply-templates select="node()|@*"/>
        </xsl:copy>
    </xsl:template>

    <xsl:template match="indirect-fire">
      <xsl:element name="indirect-fires">
          <xsl:attribute name="intervention-type"><xsl:value-of select="./@intervention-type"/></xsl:attribute>
          <xsl:attribute name="x-dispersion"><xsl:value-of select="./@x-dispersion"/></xsl:attribute>
          <xsl:attribute name="y-dispersion"><xsl:value-of select="./@y-dispersion"/></xsl:attribute>
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
               <xsl:copy-of select="ph"/>
            </xsl:if>
          </xsl:element>
      </xsl:element>
    </xsl:template>

</xsl:stylesheet>
