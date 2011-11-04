brain "brain" {}

dofile "resources/scripts/commands.lua"
dofile "resources/scripts/events.lua"
include "resources/data.lua"

function Start()

    local eventTable =
    {
        {
            events.sim:ClientConnected(),
            { },
            function( client, profile )
                if profile ~= "Free demo" then return end
                ChangeState( "waiting" )
            end
        },

        {
            events.agents:AgentEnters( drawings:Create( "Fontaine" ):GetCoordinates() ),
            { "waiting" },
            function( entity, area )
                local id = entity:GetIdentifier()
                for _, v in ipairs( enemyArmorUnits ) do
                    if id == v then
                       actions:IssueOrder( "Militia - Change population ROE" )
                       actions:IssueOrder( "Asymetric - attack population (in an area)" )
                       ChangeState( "militia_attack" )
                    end
                end
            end
        },

        {
            events.agents:AgentEnters( drawings:Create( "Beaufort" ):GetCoordinates() ),
            { "militia_attack" },
            function( entity, area )
                local id = entity:GetIdentifier()
                for _, v in ipairs( militiaUnits ) do
                    if id == v then
                       actions:IssueOrder( "Population - move" )
                       ChangeState( "pop_move" )
                    end
                end
            end
        },

                {
            events.populations:PopulationEnters( drawings:Create( "South_beaufort" ):GetCoordinates() ),
            { "pop_move" },
            function( entity, area )
                       actions:IssueOrder( "filtrate populations" )
                       ChangeState( "filtrate" )
            end
        },


        {
            events.populations:PopulationEnters( drawings:Create( "Bridge" ):GetCoordinates() ),
            { "filtrate" },
            function( entity, area )
                       actions:IssueOrder( "deny population crossing" )
                       actions:IssueOrder( "Asymetric - infiltrate" )
                       ChangeState( "infiltrate" )
            end
        },

        {
            events.agents:AgentEnters( drawings:Create( "Bridge" ):GetCoordinates() ),
            { "infiltrate" },
            function( entity, area )
                local id = entity:GetIdentifier()
                for _, v in ipairs( terroUnits ) do
                    if id == v then
                       actions:IssueOrder( "Engineer - build camps" )
                       ChangeState( "camp" )
                    end
                end
            end
        },

        {
            events.agents:AgentEnters( drawings:Create( "Blocus" ):GetCoordinates() ),
            { "camp" },
            function( entity, area )
                local id = entity:GetIdentifier()
                for _, v in ipairs( engineerUnits ) do
                    if id == v then
                       actions:IssueOrder( "Cancel maneuver" )
                       ChangeState( "cancel" )
                    end
                end
            end
        },

        {
            events.populations:PopulationEnters( drawings:Create( "Louerre" ):GetCoordinates() ),
            { "cancel" },
            function( entity, area )
                       actions:IssueOrder( "Asymetric - attack2" )
                       ChangeState( "militia_attack2" )
            end
        },

        {
            events.agents:AgentEnters( drawings:Create( "Bridge" ):GetCoordinates() ),
            { "militia_attack2" },
            function( entity, area )
                local id = entity:GetIdentifier()
                for _, v in ipairs( militiaUnits ) do
                    if id == v then
                       actions:IssueOrder( "Engineer - perform counter-mobility work" )
                       ChangeState( "barricade" )
                    end
                end
            end
        },

        {
            events.agents:AgentEnters( drawings:Create( "Withdraw" ):GetCoordinates() ),
            { "barricade" },
            function( entity, area )
                local id = entity:GetIdentifier()
                for _, v in ipairs( engineerUnits ) do
                    if id == v then
                       actions:IssueOrder( "control Populations" )
                       ChangeState( "control" )
                    end
                end
            end
        },

        {
            events.agents:AgentEnters( drawings:Create( "Louerre" ):GetCoordinates() ),
            { "control" },
            function( entity, area )
                local id = entity:GetIdentifier()
                for _, v in ipairs( friendlyUnits ) do
                    if id == v then
                       ChangeState( "end" )
                    end
                end
            end
        },


        AtState( "end",
            function()
                Display( { message = "Super" } )
                Deactivate()
            end
        )

    }

    DeclareEvents( eventTable )
end
