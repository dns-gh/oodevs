<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output method="xml" indent="yes" encoding="ISO-8859-1"/>
	
    <xsl:template match="text()"/>

    <xsl:template match="/">
        <orbat>
            <xsl:apply-templates/>
        </orbat>
    </xsl:template>
    
    <xsl:template match="sides/side">
        <camp camp="{@name}" affiliation="{@type}">
            <xsl:apply-templates/>
            <xsl:call-template name="list-diplomacies">
                <xsl:with-param name="id" select="@id"/>
            </xsl:call-template>
        </camp>
    </xsl:template>
    	
    <xsl:template match="object">
        <element categorie="objet" nom="{@name}" type="{@type}">
            <xsl:apply-templates/>
        </element>
    </xsl:template>
    
    <xsl:template match="automat">
        <xsl:variable name="knowledge-group" select="@knowledge-group"/>
        <element groupe-de-connaissance="{/orbat/sides/side/communication/knowledge-group[@id=$knowledge-group]/@name}" categorie="automate" nom="{@name}" type="{@type}"/>
        <xsl:apply-templates/>
    </xsl:template>
    
    <xsl:template match="unit">
        <element categorie="unité" nom="{@name}" type="{@type}" pc="{@command-post}" localisation="{@position}"/>
    </xsl:template>
    
    <xsl:template match="population">
        <element categorie="population" nom="{@name}" type="{@type}" localisation="{@position}" nombre="{@humans}"/>
    </xsl:template>
    
    <xsl:template match="points">
        <xsl:attribute name="localisation">
            <xsl:for-each select="point">
                <xsl:if test="position() > 1">,</xsl:if>
                <xsl:value-of select="text()"/>
            </xsl:for-each>
        </xsl:attribute>
    </xsl:template>
    
    <xsl:template name="list-diplomacies">
        <xsl:param name="id"/>
        <xsl:for-each select="/orbat/diplomacies/side[@id=$id]/relationship">
            <xsl:variable name="side" select="@side"/>
            <element categorie="diplomacie" nom="{/orbat/sides/side[@id=$side]/@name}" type="{@diplomacy}"/>
        </xsl:for-each>
    </xsl:template>
    
</xsl:stylesheet>