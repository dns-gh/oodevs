<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns="http://www.w3.org/1999/xhtml">
  <xsl:output method="xml" encoding="UTF-8" media-type="text/plain" omit-xml-declaration="yes" standalone="yes" indent="no"/>

  <xsl:template match="factors">
<xsl:text>

brain "brain" {}

dofile "resources/scripts/commands.lua"
dofile "resources/scripts/events.lua"
dofile "resources/scripts/actions.lua"

scriptName = "success-factors"

function Start()

    local indicators = {}
    local factors = {
</xsl:text>
    <xsl:apply-templates select="factor"/>
<xsl:text>
    }

    function GetIndicator( name )
        return indicators[name]
    end

    function CheckProfiles( profile )
        for _, factor in pairs( factors ) do
            if factor.CheckProfiles( profile ) then
                return true
            end
        end
        return false;
    end

    function CheckConditions()
        local result = false
        for _, factor in pairs( factors ) do
            if factor.CheckConditions() then
                factor.ExecuteActions()
                result = true
            end
        end
        return result
    end

    local eventTable =
    {
        {
            events.sim:ClientConnected(),
            { },
            function( client, profile )
                if CheckProfiles( profile ) == false then
                    return
                end
                ChangeState( "processing" )
            end
        },

        {
            events.indicators:IndicatorChanged(),
            { "processing" },
            function( name, value )
                indicators[name] = value
                if CheckConditions() then
                    ChangeState( "finish" )
                end
            end
        },

        AtState( "finish",
            function()
                Deactivate()
            end
        )
    }
    DeclareEvents( eventTable )

end
</xsl:text>
  </xsl:template>

  <xsl:template match="factor">
<xsl:text>      
        {
            CheckProfiles = function( profile )
</xsl:text>
    <xsl:apply-templates select="profiles"/>
<xsl:text>
            end,

            CheckConditions = function()
</xsl:text>
    <xsl:apply-templates select="conditions"/>
<xsl:text>
            end,

            ExecuteActions = function()
</xsl:text>
    <xsl:apply-templates select="actions"/>
<xsl:text>
            end
        },
</xsl:text>
  </xsl:template>

  <xsl:template match="profiles">
      <xsl:text>              return </xsl:text>
      <xsl:choose>
          <xsl:when test="count( ./profile ) > 0">
              <xsl:for-each select="./profile">
                  <xsl:if test="position() > 1">
                      <xsl:text>
                  or </xsl:text>
                  </xsl:if>
                  <xsl:apply-templates select="."/>
              </xsl:for-each>
          </xsl:when>
          <xsl:otherwise>
              <xsl:text>false</xsl:text>
          </xsl:otherwise>
      </xsl:choose>
  </xsl:template>

  <xsl:template match="profile">
      <xsl:text>( profile == "</xsl:text><xsl:value-of select="@name"/><xsl:text>" )</xsl:text>
  </xsl:template>

  <xsl:template match="conditions">
      <xsl:text>              return </xsl:text>
      <xsl:choose>
          <xsl:when test="count( ./condition ) > 0">
              <xsl:for-each select="./condition">
                  <xsl:if test="position() > 1">
                      <xsl:text>
                  </xsl:text><xsl:value-of select="../@operator"/><xsl:text> </xsl:text>
                  </xsl:if>
                  <xsl:apply-templates select="."/>
              </xsl:for-each>
          </xsl:when>
          <xsl:otherwise>
              <xsl:text>false</xsl:text>
          </xsl:otherwise>
      </xsl:choose>
  </xsl:template>

  <xsl:template match="condition">
      <xsl:text>( GetIndicator( "</xsl:text>
      <xsl:value-of select="@property"/>
      <xsl:text>" ) ~= nil and </xsl:text>
        <xsl:text> GetIndicator( "</xsl:text>
      <xsl:value-of select="@property"/>
      <xsl:text>" ) </xsl:text>
      <xsl:call-template name="comparison-operator">
          <xsl:with-param name="operator" select="@operator"/>
      </xsl:call-template>
      <xsl:text> </xsl:text>
      <xsl:value-of select="@value"/>
      <xsl:text> )</xsl:text>
  </xsl:template>

    <xsl:template name="comparison-operator">
        <xsl:param name="operator"/>
        <xsl:choose>
            <xsl:when test="$operator = 'eq'"><xsl:text>==</xsl:text></xsl:when>
            <xsl:when test="$operator = 'ne'"><xsl:text>!=</xsl:text></xsl:when>
            <xsl:when test="$operator = 'lt'"><xsl:text>&lt;</xsl:text></xsl:when>
            <xsl:when test="$operator = 'le'"><xsl:text>&lt;=</xsl:text></xsl:when>
            <xsl:when test="$operator = 'gt'"><xsl:text>&gt;</xsl:text></xsl:when>
            <xsl:when test="$operator = 'ge'"><xsl:text>&gt;=</xsl:text></xsl:when>
        </xsl:choose>
    </xsl:template>

  <xsl:template match="actions">
        <xsl:apply-templates select="action"/>
  </xsl:template>

  <xsl:template match="action">
      <xsl:text>                 </xsl:text>
      <xsl:value-of select="@type"/>
      <xsl:text>(</xsl:text>
      <xsl:if test="count( ./parameter ) > 0">
          <xsl:text> { </xsl:text>
      </xsl:if>
      <xsl:for-each select="./parameter">
            <xsl:if test="position() > 1">
                <xsl:text>, </xsl:text>
            </xsl:if>
            <xsl:apply-templates select="."/>
        </xsl:for-each>
        <xsl:if test="count( ./parameter ) > 0">
          <xsl:text> } </xsl:text>
      </xsl:if>
      <xsl:text>)
</xsl:text>
  </xsl:template>

  <xsl:template match="parameter">
      <xsl:value-of select="@name"/><xsl:text> = "</xsl:text><xsl:value-of select="@value"/><xsl:text>"</xsl:text>
  </xsl:template>

</xsl:stylesheet>