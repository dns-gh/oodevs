<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:output method="text" encoding="UTF-8" />
    <xsl:template name="setting" match="/">
        <xsl:if test="position() > 1">,</xsl:if>
        {
        "id" : "<xsl:value-of select="@attribute"/>",
        "label" : "<xsl:value-of select="descriptions/description[@lang='en']/@name"/>",
        "description" : "<xsl:value-of select="descriptions/description[@lang='en']/."/>",
        "type" : "<xsl:value-of select="@type"/>",
        "default" : "<xsl:value-of select="@default"/>"
        }
    </xsl:template>
    <xsl:template match="/">
        {
        "name" : "<xsl:value-of select="plugin/descriptions/description[@lang='en']/@name"/>",
        "description" : "<xsl:value-of select="plugin/descriptions/description[@lang='en']/."/>",
        "groups" :
        [
        {
        "label" : "General",
        "options" : 
        [
        <xsl:for-each select="plugin/settings/setting[not(@display)]">
            <xsl:call-template name="setting" />
        </xsl:for-each>
        ]
        }
        <xsl:for-each select="plugin/settings/group">
            ,
            {
            "label" : "<xsl:value-of select="descriptions/description[@lang='en']/@name" />",
            "options" : 
            [
            <xsl:for-each select="settings/setting[not(@display)]">
                <xsl:call-template name="setting" />
            </xsl:for-each>
            <xsl:for-each select="settings/group/settings/setting[not(@display)]">
                <xsl:if test="position() = 1">,</xsl:if>
                <xsl:call-template name="setting"/>
            </xsl:for-each>
            ]
            }
        </xsl:for-each>
        ]
        }
    </xsl:template>
</xsl:stylesheet>
