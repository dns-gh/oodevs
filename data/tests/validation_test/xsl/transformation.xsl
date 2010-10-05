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
            events.sim:ClientConnected(),
            {},
            function( client, profile )
                if profile ~= "</xsl:text><xsl:value-of select="$profilename"/><xsl:text>" then
                    return
                end
                ClearDisplay()
                ChangeState( "</xsl:text><xsl:value-of select="$profilename"/><xsl:text>_waitstep" )
            end
        },
        
        {
            events.sim:TickEnded(),
            { "</xsl:text><xsl:value-of select="$profilename"/><xsl:text>_waitstep" },
            function()
                sim:Pause()
                ChangeState( "</xsl:text><xsl:value-of select="$profilename"/><xsl:text>_creations" )
            end
        },

        AtState( "</xsl:text><xsl:value-of select="$profilename"/><xsl:text>_creations",
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
                ChangeState( "</xsl:text><xsl:value-of select="$profilename"/><xsl:text>_autoormandialog" )
            end
        ),

        AtState( "</xsl:text><xsl:value-of select="$profilename"/><xsl:text>_autoormandialog",
            function()
                sim:Pause()
                Dialog( { id      = "selectMode_choice",
                          message = "Test de validation </xsl:text><xsl:value-of select="$profilename"/><xsl:text>. Selectionnez un mode de jeu :",
                          buttons = { "Automatique", "manuel" } } )
            end
        ),

        {
            events.client:UserChose(),
            { "</xsl:text><xsl:value-of select="$profilename"/><xsl:text>_autoormandialog" },
            function( dialog, answer )
                if dialog == "selectMode_choice" then
                    if answer == "Automatique" then
                        ChangeState( "</xsl:text><xsl:value-of select="$profilename"/><xsl:text>_play" )
                    else
                        ChangeState( "</xsl:text><xsl:value-of select="$profilename"/><xsl:text>_initDialog" )
                    end
                end
            end
        },

        AtState( "</xsl:text><xsl:value-of select="$profilename"/><xsl:text>_play",
            function()
                sim:Resume()
        </xsl:text>
        <xsl:for-each select='action[@name!="Unit Creation"]'>
                <xsl:text>
                actions:IssueOrderFromFile( "</xsl:text><xsl:value-of select="@name"/><xsl:text>", "</xsl:text><xsl:value-of select="$profilename"/><xsl:text>" )</xsl:text>
        </xsl:for-each>
        <xsl:text>
                ChangeState( "</xsl:text><xsl:value-of select="$profilename"/><xsl:text>_initDialog" )
            end
        ),

        AtState( "</xsl:text><xsl:value-of select="$profilename"/><xsl:text>_initDialog",
            function()
                Dialog( { id      = "init_choice",
                          message = "Test de validation </xsl:text><xsl:value-of select="$profilename"/><xsl:text>.",
                          buttons = { "Reset" } } )
            end
        ),

        {
            events.client:UserChose(),
            { "</xsl:text><xsl:value-of select="$profilename"/><xsl:text>_initDialog" },
            function( dialog, answer )
                if dialog == "init_choice" then
                    if answer == "Reset" then
                        Dialog( { id      = "init_wait",
                          message = "Test de validation </xsl:text><xsl:value-of select="$profilename"/><xsl:text>. Attendez avant de deconnecter.",
                          buttons = {} } )
                        </xsl:text>
                        <xsl:for-each select='action[@name="Unit Creation"]'>
                        <xsl:text>        
                        --local entity = model.orbat:FindAgent( </xsl:text><xsl:value-of select="@id"/><xsl:text> )
                        --entity:Teleport( coord:UtmPosition( "29TNF2072470458" ) )
                        -- entity:UnrecoverAll()</xsl:text>
                        <xsl:value-of select='./parameter[@name="UnitType"]/@value'/>
                        <xsl:text>", "</xsl:text>
                        <xsl:value-of select='@target'/>
                        <xsl:text>" )</xsl:text>
                        </xsl:for-each>

                        <xsl:variable name="orbat" select="document('../out/Poseidon VA/orbat.xml')"/>
                        <xsl:for-each select="$orbat//side/tactical/formation/automat/unit">
                        <xsl:text>
                        local entity = model.orbat:FindAgent( </xsl:text><xsl:value-of select="@id"/><xsl:text> )
                        if ( coord:ToUtm( entity:GetPosition() ) ~= "</xsl:text><xsl:value-of select="@position"/><xsl:text>" ) then
                            entity:Teleport( coord:UtmPosition( "</xsl:text><xsl:value-of select="@position"/><xsl:text>" ) )
                        end
                        entity:RecoverAll()
                        </xsl:text>
                        </xsl:for-each>
                        <xsl:text>
                        ChangeState( "</xsl:text><xsl:value-of select="$profilename"/><xsl:text>_clean" )
                    end
                end
            end
        },

        {
            events.sim:TickEnded(),
            { "</xsl:text><xsl:value-of select="$profilename"/><xsl:text>_clean" },
            function()
                Dialog( { id      = "ok_for_disconect",
                          message = "Test de validation </xsl:text><xsl:value-of select="$profilename"/><xsl:text>. OK pour deconnecter.",
                          buttons = {} } )
                Deactivate()
            end
        },
    }
    DeclareEvents( eventTable )

end
        </xsl:text>
    </xsl:template>

</xsl:stylesheet>
