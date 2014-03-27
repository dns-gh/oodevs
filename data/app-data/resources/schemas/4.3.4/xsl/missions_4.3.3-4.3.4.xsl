<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fn="http://exslt.org/sets">
    <xsl:output method="xml" indent="yes" encoding="UTF-8"/>

    <xsl:template match="node()|@*">
        <xsl:copy>
            <xsl:apply-templates select="node()|@*"/>
        </xsl:copy>
    </xsl:template>

    <xsl:template match="parameter[@type='ResourceNetwork']">
        <xsl:copy>
            <xsl:apply-templates select="node()|@*"/>
            <xsl:attribute name="type">ResourceNetworkNode</xsl:attribute>
        </xsl:copy>
    </xsl:template>

    <xsl:template match="value[@name='emploi_force_interdit']">
        <xsl:copy>
            <xsl:apply-templates select="node()|@*"/>
            <xsl:attribute name="id">1</xsl:attribute>
        </xsl:copy>
    </xsl:template>

    <xsl:template match="value[@name='emploi_armes_non_letales_autorisees']">
        <xsl:copy>
            <xsl:apply-templates select="node()|@*"/>
            <xsl:attribute name="id">2</xsl:attribute>
        </xsl:copy>
    </xsl:template>

    <xsl:template match="value[@name='emploi_armes_letales_autorisees']">
        <xsl:copy>
            <xsl:apply-templates select="node()|@*"/>
            <xsl:attribute name="id">3</xsl:attribute>
        </xsl:copy>
    </xsl:template>

</xsl:stylesheet>
