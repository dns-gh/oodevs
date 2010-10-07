<xsl:stylesheet version = '1.0' xmlns:xsl='http://www.w3.org/1999/XSL/Transform' exclude-result-prefixes="url">
    <xsl:output method="text" encoding="iso-8859-1" media-type="text/plain" omit-xml-declaration="yes" standalone="yes" indent="no"/>
    <xsl:param name="profilename"/>
    
    <xsl:template match="actions">
        <xsl:text>brain "brain" {}

dofile "resources/scripts/commands.lua"
dofile "resources/scripts/events.lua"

function Start()

    local eventTable =
    {
    
        {
            events:Once(),
            { },
            function()
                Trace( "init" )
                ChangeState( "init" )
            end
        },

        {
            events.sim:TickEnded(),
            { "init" },
            function()
                Trace( "firstTick" )</xsl:text>
                <xsl:for-each select='action[@name="Unit Creation"]'>
                    <xsl:text>
                sim:CreateUnit( coord:UtmPosition( "</xsl:text>
            <xsl:value-of select=".//point/@coordinates"/>
            <xsl:text>" ), "</xsl:text>
            <xsl:value-of select='./parameter[@name="UnitType"]/@value'/>
            <xsl:text>", "</xsl:text>
            <xsl:value-of select='@target'/>
            <xsl:text>")</xsl:text>
        </xsl:for-each>
         <xsl:text>
                ChangeState( "CreationEnded" )
            end
        },

        {
            events.sim:TickEnded(),
            { "CreationEnded" },
            function()
                Trace( "CreationsEnded" )
                ChangeState( "ready" )
            end
        },

        {
            events:Once(),
            { "ready" },
            function()
                Trace( "play" )</xsl:text>
        <xsl:for-each select='action[@name!="Unit Creation"]'>
                <xsl:text>
                actions:IssueOrderFromFile( "</xsl:text><xsl:value-of select="@name"/><xsl:text>", "</xsl:text><xsl:value-of select="$profilename"/><xsl:text>" )</xsl:text>
        </xsl:for-each>
        <xsl:text>
                ChangeState( "end" )
            end
        },

    }
    DeclareEvents( eventTable )

end
        </xsl:text>
    </xsl:template>

</xsl:stylesheet>
