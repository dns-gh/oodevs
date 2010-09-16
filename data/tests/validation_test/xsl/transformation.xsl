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
            {},
            function()
                sim:Pause()
            end
        },

        {
            events.sim:ClientConnected(),
            {},
            function( client, profile )
                if profile ~= "</xsl:text><xsl:value-of select="$profilename"/><xsl:text>" then Deactivate() return end
                ResetOptions( { client = client } )
                SetDock( { client = client, hide = docks.ALL } )
                SetDock( { show = { "orbat" } } )
                SetDock( { show = { "clock" } } )
                ChangeState( "creations" )
            end
        },

        AtState( "creations",
            function()
        </xsl:text>

        <xsl:for-each select='action[@name="Unit Creation"]'>
            <xsl:text>        sim:CreateUnit( coord:UtmPosition( "</xsl:text>
            <xsl:value-of select=".//point/@coordinates"/>
            <xsl:text>" ), "</xsl:text>
            <xsl:value-of select='./parameter[@name="UnitType"]/@value'/>
            <xsl:text>", "</xsl:text>
            <xsl:value-of select='@target'/>
            <xsl:text>" )</xsl:text>
        </xsl:for-each>
        <xsl:text>
                sim:Resume()
                ChangeState( "autoormandialog" )
            end
        ),

        AtState( "autoormandialog",
            function()
                sim:Pause()
                Dialog( { id      = "selectMode_choice",
                          message = "Test de validation </xsl:text><xsl:value-of select="$profilename"/><xsl:text>. Selectionnez un mode de jeu :",
                          buttons = { "Automatique", "manuel" } } )
            end
        ),

        {
            events.client:UserChose(), {},
            function( dialog, answer )
                if dialog == "selectMode_choice" then
                    if answer == "Automatique" then
                        ChangeState( "play" )
                    else
                        ChangeState( "initDialog" )
                    end
                end
            end
        },

        AtState( "play",
            function()
                sim:Resume()
        </xsl:text>
        <xsl:for-each select='action[@name!="Unit Creation"]'>
                <xsl:text>
                actions:IssueOrderFromFile( "</xsl:text><xsl:value-of select="@name"/><xsl:text>", "</xsl:text><xsl:value-of select="$profilename"/><xsl:text>" )</xsl:text>
        </xsl:for-each>
        <xsl:text>
                ChangeState( "initDialog" )
            end
        ),
        AtState( "initDialog",
            function()
                Dialog( { id      = "init_choice",
                          message = "Test de validation </xsl:text><xsl:value-of select="$profilename"/><xsl:text>. Reinitialiser l'exercice",
                          buttons = { "Reinitialisation" } } )
            end
        ),

        {
            events.client:UserChose(), {},
            function( dialog, answer )
                if dialog == "init_choice" then
                    if answer == "Reinitialisation" then
                        ChangeState( "init" )
                    end
                end
            end
        },

        AtState( "init",
            function()
                -- TODO REINITIALISATION
            end
        ),

        {
            events.sim:ClientLeft(),
            { },
            function( client )
                plugin:Reset()
            end
        }
    }
    DeclareEvents( eventTable )

end
        </xsl:text>
    </xsl:template>

</xsl:stylesheet>
