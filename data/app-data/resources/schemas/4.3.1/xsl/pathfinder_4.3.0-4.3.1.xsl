<?xml version="1.0" encoding="UTF-8"?><xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">    <xsl:import href="./tools.xsl"/>    <xsl:output method="xml" indent="yes" encoding="UTF-8"/>       <xsl:template match="rule/avoided-terrains/avoided-terrain/@type">        <xsl:attribute name="type">            <xsl:call-template name="convertTerrain">                <xsl:with-param name="type" select="."/>            </xsl:call-template>        </xsl:attribute>    </xsl:template>        <xsl:template match="rule/preferred-terrains/preferred-terrain/@type">        <xsl:attribute name="type">            <xsl:call-template name="convertTerrain">                <xsl:with-param name="type" select="."/>            </xsl:call-template>        </xsl:attribute>    </xsl:template>     <xsl:template match="node()|@*">        <xsl:copy>            <xsl:apply-templates select="node()|@*"/>        </xsl:copy>    </xsl:template>   </xsl:stylesheet>