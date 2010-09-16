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
                sim:Pause()
            end
        },
    
        {
            events.sim:ClientConnected(),
            { },
            function( client, profile )
                if profile ~= "</xsl:text><xsl:value-of select="$profilename"/><xsl:text>" then Deactivate() return end
                sim:Resume()
                ResetOptions( { client = client } )
                SetDock( { client = client, hide = docks.ALL } )
                SetDock( { show = { "orbat" } } )
                SetDock( { show = { "clock" } } )
                ChangeState( "selectMode" )
            end
        },

        -- INITIALISATION
        
        AtState( "selectMode",
            function()
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
                        ChangeState( "autoexec" )
                    else
                        ChangeState( "initDialog" )
                    end

                end
            end
        },
        
        AtState( "autoexec",
            function()
        </xsl:text>

        <xsl:for-each select="action">
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
        -- REINITIALISATION
        AtState( "init",
            function()
                -- TODO
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
