brain "brain" {}

require "io"
require "table"

dofile "resources/scripts/commands.lua"
dofile "resources/scripts/events.lua"

function Start()

    local eventTable =
    {
        {
            events.sim:ClientConnected(),
            { },
            function( client, profile )
                if profile ~= "object_destruction" then return end
                ChangeState( "od_startup" )
            end
        },

        {
            events.sim:TickEnded(),
            { "od_startup" },
            function()
                Trace( "od_startup" )
                ChangeState( "od_test_destruction" )
            end
        },

        {
            events.objects:ObjectDestroyed(),
            { "od_test_destruction" },
            function( entity )
                Trace( "od_destroyed" )
                Display( { message = "Object " .. entity:GetName() .. " (id=" .. tostring( entity:GetIdentifier() ) .. ") of type " .. entity:GetType() .. " has been destroyed." } )
                Deactivate()
            end
        }
    }

    DeclareEvents( eventTable )
end
