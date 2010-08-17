<xsl:stylesheet version = '1.0' xmlns:xsl='http://www.w3.org/1999/XSL/Transform'>
  <xsl:output omit-xml-declaration="yes"/>

  <xsl:variable name="missionsDoc" select="document('../src/Missions.xml')"/>
  <xsl:variable name="unitsDoc" select="document('../src/pions.xml')"/>
  <xsl:template match="/">-- Model
model =
{
    entities =
    {
        -- Automats cannot be dynamically created, so we need to define one of each existing type
        -- in order to be able to create subordinate units
        -- TODO: see if each automat must have at least one subordinate (command post)
        automats =
        {
            [ "ABC" ] = 12, <!-- dont know how to do this -->
        }
    },

    types =
    {
        units =
        {
        <xsl:apply-templates/>
        },
        missions =
        {
        <xsl:call-template name="missions"/>
        }
    }
}
</xsl:template>

  <xsl:template match="units">
    <xsl:for-each select="unit">
      <xsl:call-template name="findUnitIdInBase">
        <xsl:with-param name="unit"><xsl:value-of select="@name"/></xsl:with-param>
      </xsl:call-template>
    </xsl:for-each>
  </xsl:template>
  
  <xsl:template name="findUnitIdInBase">
    <xsl:param name="unit"/>
    <xsl:for-each select="$unitsDoc//unit">
      <xsl:if test="@decisional-model = $unit">
        [ "<xsl:value-of select="@decisional-model"/>" ] = <xsl:value-of select="@id"/>,
      </xsl:if>
    </xsl:for-each>
  </xsl:template>
  
  <xsl:template name="missions">
    <xsl:for-each select="$missionsDoc//missions/units/mission">
        [ "<xsl:value-of select="@name"/>" ] = <xsl:value-of select="@id"/>,
    </xsl:for-each>
  </xsl:template>

</xsl:stylesheet>
