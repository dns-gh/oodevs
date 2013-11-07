
brain "brain" {}

dofile "resources/scripts/commands.lua"
dofile "resources/scripts/events.lua"
dofile "resources/scripts/actions.lua"

scriptName = "success-factors"

function Start()

    local indicators = {}
    local factors = {

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
