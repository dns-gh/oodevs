<xsl:stylesheet version = '1.0' xmlns:xsl='http://www.w3.org/1999/XSL/Transform'
                xmlns:lxslt="http://xml.apache.org/xslt"
                xmlns:redirect="http://xml.apache.org/xalan/redirect"
                extension-element-prefixes="redirect">
  <xsl:output method="text" encoding="iso-8859-1" media-type="text/plain" omit-xml-declaration="yes" standalone="yes" indent="no"/>
  <xsl:param name="output"/>
  <xsl:variable name="missionsDoc" select="document('../src/Missions.xml')"/>

    <xsl:template match="comment()|text()"/>

   <xsl:template match="units/unit/missions">
   <redirect:write file="{$output}/{../@name}.lua">
   <xsl:text>brain "brain" {}

require "io"
require "table"

dofile "resources/scripts/actions.lua"
dofile "resources/scripts/events.lua"
dofile "resources/scripts/indicators.lua"
include "resources/model.lua"
include "resources/config.lua"

function Start()

    local currentTick = 0
    local unitId = 0

    local eventTable =
    {
        {
            events.sim:ClientConnected(),
            { },
            function( client, profile )
                if profile ~= "supervisor" then return end
                ChangeState( "startup" )
            end
        },

        {
            events.sim:TickEnded(),
            { "startup" },
            function()
                Trace( "startup" )
                ChangeState( "test_begin" )
            end
        },
        </xsl:text>
   <xsl:text>
        {
            events:Once(),
            { "test_begin" },
            function()
                sim:CreateUnit( coord:UtmPosition( config.positions.start[1] )
                               , model.types.units["</xsl:text><xsl:value-of select="../@name"/><xsl:text>"]
                               , model.entities.automats["ABC"] ) -- verifier comment ne pas passer d'automate ou comment trouver un automate correct
                ChangeState( "test_wait_unit_creation" )
                Trace( "creating </xsl:text><xsl:value-of select="../@name"/><xsl:text>" )
            end
        },
        {
            events.agents:AgentCreated(),
            { "test_wait_unit_creation" },
            function( entity )
                unitId = entity:GetIdentifier()
                ChangeState( "test_missions" )
            end
        },

        {
        -- start all missions
        events.sim:TickEnded(),
            { "test_missions" },
            function( tick )
                currentTick = currentTick + 1
                Trace( "mission" )</xsl:text>
     <xsl:for-each select="mission">
        <xsl:call-template name="unitMission">
        <xsl:with-param name="unit"><xsl:value-of select="../../@name"/></xsl:with-param>
         <xsl:with-param name="mission"><xsl:value-of select="@name"/></xsl:with-param>
         <xsl:with-param name="position"><xsl:value-of select="position()"/></xsl:with-param>
       </xsl:call-template>
     </xsl:for-each>
        <xsl:text>elseif currentTick > </xsl:text><xsl:value-of select="count( mission ) * 10"/><xsl:text> then
                    ChangeState( "test_end" )
                end
            end
        },
        </xsl:text>
         <xsl:text>
        AtState( "test_end",
            function()
                Trace( "end" )
                Deactivate()
            end
        ),
    }

    DeclareEvents( eventTable )
end
</xsl:text>
    </redirect:write>
    </xsl:template>

    <xsl:template name="unitMission">
      <xsl:param name="unit"/>
      <xsl:param name="mission"/>
      <xsl:param name="position"/>
      <xsl:call-template name="formatMission">
        <xsl:with-param name="mission"><xsl:value-of select="$mission"/></xsl:with-param>
        <xsl:with-param name="unit"><xsl:value-of select="$unit"/></xsl:with-param>
        <xsl:with-param name="position"><xsl:value-of select="$position"/></xsl:with-param>
      </xsl:call-template>
    </xsl:template>

    <xsl:template name="formatMission">
        <xsl:param name="mission"/>
        <xsl:param name="unit"/>
        <xsl:param name="position"/>
        <xsl:for-each select="$missionsDoc//missions/units/mission[@name=$mission]">
            <xsl:choose>
                <xsl:when test="$position = 1"><xsl:text>
                if</xsl:text></xsl:when>
                <xsl:otherwise><xsl:text>elseif</xsl:text></xsl:otherwise>
            </xsl:choose>
            <xsl:text> currentTick == </xsl:text>
            <xsl:value-of select="$position * 10"/>
            <xsl:text> then </xsl:text>
            <xsl:text>
                    Mission.create( unitId, model.types.missions["</xsl:text><xsl:value-of select="@name"/><xsl:text>"] )
            </xsl:text>
                <xsl:call-template name="common-parameters"/><xsl:text>
                </xsl:text>
                <xsl:for-each select="parameter[@optional='false']">
                    <xsl:call-template name="parameter"/>
                </xsl:for-each>
                <xsl:text>
                        :Issue()
                    Trace( "</xsl:text><xsl:value-of select="$unit"/><xsl:text> : running </xsl:text><xsl:value-of select="@name"/><xsl:text>" )
                </xsl:text>
        </xsl:for-each>
    </xsl:template>

    <xsl:template name="common-parameters">
        <xsl:text>            :With( { name = "Danger direction", type = "Direction", value = 0 } )</xsl:text>
    </xsl:template>

    <xsl:template name="parameter">
        <xsl:choose>
            <xsl:when test="@type = 'Path'">
                <xsl:text>        :With( Path.create( "Route" ):AddPoint( "Destination", config.positions.destination[1] ) )</xsl:text>
            </xsl:when>
            <xsl:when test="@type = 'Unit'">
                <xsl:text>        :With( { name = "Unit", type="unit", value = stubs.EnemyUnit() )</xsl:text>
            </xsl:when>
        </xsl:choose>
    </xsl:template>

</xsl:stylesheet>