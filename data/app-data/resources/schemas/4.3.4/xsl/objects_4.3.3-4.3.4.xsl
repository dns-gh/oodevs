<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:output method="xml" indent="yes" encoding="UTF-8"/>

    <xsl:template match="node()|@*">
        <xsl:copy>
            <xsl:apply-templates select="node()|@*"/>
        </xsl:copy>
    </xsl:template>

    <xsl:template match="object/@geometry"/>
    <xsl:template match="object/@symbol"/>

    <xsl:template match="object">
        <xsl:element name="object">
            <xsl:apply-templates select="node()|@*"/>
            <xsl:element name="geometries">
                <xsl:call-template name="split">
                    <xsl:with-param name="list"><xsl:value-of select="./@geometry" /></xsl:with-param>
                </xsl:call-template>
            </xsl:element>            
        </xsl:element>
    </xsl:template>

    <xsl:template name="split">
        <xsl:param name="list" />
        <xsl:variable name="newlist" select="concat(normalize-space($list), ' ')" />
        <xsl:variable name="first" select="substring-before($newlist, ' ')" />
        <xsl:variable name="remaining" select="substring-after($newlist, ' ')" />
        <xsl:element name="geometry">
            <xsl:attribute name="type"><xsl:value-of select="$first"/></xsl:attribute>
            <xsl:attribute name="symbol"><xsl:value-of select="./@symbol"/></xsl:attribute>
        </xsl:element>
        <xsl:if test="$remaining">
            <xsl:call-template name="split">
                <xsl:with-param name="list" select="$remaining" />
            </xsl:call-template>
        </xsl:if>
    </xsl:template>
  
</xsl:stylesheet>

