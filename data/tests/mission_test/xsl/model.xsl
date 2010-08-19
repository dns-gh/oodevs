<xsl:stylesheet version = '1.0' xmlns:xsl='http://www.w3.org/1999/XSL/Transform'
                xmlns:url="http://whatever/java/java.net.URLEncoder"
                exclude-result-prefixes="url">
  <xsl:output omit-xml-declaration="yes"/>

  <xsl:template match="/">
    <xsl:text>model =
{
    entities =
    {
        automats =
        {
            [ "ABC" ] = 12, -- must be generated
        }
    },

    types =
    {
        units =
        {</xsl:text>
         <xsl:apply-templates/>
         <xsl:text>
        },
        missions =
        {</xsl:text>
         <xsl:call-template name="missions"/>
         <xsl:text>
        }
    }
}
    </xsl:text>
  </xsl:template>

  <xsl:template match="units">
    <xsl:for-each select="unit">
        <xsl:text>
            [ "</xsl:text><xsl:value-of select="url:encode(@name)"/><xsl:text>" ] = </xsl:text><xsl:value-of select="@id"/><xsl:text>,</xsl:text>
    </xsl:for-each>
  </xsl:template>

  <xsl:template name="missions">
    <xsl:for-each select="document('../../../data/models/ada/physical/france/Missions.xml')//missions/units/mission">
      <xsl:text>
            [ "</xsl:text><xsl:value-of select="@name"/><xsl:text>" ] = </xsl:text><xsl:value-of select="@id"/><xsl:text>,</xsl:text>
    </xsl:for-each>
  </xsl:template>

</xsl:stylesheet>
