<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fn="http://exslt.org/sets">
	<xsl:output method="xml" indent="yes" encoding="UTF-8"/>
	<xsl:template match="node()|@*">
		<xsl:copy>
			<xsl:apply-templates select="node()|@*"/>
		</xsl:copy>
	</xsl:template>
	<xsl:template match="@intoxication">
		<xsl:attribute name="affliction">
            <xsl:value-of select="."/>
		</xsl:attribute>
	</xsl:template>
	<xsl:template match="@wound">
	    <xsl:attribute name="wound">
	        <xsl:choose>
                <xsl:when test=".='nonblesse'">healthy</xsl:when>
                <xsl:when test=".='mort'">dead</xsl:when>
                <xsl:otherwise><xsl:value-of select="."/></xsl:otherwise>
            </xsl:choose>
	     </xsl:attribute>
    </xsl:template>
</xsl:stylesheet>
