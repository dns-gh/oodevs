brain "brain" {}

dofile "resources/scripts/commands.lua"
dofile "resources/scripts/events.lua"
include "resources/data.lua"

scriptName = "Test"

function Start()

    local eventTable =
    {
        {
            events.sim:ClientConnected(),
            { },
            function( client, profile )
                if profile ~= "Test" then return end
                ResetOptions( { client = client } )
                SetDock( { client = client, hide = docks.ALL } ) -- hide all docks
                SetDock( { show = { "orbat" } } )
                SetDock( { show = { "clock" } } )
                sim:ChangeTimeFactor(75)
                ChangeState( "C9" )
            end
        },


        {
            events:Once(),
            { "C9" },
            function()
                Display( { message = "A main concern in training is the performance assessment. Was I successful ? Why ?" } )
                Center( { coordinates = drawings:Create( "C9_center" ):GetCoordinates()[1] } )
                Zoom( { width = 50000 } )
                SetDock( { show = { "score" } } )
                actions:IssueOrder( "Blue - Defense" )
                ChangeState( "C9_begin" )
            end
        },

        {
            events.agents:AgentEnters( drawings:Create( "C9_Gennes" ):GetCoordinates() ),
            { "C9_begin" },
            function( entity, area )
                local id = entity:GetIdentifier()
                for _, v in ipairs( Defense ) do
                    if id == v then
                       Display( { message = "Intels report a dense activity of Red units north to the river" } )
                       actions:IssueOrder( "Red - scout" )
                       ChangeState( "C9_scout" )
                    end
                end
            end
        },

        {
            events.agents:AgentEnters( drawings:Create( "C9_River" ):GetCoordinates() ),
            { "C9_scout" },
            function( entity, area )
                local id = entity:GetIdentifier()
                for _, v in ipairs( Red_scout ) do
                    if id == v then
                       Display( { message = "Players can assess at any moment the current value, the tendency and the quality of these KPIs with tables and charts" } )
                       actions:IssueOrder( "Red - attack" )
                       actions:IssueOrder( "Blue Battalion - move" )
                       actions:IssueOrder( "C9_move" )
                       ChangeState( "C9_attack" )
                    end
                end
            end
        },

        {
            events.agents:AgentEnters( drawings:Create( "C9_North" ):GetCoordinates() ),
            { "C9_attack" },
            function( entity, area )
                local id = entity:GetIdentifier()
                for _, v in ipairs( Red_attack ) do
                    if id == v then
                       Display( { message = "These scores can be exported in a .csv file, or in a html page, to generate evaluation reports and integrate these training sessions in a global learning process" } )
                       actions:IssueOrder( "Blue Battalion - cattack" )
                       ChangeState( "C9_cattack" )
                    end
                end
            end
        },

        {
            events.agents:AgentEnters( drawings:Create( "C9_East" ):GetCoordinates() ),
            { "C9_cattack" },
            function( entity, area )
                local id = entity:GetIdentifier()
                for _, v in ipairs( Blue_cattack ) do
                    if id == v then
                       Display( { message = "Scores can also be used in scripting and to automatically define success/failure factors. In this example, if half the red forces are destroyed, the exercise is considered to be successful" } )
                       actions:IssueOrder( "Blue - cattack" )
                       ChangeState( "C9_end" )
                    end
                end
            end
        },

        {
            events.indicators:IndicatorChanged(),
            { "C9_end" },
            function( name, value )
                if name == "Red Attack" then
                   if value <= 0.5 then
                      Briefing( { id = "debriefing", file = "resources/debriefing.html" } )
                      actions:IssueOrder( "Red - withdraw" )
                      sim:Pause()
                      ChangeState( "C9_after_end" )
                   end
                end
            end
        },

        {
            events.agents:AgentEnters( drawings:Create( "C9_Withdraw" ):GetCoordinates() ),
            { "C9_after_end" },
            function( entity, area )
                local id = entity:GetIdentifier()
                for _, v in ipairs( Red_attack ) do
                    if id == v then
                       Display( { message = "End of the demo" } )
                       ChangeState( "C9_finished" )
                    end
                end
            end
        },

    }
    DeclareEvents( eventTable )


end












