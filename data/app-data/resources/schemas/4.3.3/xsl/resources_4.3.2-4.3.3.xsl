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
</xsl:stylesheet>
