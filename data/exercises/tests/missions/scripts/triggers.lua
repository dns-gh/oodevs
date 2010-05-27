brain "brain" {}

require "io"
require "table"

dofile "resources/scripts/commands.lua"
dofile "resources/scripts/actions.lua"
dofile "resources/scripts/events.lua"
include "resources/model.lua"
include "resources/config.lua"

function Start()

    local currentTick = 0

    local eventTable =
    {
        {
            events.sim:ClientConnected(),
            { },
            function( client, profile )
                if profile ~= "triggers" then return end
                ChangeState( "trigger_startup" )
            end
        },

        {
            events.sim:TickEnded(),
            { "trigger_startup" },
            function()
                currentTick = currentTick + 1
                Display( { message = "Current tick is " .. tostring( currentTick ) } )
                if currentTick > 19 then
                    actions:IssueOrder( "triggers_red" )
                    sim:CreateUnit( coord:UtmPosition( config.positions.start[1] )
                              , model.types.units["MBT platoon"]
                              , model.entities.automats["ABC"] )
                    ChangeState( "trigger_test_creation" )
                end
            end
        },

        {
            events.agents:AgentCreated(),
            { "trigger_test_creation" },
            function( entity )
				Mission.create( entity:GetIdentifier(), model.types.missions["move"] )
						:With( { name = "Danger direction", type = "Direction", value = 0 } )
						:With( Path.create( "Route" ):AddPoint( "Destination", config.positions.destination[1] ) )
						:Issue()
                ChangeState( "trigger_test_position" )
            end
        },
        
        {
            events.agents:AgentEnters( drawings:Create( "trigger_p1" ):GetCoordinates() ),
            { "trigger_test_position" },
            function( agent )
                Display( { message = "Unit " .. agent:GetIdentifier() .. " entered trigger area P1" } )
                sim:Pause()
                ChangeState( "trigger_test_detection" )
            end
        },
	
        {
            events.agents:KnowledgeCreated(),
            { "trigger_test_detection" },
            function( knowledge, agent )
                Display( { message = "Knowledge " .. tostring( knowledge:GetIdentifier() ) .. " created" } )
                sim:Pause()
                ChangeState( "trigger_test_identification" )
            end
        },
	
        {
            events.agents:PerceptionChanged(),
            { "trigger_test_identification" },
            function( knowledge, entity )
                Display( { message = "Knowledge " .. tostring( knowledge:GetIdentifier() ) .. " perception level changed" } )
                sim:Pause()
                ChangeState( "trigger_test_operational_state" )
            end
        },
	
        {
            events.agents:OperationalStateChanged(),
            { "trigger_test_operational_state" },
            function( entity )
                if entity:GetOperationalState() < 100 then
                    Display( { message = "Unit " .. tostring( entity:GetIdentifier() ) .. " operational state is " .. tostring( entity:GetOperationalState() ) } )
                    sim:Pause()
                    ChangeState( "trigger_test_end" )
                end
            end
        },
	
		AtState( "trigger_test_end",
            function()
                Deactivate()
            end
        )

    }

    DeclareEvents( eventTable )
end
