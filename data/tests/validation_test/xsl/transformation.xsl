<xsl:stylesheet version = '1.0' xmlns:xsl='http://www.w3.org/1999/XSL/Transform'
                xmlns:lxslt="http://xml.apache.org/xslt"
                xmlns:redirect="http://xml.apache.org/xalan/redirect"
                extension-element-prefixes="redirect"
                xmlns:url="http://whatever/java/java.net.URLEncoder"
                exclude-result-prefixes="url">
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
            events.sim:ClientConnected(),
            { },
            function( client, profile )
                if profile ~= "</xsl:text><xsl:value-of select="$profilename"/><xsl:text>" then return end
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

        -- EXECUTION DES ACTIONS

        -- REINITIALISATION

    }
    DeclareEvents( eventTable )

end
        </xsl:text>
    </xsl:template>
</xsl:stylesheet>



    <!--
        {
            events.client:UserChose(),
            { },
            function( dialog, answer )
                if dialog == "Q1_choice" then
                    if answer == "manuel" then
                        ChangeState( "Q2" )
                    else
                        ChangeState( "Q2" )
                    end

                end
            end
        },

        AtState( "Q2",
            function()
                Dialog( { id      = "Q2_choice",
                          message = "", 
                          buttons = { "Reinitialisation de l'exercice" } } )
            end
        ),
   
-->

