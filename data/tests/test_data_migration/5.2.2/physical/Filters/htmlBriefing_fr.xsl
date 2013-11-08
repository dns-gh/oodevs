<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output method="html" indent="yes" encoding="ISO-8859-1"/>
	
    <xsl:template match="comment()|text()"/>

    <xsl:template match="/exercise">
        <html>
        <head>
            <xsl:apply-templates select="meta/name"/>
        </head>
        <body>
            <h1>Données d'exercice:</h1>
            <ul>
                <xsl:apply-templates select="model"/>
                <xsl:apply-templates select="terrain"/>
            </ul>
            <xsl:apply-templates select="meta/briefing"/>
            <xsl:apply-templates select="meta/resources"/>
        </body>
        </html>
    </xsl:template>

    <xsl:template match="/exercise/meta/name">
       <title><xsl:value-of select="text()"/></title>
	</xsl:template>
    
    <xsl:template match="/exercise/meta/briefing/text">
        <div xml:lang="{@lang}">
            <h2>Briefing <xsl:value-of select="@lang"/></h2>
            <xsl:value-of select="text()" disable-output-escaping="yes"/>
        </div>
    </xsl:template>

    <xsl:template match="/exercise/meta/resources">
        <h2>Ressources</h2>
        <ul>
            <xsl:apply-templates select="resource"/>
        </ul>
    </xsl:template>
    
    <xsl:template match="/exercise/meta/resources/resource">
        <li><xsl:value-of select="@name"/>: <xsl:value-of select="@file"/></li>
    </xsl:template>
    
    <xsl:template match="terrain">
        <li>Terrain: <xsl:value-of select="@name"/></li>
    </xsl:template>

    <xsl:template match="model">
        <li>Modele decisionnel: <xsl:value-of select="@dataset"/></li>
        <li>Modele physique: <xsl:value-of select="@physical"/></li>
    </xsl:template>
    
</xsl:stylesheet>
