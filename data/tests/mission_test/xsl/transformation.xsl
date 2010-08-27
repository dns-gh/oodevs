<xsl:stylesheet version = '1.0' xmlns:xsl='http://www.w3.org/1999/XSL/Transform'
                xmlns:lxslt="http://xml.apache.org/xslt"
                xmlns:redirect="http://xml.apache.org/xalan/redirect"
                extension-element-prefixes="redirect"
                xmlns:url="http://whatever/java/java.net.URLEncoder"
                exclude-result-prefixes="url"
                >
  <xsl:output method="text" encoding="iso-8859-1" media-type="text/plain" omit-xml-declaration="yes" standalone="yes" indent="no"/>
  <xsl:param name="output"/>
  <xsl:param name="missionsPath"/>
  <xsl:param name="modelesPath"/>
  <xsl:param name="reportFile"/>
  
  <xsl:variable name="missionsDoc" select="document('../../../data/models/ada/physical/france/Missions.xml')"/> <!-- use params from ant file ! -->
  <xsl:variable name="modelesDoc"  select="document('../../../data/models/ada/physical/france/Modeles.xml')"/>  <!-- use params from ant file ! -->

  <xsl:template match="comment()|text()"/>

  <xsl:template match="unit">
    <xsl:variable name="physical" select="url:encode(@name)"/>
    <xsl:variable name="physicalNotEncoded" select="@name"/>
    <xsl:variable name="decisional" select="@decisional-model"/>
    <xsl:for-each select="$modelesDoc//missions[../@name = $decisional]">
      <redirect:write file="{$output}/{$physical}.lua">
          <xsl:text>brain "brain" {}

require "io"
require "table"

dofile "resources/scripts/actions.lua"
dofile "resources/scripts/events.lua"
dofile "resources/scripts/indicators.lua"
include "resources/model.lua"
include "resources/config.lua"

function Start()

    local frequency = 1 -- second(s) between mission start
    local unit = nil

    local eventTable =
    {
        {
            events:Once(),
            { },
            function( client, profile )
                ChangeState( "</xsl:text><xsl:value-of select="$physical"/><xsl:text>_startup" )
            end
        },

        {
            events.sim:TickEnded(),
            { "</xsl:text><xsl:value-of select="$physical"/><xsl:text>_startup" },
            function()
                ChangeState( "</xsl:text><xsl:value-of select="$physical"/><xsl:text>_test_begin" )
            end
        },
        </xsl:text>
   <xsl:text>
        {
            events:Once(),
            { "</xsl:text><xsl:value-of select="$physical"/><xsl:text>_test_begin" },
            function()
                sim:CreateUnit( coord:UtmPosition( config.positions.start[1] )
                                , model.types.units["</xsl:text><xsl:value-of select="$physical"/><xsl:text>"]
                                </xsl:text>
                                <xsl:choose>
                                    <xsl:when test="../@file = 'BMPion.bms'">
                                        <xsl:text>, model.entities.automats["SGTIA"] )</xsl:text>
                                    </xsl:when>
                                    <xsl:otherwise>
                                        <xsl:text>, model.entities.automats["ABC"] )</xsl:text>
                                    </xsl:otherwise>
                                </xsl:choose>
                               <xsl:text>
                ChangeState( "</xsl:text><xsl:value-of select="$physical"/><xsl:text>_test_wait_unit_creation" )
                Trace( "</xsl:text><xsl:value-of select="$physical"/><xsl:text> : agent creation" )
            end
        },

        {
            events.agents:AgentCreated(),
            { "</xsl:text><xsl:value-of select="$physical"/><xsl:text>_test_wait_unit_creation" },
            function( entity )
                if entity:GetName() == "</xsl:text><xsl:value-of select="$physicalNotEncoded"/><xsl:text>" then
                    unit = entity
                    StartTimeSequence( "</xsl:text><xsl:value-of select="$physical"/><xsl:text>_messages" )
                end
            end
        },
        </xsl:text>
     <xsl:for-each select="mission">
        <xsl:call-template name="unitMission">
          <xsl:with-param name="unit"    ><xsl:value-of select="$physical"/></xsl:with-param>
          <xsl:with-param name="mission" ><xsl:value-of select="@name"      /></xsl:with-param>
       </xsl:call-template>
     </xsl:for-each>
     <xsl:text>
        TimeSequence( "</xsl:text><xsl:value-of select="$physical"/><xsl:text>\messages", frequency,
            function()
                ChangeState( "</xsl:text><xsl:value-of select="$physical"/><xsl:text>_test_end" )
            end
        ),
     
        AtState( "</xsl:text><xsl:value-of select="$physical"/><xsl:text>_test_end",
            function()
                Deactivate()
            end
        ),
    }

    DeclareEvents( eventTable )
