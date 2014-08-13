<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:output method="xml" indent="yes" encoding="UTF-8"/>

    <xsl:template match="/decisional">
        <xsl:copy>
            <xsl:apply-templates select="node()|@*"/>
            <xsl:if test="count( ./perception ) = 0">
                <xsl:element name="perception">
                    <xsl:attribute name="detect-destroyed-units">false</xsl:attribute>
                </xsl:element>
            </xsl:if>
        </xsl:copy>
    </xsl:template>

    <xsl:template match="node()|@*">
        <xsl:copy>
            <xsl:apply-templates select="node()|@*"/>
        </xsl:copy>
    </xsl:template>
  
</xsl:stylesheet>