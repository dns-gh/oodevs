<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fn="http://exslt.org/sets">
    <xsl:output method="xml" indent="yes" encoding="UTF-8"/>
    <xsl:template match="/breakdowns">
        <xsl:copy>
            <xsl:apply-templates select="@*"/>
            <xsl:if test="count( ./repair-duration-in-man-hours ) = 0">
                <xsl:element name="repair-duration-in-man-hours">
                    <xsl:attribute name="value">false</xsl:attribute>
                </xsl:element>
            </xsl:if>
            <xsl:apply-templates select="node()"/>
        </xsl:copy>
    </xsl:template>

    <xsl:template match="node()|@*">
        <xsl:copy>
            <xsl:apply-templates select="node()|@*"/>
        </xsl:copy>
    </xsl:template>
</xsl:stylesheet>
