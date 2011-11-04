brain "brain" {}

dofile "resources/scripts/commands.lua"
dofile "resources/scripts/events.lua"
include "resources/data.lua"

scriptName = "Total"

function Start()

    local eventTable =
    {
        {
            events.sim:ClientConnected(),
            { },
            function( client, profile )
                if profile ~= "Scripted demo" then return end
                ResetOptions( { client = client } )
                SetDock( { client = client, hide = docks.ALL } ) -- hide all docks
                SetDock( { show = { "orbat" } } )
                SetDock( { show = { "clock" } } )
                Briefing( { id = "briefing", file = "resources/briefing.html" } )
                sim:ChangeTimeFactor(75)
                ChangeState( "Q1" )
            end
        },

        AtState( "Q1",
            function()
                local text = "Do you want to jump ?"
                Dialog( { id = "Q1_choice", message = text, buttons = { "Yes", "No" } } )
            end
        ),

        {
            events.client:UserChose(),
            { },
            function( dialog, answer )
                if dialog == "Q1_choice" then
                    if answer == "No" then
                        ChangeState( "C1" )
                    else
                        ChangeState( "Q2" )
                    end

                end
            end
        },

        AtState( "Q2",
            function()
                local text = "Jump to ?"
                Dialog( { id = "Q2_choice", message = text, buttons = { "C8", "C4" } } )
            end
        ),

        {
            events.client:UserChose(),
            { },
            function( dialog, answer )
                if dialog == "Q2_choice" then
                    if answer == "C8" then
                        sim:Resume()
                        ChangeState( "C8" )
                    else
                        ChangeState( "C4" )
                    end

                end
            end
        },


        {
            events:Once(),
            { "C1" },
            function()
                sim:Resume()
                -- actions:IssueOrder( "Armor - attack - C1" )
                Display( { message = "Units are controlled using parameterizable missions: danger direction, route, objective, boundary limit, phase line, etc.\n"
                                  .. "Please order this MBT Platoon to attack north" } )
                Center( { coordinates = drawings:Create( "C1_center" ):GetCoordinates()[1] } )
                Zoom( { width = 40000 } )
                Select( { target = "id://agent/" .. C1[1].id } )
                ChangeState( "C1_mid" )
            end
        },

        {
            events.agents:AgentEnters( drawings:Create( "C1_draw_mid" ):GetCoordinates() ),
            { "C1_mid" },
            function( entity, area )
                local id = entity:GetIdentifier()
                for _, v in ipairs( C1 ) do
                    if id == v.id then
                       Select( { target = "id://agent/" .. C1[1].id } )
                       Display( { message = "The platoon has automatically chosen its path according to its knowledge of the terrain" } )
                       ChangeOptions( { Paths = options.fourstate.selected, OldPaths = options.fourstate.selected} )
                       ChangeState( "C1_end" )
                    end
                end
            end
        },

        {
            events.agents:AgentEnters( drawings:Create( "C1_draw_end" ):GetCoordinates() ),
            { "C1_end" },
            function( entity, area )
                local id = entity:GetIdentifier()
                for _, v in ipairs( C1 ) do
                    if id == v.id then
                       -- Display( { message = "C1 : End" } )
                       ChangeState( "C2" )
                    end
                end
            end
        },

        {
            events:Once(),
            { "C2" },
            function()
                actions:IssueOrder( "Armor - attack - C2" )
                Display( { message = "Boundary limits can be used to restrict the operating range of a mission" } )
                Center( { coordinates = drawings:Create( "C2_center" ):GetCoordinates()[1] } )
                Zoom( { width = 40000 } )
                Select( { target = "id://agent/" .. C2[1].id } )
                ChangeState( "C2_mid2" )
            end
        },

        {
            events.agents:AgentEnters( drawings:Create( "C2_draw_mid2" ):GetCoordinates() ),
            { "C2_mid2" },
            function( entity, area )
                local id = entity:GetIdentifier()
                for _, v in ipairs( C2 ) do
                    if id == v.id then
                       Display( { message = "The platoon automatically avoids these blue obstacles as he knows their position" } )
                       Zoom( { width = 30000 } )
                       ChangeState( "C2_end" )
                    end
                end
            end
        },
        
        {
            events.agents:AgentEnters( drawings:Create( "C2_draw_end" ):GetCoordinates() ),
            { "C2_end" },
            function( entity, area )
                local id = entity:GetIdentifier()
                for _, v in ipairs( C2 ) do
                    if id == v.id then
                       -- Display( { message = "C2 : End" } )
                       ChangeState( "C3" )
                    end
                end
            end
        },
        
        {
            events:Once(),
            { "C3" },
            function()
                actions:IssueOrder( "Armor - attack - C3" )
                Display( { message = "Depending on the relief and its sensoring capacity, the platoon can: Identity (white), Spot (light blue) or Detect (dark blue) ennemy units and objects" } )
                Center( { coordinates = drawings:Create( "C3_center" ):GetCoordinates()[1] } )
                Zoom( { width = 40000 } )
                Select( { target = "id://agent/" .. C3[1].id } )
                ChangeOptions( { VisionSurfaces = options.fourstate.selected } )
                ChangeState( "C3_mid" )
            end
        },

        {
            events.agents:AgentEnters( drawings:Create( "C3_draw_mid" ):GetCoordinates() ),
            { "C3_mid" },
            function( entity, area )
                local id = entity:GetIdentifier()
                for _, v in ipairs( C3 ) do
                    if id == v.id then
                       SetFilter ({ target = "id://agent/" .. C3[1].id } )
                       Display( { message = "For the moment, the platoon has no knowledge of the presence of ennemy forces. The circle in dot line around the platoon is its maximum weapon range" } )
                       -- Fog of war
                       ChangeState( "C3_mid2" )
                    end
                end
            end
        },

        {
            events.agents:AgentEnters( drawings:Create( "C3_draw_mid2" ):GetCoordinates() ),
            { "C3_mid2" },
            function( entity, area )
                local id = entity:GetIdentifier()
                for _, v in ipairs( C3 ) do
                    if id == v.id then
                       Display( { message = "The platoon reacts automatically, according to its mission, environment (force ratio, knowledge, ...), capacities (movement, weaponry, ...), operational status, human factors ..." } )
                       ChangeState( "C3_end" )
                    end
                end
            end
        },
        
        {
            events.agents:AgentEnters( drawings:Create( "C3_draw_end" ):GetCoordinates() ),
            { "C3_end" },
            function( entity, area )
                local id = entity:GetIdentifier()
                for _, v in ipairs( C3 ) do
                    if id == v.id then
                       -- Display( { message = "C3 : End" } )
                       ChangeState( "C4" )
                    end
                end
            end
        },

        {
            events:Once(),
            { "C4" },
            function()
                ClearFilter()
                actions:IssueOrder( "Armor - scout - C4" )
                Display( { message = "This time the platoon receives a <b>scout</b> mission instead of an <b>attack</b> mission" } )
                Center( { coordinates = drawings:Create( "C4_center" ):GetCoordinates()[1] } )
                Zoom( { width = 40000 } )
                Select( { target = "id://agent/" .. C4[1].id } )
                ChangeState( "C4_mid" )
            end
        },
        
        {
            events.agents:AgentEnters( drawings:Create( "C4_draw_mid" ):GetCoordinates() ),
            { "C4_mid" },
            function( entity, area )
                local id = entity:GetIdentifier()
                for _, v in ipairs( C4 ) do
                    if id == v.id then
                       SetFilter ({ target = "id://agent/" .. C4[1].id } )
                       Display( { message = "Its behavior will be different than before, as it will try to avoid contact and take time to observe ennemy units" } )
                       ChangeState( "C4_end" )
                    end
                end
            end
        },

        {
            events.agents:AgentEnters( drawings:Create( "C4_draw_end" ):GetCoordinates() ),
            { "C4_end" },
            function( entity, area )
                local id = entity:GetIdentifier()
                for _, v in ipairs( C4 ) do
                    if id == v.id then
                       -- Display( { message = "C4 : End" } )
                       ChangeState( "C5" )
                    end
                end
            end
        },

        {
            events:Once(),
            { "C5" },
            function()
                ClearFilter()
                actions:IssueOrder( "Armor - attack - C5" )
                Display( { message = "It is also possible to give a mission to an entire company: a commanding automat will understand and decompoze this mission regarding the specificities of the units under its command" } )
                Center( { coordinates = drawings:Create( "C5_center" ):GetCoordinates()[1] } )
                ChangeOptions( { VisionSurfaces = options.fourstate.off, MissionParameters = options.fourstate.superior, Paths = options.fourstate.superior, OldPaths = options.fourstate.off } )
                Zoom( { width = 40000 } )
                Select( { target = "id://automat/" .. C52[1].id } )
                ChangeState( "C5_mid" )
            end
        },

        {
            events.agents:AgentEnters( drawings:Create( "C5_draw_mid" ):GetCoordinates() ),
            { "C5_mid" },
            function( entity, area )
                local id = entity:GetIdentifier()
                for _, v in ipairs( C5 ) do
                    if id == v.id then
                       SetFilter ({ target = "id://automat/" .. C52[1].id } )
                       Display( { message = "From this company level attack mission, two MBT platoons receive an attack mission, two follow and the mortar supports them.\n"
                                         .. "This formation will dynamically evolve with the course of action: if an attacking MBT platoon is destroyed, the following one will take its place" } )
                       ChangeState( "C5_mid2" )
                    end
                end
            end
        },

        {
            events.agents:AgentEnters( drawings:Create( "C5_draw_mid2" ):GetCoordinates() ),
            { "C5_mid2" },
            function( entity, area )
                local id = entity:GetIdentifier()
                for _, v in ipairs( C5 ) do
                    if id == v.id then
                       Display( { message = "Units share their knowledge through <b>Knowledge groups.</b> Each unit of a knowledge group shares immediatly its knowledge with every unit included in this knowledge group" } )
                       ChangeState( "C5_end" )
                    end
                end
            end
        },

        {
            events.agents:AgentEnters( drawings:Create( "C5_draw_end" ):GetCoordinates() ),
            { "C5_end" },
            function( entity, area )
                local id = entity:GetIdentifier()
                for _, v in ipairs( C5 ) do
                    if id == v.id then
                       Display( { message = "C5 : End" } )
                       ChangeState( "C7" )
                    end
                end
            end
        },
        

        {
            events:Once(),
            { "C6" },
            function()
                ClearFilter()
                actions:IssueOrder( "Battalion - Attack - C6" )
                Display( { message = "Battalion level commanding automats allow to control numerous units with a few clicks. This attack mission is logically decompozed to companies and platoons" } )
                Center( { coordinates = drawings:Create( "C6_center" ):GetCoordinates()[1] } )
                Zoom( { width = 40000 } )
                Select ({ target = "id://automat/" .. C62[1].id } )
                ChangeState( "C6_mid" )
            end
        },

        {
            events.agents:AgentEnters( drawings:Create( "C6_draw_mid" ):GetCoordinates() ),
            { "C6_mid" },
            function( entity, area )
                local id = entity:GetIdentifier()
                for _, v in ipairs( C6 ) do
                    if id == v then
                       Display( { message = "Boundary lines are automatically created by commanding automats to organize the formation of their company/battalion" } )
                       Zoom( { width = 30000 } )
                       ChangeOptions( { Paths = options.fourstate.off } )
                       ChangeState( "C6_mid2" )
                    end
                end
            end
        },

        {
            events.agents:AgentEnters( drawings:Create( "C6_draw_mid2" ):GetCoordinates() ),
            { "C6_mid2" },
            function( entity, area )
                local id = entity:GetIdentifier()
                for _, v in ipairs( C6 ) do
                    if id == v then
                       Display( { message = "A unit can be dynamically engaged (ie. controled by a commanding automat, symbolized by a closed padlock) or disengaged (manually controled)" } )
                       Zoom( { width = 20000 } )
                       ChangeState( "C6_end" )
                    end
                end
            end
        },

        {
            events.agents:AgentEnters( drawings:Create( "C6_draw_end" ):GetCoordinates() ),
            { "C6_end" },
            function( entity, area )
                local id = entity:GetIdentifier()
                for _, v in ipairs( C6 ) do
                    if id == v then
                       -- Display( { message = "C6 : End" } )
                       ChangeState( "C7" )
                    end
                end
            end
        },
        
        {
            events:Once(),
            { "C7" },
            function()
                ClearFilter()
                Display( { message = "Orders can be saved and loaded in a timeline. Some orders have been pre-recorded for these units. Please load the 7.ord file in the orders folder\n"
                                  .. "The red line matchs the simulation time. You can slide it to move in time. Please change it to 22:30" } )
                Center( { coordinates = drawings:Create( "C7_center" ):GetCoordinates()[1] } )
                Zoom( { width = 40000 } )
                SetDock( { show = { "Actions timeline" } } )
                SetDock( { show = { "actions" } } )
                ChangeOptions( { MissionParameters = options.fourstate.off} )
                ChangeState( "C7_mid" )
            end
        },
        
        {
            events.agents:AgentEnters( drawings:Create( "C7_draw_mid" ):GetCoordinates() ),
            { "C7_mid" },
            function( entity, area )
                local id = entity:GetIdentifier()
                for _, v in ipairs( C7_gauche_all ) do
                    if id == v then
                       Display( { message = "User can add, move, delete, modify orders in the timeline to create a full storyboard of his scenario" } )
                       Zoom( { width = 30000 } )
                       ChangeState( "C7_mid2" )
                    end
                end
            end
        },
        
        {
            events.agents:AgentEnters( drawings:Create( "C7_draw_mid2" ):GetCoordinates() ),
            { "C7_mid2" },
            function( entity, area )
                local id = entity:GetIdentifier()
                for _, v in ipairs( C7_gauche_all ) do
                    if id == v then
                       SetDock( { hide = { "Actions timeline" } } )
                       SetDock( { hide = { "actions" } } )
                       SetDock( { show = { "info" } } )
                       Display( { message = "In the left side, red units received their 'Hold mission' too late to prepare their positions and are surprised by the blue units,\n"
                                         .. "while in the right side red units have time to install and prepare their defence, denying the advance of the blue units" } )
                       Center( { coordinates = drawings:Create( "C7_center2" ):GetCoordinates()[1] } )
                       Select ({ target = "id://agent/" .. C71[1].id } )
                       ChangeState( "C7_mid3" )
                    end
                end
            end
        },
        
        {
            events.agents:AgentEnters( drawings:Create( "C7_draw_mid3" ):GetCoordinates() ),
            { "C7_mid3" },
            function( entity, area )
                local id = entity:GetIdentifier()
                for _, v in ipairs( C7_droite_all ) do
                    if id == v then
                       Display( { message = "The operational state of a unit represents its capacity to fulfill its mission. An operationally destroyed unit will look for \n"
                                         .. "safety and maintenance, while a fully destroyed unit cannot interact with the simulation anymore (magic actions excepted)" } )
                       Select ({ target = "id://automat/" .. C72[1].id } )
                       SetFilter ({ target = "id://automat/" .. C72[1].id } )
                       ChangeState( "C7_end" )
                    end
                end
            end
        },

        {
            events.agents:AgentEnters( drawings:Create( "C7_draw_end" ):GetCoordinates() ),
            { "C7_end" },
            function( entity, area )
                local id = entity:GetIdentifier()
                for _, v in ipairs( C7_gauche_last ) do
                    if id == v then
                       -- Display( { message = "C7 : End" } )
                       SetDock( { hide = { "info" } } )
                       ChangeState( "C8" )
                    end
                end
            end
        },
        
                {
            events:Once(),
            { "C8" },
            function()
                ClearFilter()
                Display( { message = "Scenarios can also be scripted in a 'If ... Then' mode. Actions can be set off by triggers, like the position of a unit, its status, or an action issued by a user" } )
                Center( { coordinates = drawings:Create( "C8_center" ):GetCoordinates()[1] } )
                Zoom( { width = 50000 } )
                actions:IssueOrder( "Invasion" )
                ChangeState( "C8_begin" )
            end
        },


--Script OOTW

      {
            events.agents:AgentEnters( drawings:Create( "Fontaine" ):GetCoordinates() ),
            { "C8_begin" },
            function( entity, area )
                local id = entity:GetIdentifier()
                for _, v in ipairs( enemyArmorUnits ) do
                    if id == v then
                       Display( { message = "Reacting to the red invasion, red-supporting militias begin to harass local populations. Non-military units can conduct specific missions and intervene on the course of actions" } )
                       actions:IssueOrder( "Militia - Change population ROE" )
                       actions:IssueOrder( "Asymetric - attack population (in an area)" )
                       ChangeOptions( { MissionParameters = options.fourstate.selected } )
                       Select ({ target = "id://agent/" .. militia1[1].id } )
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
                       Display( { message = "Population are represented as special entities that move like a fluid, its size depending on the number of people in this population. \n"
                                       ..   "Populations can suffer casualities from direct and indirect actions, demonstrate, delay logistic trucks ..." } )
                       actions:IssueOrder( "Population - move" )
                       Center( { coordinates = drawings:Create( "C8_center2" ):GetCoordinates()[1] } )
                       Zoom( { width = 30000 } )
                    -- Select Pop
                       ChangeState( "pop_move" )
                    end
                end
            end
        },

                        {
            events.populations:PopulationEnters( drawings:Create( "South_beaufort" ):GetCoordinates() ),
            { "pop_move" },
            function( entity, area )
                       Display( { message = "Populations can also be escorted, filtered, hosted in refugee camps ... Peacekeeping blue unit creates a checkpoint to filter this population and prevent hostile units from crossing the bridge" } )
                       Select ({ target = "id://agent/" .. Check[1].id } )
                       actions:IssueOrder( "filtrate populations" )
                       ChangeState( "filtrate" )
            end
        },

{
            events.populations:PopulationEnters( drawings:Create( "Bridge" ):GetCoordinates() ),
            { "filtrate" },
            function( entity, area )
                       Display( { message = "Terrorist units can infiltrate populations to conduct attacks, bombings and destructions. Once infiltrated, they can't be detected except by operated checkpoints" } )
                       actions:IssueOrder( "deny population crossing" )
                       actions:IssueOrder( "Population - move2" )
                       actions:IssueOrder( "Asymetric - infiltrate" )
                       Select ({ target = "id://agent/" .. terroUnits[1].id } )
                       ChangeState( "infiltrate" )
            end
        },
        
        {
            events.agents:AgentEnters( drawings:Create( "Bridge" ):GetCoordinates() ),
            { "infiltrate" },
            function( entity, area )
                local id = entity:GetIdentifier()
                for _, v in ipairs( terro ) do
                    if id == v then
                       actions:IssueOrder( "Engineer - build camps" )
                       Center( { coordinates = drawings:Create( "C8_center3" ):GetCoordinates()[1] } )
                       Zoom( { width = 20000 } )
                       Select ({ target = "id://agent/" .. engineer[1].id } )
                       ChangeState( "camp" )
                    end
                end
            end
        },
        
        AtState( "camp",
			function()
					StartTimeSequence( "wait_camp" )
			end
		),

        TimeSequence( "wait_camp", 20,
			function()
                      Display( { message = "Populations can be taken care of in refugee camps. Without security, medical attention and supplies, a population can \n"
                      ..   "change its behavior and become hostile. A camp is built by engineers while blue units control the progression of the population" } )
			end
		),

		TimeSequenceEnd( "wait_camp",
			function()
                ChangeState( "camp_end" )
			end
		),


        AtState( "camp_end",
            function()
                 Display( { message = "Population management can an important success or failure factor of a scenario" } )
                 actions:IssueOrder( "Engineer - open a route" )
                 ChangeState( "camp2" )
            end
        ),

        {
            events.agents:AgentEnters( drawings:Create( "Blocus" ):GetCoordinates() ),
            { "camp2" },
            function( entity, area )
                local id = entity:GetIdentifier()
                for _, v in ipairs( engineerUnits ) do
                    if id == v then
                       Display( { message = "Military units have specific rules of engagement and diplomacy towards populations" } )
                       actions:IssueOrder( "Population - move" )
                       actions:IssueOrder( "move_blue" )
                       ChangeState( "cancel" )
                    end
                end
            end
        },

               {
            events.populations:PopulationEnters( drawings:Create( "Louerre" ):GetCoordinates() ),
            { "cancel" },
            function( entity, area )
                       Display( { message = "Blue units will thus protect green populations, but shoot at sight red units" } )
                       actions:IssueOrder( "Asymetric - attack2" )
                       actions:IssueOrder( "Engineer - perform work" )
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
                       StartTimeSequence( "wait_abatis" )
                    end
                end
            end
        },


        TimeSequence( "wait_abatis", 35,
			function()
                      Display( { message = "Engineers have a wide range of capacities, from mine clearing to counter-mobility"} )
			end
		),

		TimeSequenceEnd( "wait_abatis",
			function()
                ChangeState( "abatis_end" )
			end
		),


        AtState( "abatis_end",
            function()
                 Display( { message = "The crossing of the bridge is thus denied" } )
                 actions:IssueOrder( "withdraw" )
                 ChangeState( "withdraw" )
            end
        ),

        {
            events.agents:AgentEnters( drawings:Create( "Withdraw" ):GetCoordinates() ),
            { "withdraw" },
            function( entity, area )
                local id = entity:GetIdentifier()
                for _, v in ipairs( engineerUnits ) do
                    if id == v then
                       actions:IssueOrder( "control Populations" )
                       actions:IssueOrder( "Infantry - control defensive" )
                       StartTimeSequence( "wait_C9" )
                    end
                end
            end
        },
        
        TimeSequence( "wait_C9", 20,
			function()
                       Display( { message = "Populations' entropy is defined by their 'Domination'. Specific missions can change this domination and allow control of these populations, but can imply casualties" } )
			end
		),
		
		TimeSequenceEnd( "wait_C9",
			function()
                ChangeState( "C9" )
			end
		),


-- Script OOTW end


        
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
                       Display( { message = "Key Performance Indicators can be defined to give a real-time picture of the situation and its evolution. In this example, we monitor " } )
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






    



    
    