end
</xsl:text>
      </redirect:write>
    </xsl:for-each>
  </xsl:template>

    <xsl:template name="unitMission">
        <xsl:param name="mission"/>
        <xsl:param name="unit"/>
        <xsl:for-each select="$missionsDoc//missions/units/mission[@name=$mission]">
            <xsl:text>
        TimeSequence( "</xsl:text><xsl:value-of select="$unit"/><xsl:text>_messages", frequency,
            function()
                Mission.create( unit:GetIdentifier(), model.types.missions["</xsl:text><xsl:value-of select="@name"/><xsl:text>"] )
            </xsl:text>
                <xsl:call-template name="common-parameters"/>
                <xsl:for-each select="parameter[@optional='false']">
                    <xsl:call-template name="parameter">
                        <xsl:with-param name="unit"><xsl:value-of select="$unit"/></xsl:with-param>
                    </xsl:call-template>
                </xsl:for-each>
                <xsl:text>
                    :Issue()
                Trace( "</xsl:text><xsl:value-of select="$unit"/><xsl:text> : running </xsl:text><xsl:value-of select="@name"/><xsl:text>" )
            end),
        TimeSequence( "</xsl:text><xsl:value-of select="$unit"/><xsl:text>_messages", frequency,
            function()
                unit:Teleport( coord:UtmPosition( config.positions.start[1] ) )
                unit:RecoverAll()
            end
        ),

        </xsl:text>
        </xsl:for-each>
    </xsl:template>

    <xsl:template name="common-parameters">
        <xsl:text>        :With( { name = "Danger direction", type = "Direction", value = 0 } )
                </xsl:text>
    </xsl:template>

    <xsl:template name="parameter">
        <xsl:param name="unit"/>
        <xsl:param name="count"/>
        <xsl:param name="index"/>
        <xsl:choose>
            <xsl:when test="@type = 'Location'">
                <xsl:text>    :With( LocationParameter.create( "</xsl:text><xsl:value-of select="@name"/><xsl:text>", { config.positions.destination[1], config.positions.destination[2], config.positions.destination[3] } ) )</xsl:text>
            </xsl:when>
            <xsl:when test="@type = 'Enumeration'">
                <xsl:text>    :With( { name = "</xsl:text><xsl:value-of select="@name"/><xsl:text>", type = "Enumeration", value = "1"} )</xsl:text>
            </xsl:when>
            <xsl:when test="@type = 'Bool'">
                <xsl:text>    :With( { name = "</xsl:text><xsl:value-of select="@name"/><xsl:text>", type = "Bool", value = "false"} )</xsl:text>
            </xsl:when>
            <xsl:when test="(@type = 'Path')"> <!-- or (@type = 'PathBM') -->
                <xsl:text>    :With( Path.create( "Route" ):AddPoint( "Destination", config.positions.destination[1] ) )</xsl:text>
            </xsl:when>
            <xsl:when test="(@type = 'Point') or (@type = 'PointBM')">
                <xsl:text>    :With( PointLocation.create( "</xsl:text><xsl:value-of select="@name"/><xsl:text>", config.positions.destination[1], "</xsl:text><xsl:value-of select="@type"/><xsl:text>" ) )</xsl:text>
            </xsl:when>
            <xsl:when test="@type = 'Polygon'">
                <xsl:text>    :With( Polygon.create( "</xsl:text><xsl:value-of select="@name"/><xsl:text>", { config.positions.destination[1], config.positions.destination[2], config.positions.destination[3] } ) )</xsl:text>
            </xsl:when>
            <xsl:when test="@type = 'NatureAtlas'">
                <xsl:text>    :With( { name = "</xsl:text><xsl:value-of select="@name"/><xsl:text>", type = "NatureAtlas", value = "256" } )</xsl:text>
            </xsl:when>
            <xsl:when test="@type = 'Numeric'">
                <xsl:text>    :With( { name = "</xsl:text><xsl:value-of select="@name"/><xsl:text>", type = "Numeric", value = "1" } )</xsl:text>
            </xsl:when>
            <xsl:when test="(@type = 'Agent') or (@type = 'AgentBM')">
                <xsl:text>    :With( { name = "</xsl:text><xsl:value-of select="@name"/><xsl:text>", type = "</xsl:text><xsl:value-of select="@type"/><xsl:text>", value = config.id.blueUnit } )</xsl:text>
            </xsl:when>
            
            
            <xsl:otherwise>
                <xsl:text>    -- Missing argument</xsl:text>
                <redirect:write append="true" file="{$reportFile}">
<xsl:value-of select="$unit"/><xsl:text>;</xsl:text><xsl:value-of select="../@name"/><xsl:text>;</xsl:text><xsl:value-of select="@type"/><xsl:text>
</xsl:text>
                </redirect:write>
            </xsl:otherwise>
        </xsl:choose>
    </xsl:template>

</xsl:stylesheet>