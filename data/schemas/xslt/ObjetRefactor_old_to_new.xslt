<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    version="1.0">

    <xsl:output method="xml" version="1.0" encoding="ISO-8859-1" indent="yes" standalone="no"/>

    <xsl:template match="@*|node()|text()">
        <xsl:copy>
            <xsl:apply-templates select="@*|node()|text()"/>
        </xsl:copy>
    </xsl:template>

    <xsl:template match="object">
        <object>
            <xsl:attribute name="id">
                <xsl:value-of select="@id" />
            </xsl:attribute>
            <xsl:attribute name="name">
                <xsl:value-of select="@name" />
            </xsl:attribute>
            <xsl:attribute name="type">
                <xsl:value-of select="@type" />
            </xsl:attribute>
            <attributes>
                <xsl:apply-templates select="@obstacle-type"/>
                <xsl:apply-templates select="specific-attributes"/>
            </attributes>
            <xsl:apply-templates select="shape|text()"/>
        </object>
    </xsl:template>

    <xsl:template match="@obstacle-type">
        <obstacle activated="true" type="preliminary"/>
    </xsl:template>

    <xsl:template match="specific-attributes">
        <xsl:choose>
            <xsl:when test="width">
                <crossing-site>
                    <xsl:apply-templates/>
                </crossing-site>
            </xsl:when>
            <xsl:otherwise>
                <xsl:apply-templates select="@*|node()|text()"/>
            </xsl:otherwise>
        </xsl:choose>
    </xsl:template>

    <xsl:template match="activity-time">
        <activity-time>
            <xsl:attribute name="value">
                <xsl:value-of select="." />
            </xsl:attribute>
        </activity-time>
    </xsl:template>

    <xsl:template match="density">
        <mine>
        <xsl:copy>
            <xsl:apply-templates select="@*|node()|text()"/>
        </xsl:copy>
        </mine>
    </xsl:template>

    <xsl:template match="danger"/>

    <xsl:template match="nbc-agents">
        <xsl:copy>
            <xsl:attribute name="state">gaseous</xsl:attribute>
                <xsl:attribute name="danger">
                    <xsl:value-of select="parent::*/danger"/>
                </xsl:attribute>
            <xsl:apply-templates select="@*|node()|text()"/>
        </xsl:copy>
    </xsl:template>

</xsl:transform>
