<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output method="xml" indent="yes" encoding="ISO-8859-1"/>
	
    <xsl:template match="text()"/>

    <xsl:template match="/">
        <orbat>
            <xsl:apply-templates/>
        </orbat>
    </xsl:template>
    
    <xsl:template match="parties/party">
        <side side="{@name}" affiliation="{@type}">
            <xsl:apply-templates/>
            <xsl:call-template name="list-diplomacies">
                <xsl:with-param name="id" select="@id"/>
            </xsl:call-template>
        </side>
    </xsl:template>
    
    <xsl:template match="object">
        <element category="object" name="{@name}" type="{@type}">
            <xsl:apply-templates/>
        </element>
    </xsl:template>
    
    <xsl:template match="automat">
        <xsl:variable name="knowledge-group" select="@knowledge-group"/>
        <element knowledge-group="{/orbat/sides/side/communication/knowledge-group[@id=$knowledge-group]/@name}" category="automat" name="{@name}" type="{@type}"/>
        <xsl:apply-templates/>
    </xsl:template>
    
    <xsl:template match="unit">
        <element category="unit" name="{@name}" type="{@type}" command-post="{@command-post}" location="{@position}"/>
    </xsl:template>
    
    <xsl:template match="population">
        <element category="population" name="{@name}" type="{@type}" location="{@position}" count="{@humans}"/>
    </xsl:template>

    <xsl:template match="points">
        <xsl:attribute name="location">
            <xsl:for-each select="point">
                <xsl:if test="position() > 1">,</xsl:if>
                <xsl:value-of select="text()"/>
            </xsl:for-each>
        </xsl:attribute>
    </xsl:template>
    
    <xsl:template name="list-diplomacies">
        <xsl:param name="id"/>
        <xsl:for-each select="/orbat/diplomacy/party[@id=$id]/relationship">
            <xsl:variable name="side" select="@party"/>
            <element category="diplomacy" name="{/orbat/parties/party[@id=$party]/@name}" type="{@diplomacy}"/>
        </xsl:for-each>
    </xsl:template>
        
</xsl:stylesheet>
