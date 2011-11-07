brain "brain" {}

dofile "resources/scripts/commands.lua"
dofile "resources/scripts/events.lua"



function Start()

	local ExplosionSite = drawings:Create("Explosion"):GetCoordinates()
	local DeconSite = drawings:Create("Decontamination Area"):GetCoordinates()
	local Profile = ""
 	local eventTable =
	{
		{
            events.sim:ClientConnected(),
            { },
            function( client, profile )
				Profile = profile
                if profile == "x - Monterey3"	then
					--Message ({message = "Logged in as Monterey3"})
					Trace ("Logged in as Monterey3")
				elseif profile == "x - Salinas35" then
					--Message ({message = "Logged in as Salinas35"})
					Trace ("Logged in as Salinas35")
				elseif profile == "x - Laguna Seca77" then
					--Message ({message = "Logged in as Laguna Seca77"})
					Trace ("Logged in as Laguna Seca77")
				elseif profile == "Terrorists" then
					--Message ({message = "Logged in as Terrorists"})
					Trace ("Logged in as Terrorists")
				elseif profile == "Godseye" then
					--Message ({message = "Logged in as Godseye"})
					Trace ("Logged in as Godseye")
				else
					return
				end
				sim:Pause()
                Briefing( { id = "briefing", file = "resources/briefing.html" } )
				plugin:Later( function() fsm:ChangeState( "Briefing" ) end )
				--Message ({message = "Briefing up, sim paused"})
				Trace ("Briefing up, sim paused")
            end
        },

		{
			events.client:UserChose(),
			{ "Briefing" },
			function( dialog, answer )
				if dialog == "briefing" then
					if answer == "ok" then
					sim:Resume()
					plugin:Later( function() fsm:ChangeState( "Brief Done" ) end )
					script:ChangePhase( "Connected")
					--Message ({message = "Briefing done, sim unpaused"})
					Trace ("Briefing done, sim unpaused")
					end
				end
			end
		},

 		AtState( "Brief Done",
			function()
				Display( { message = "Terrorist approaching bombing site"})
				actions:IssueOrder("Asymetric - carry out a dirty bombing attack")
				--Message ({message = "Issuing order for terrorists to carry out attack"})
				Trace ("Issuing order for terrorists to carry out attack")
				plugin:Later( function() fsm:ChangeState( "Terrorists Attacking" ) end )
			end
 		),

		{
            events.agents:AgentEnters(ExplosionSite),
            { "Terrorists Attacking" },
            function( entity)
				if entity:GetTeam() == "Terrorists" then
					Display( { message = "Bomb exploded"})
					plugin:Later( function() fsm:ChangeState( "Explosion" ) end )
					--Message ({message = "Explosion"})
					Trace ("Explosion")
				end
            end
        },
		Wait ( "Explosion","NBC Protection", 5),

 		AtState( "NBC Protection",
			function()
				Display( { message = "Laguna Seca Units donning NBC Gear In Case of Dirty Bomb"})
				actions:IssueOrder("LS NBC suit on")
				--Message ({message = "Laguna Seca NBC suit"})
				Trace ("Laguna Seca NBC suit")
				StartTimeSequence( "Spending Time" )
				plugin:Later( function() fsm:ChangeState( "NBC Ready" ) end )
			end
		),
		Wait ( "NBC Ready","Need to Respond", 20),

 		AtState( "Need to Respond",
			function()
				Display( { message = "Laguna Seca Police Responding to Explosion, Evacuating Area"})
				actions:IssueOrder("Emergency - evacuate area 91")
				actions:IssueOrder("Emergency - evacuate area 90")
				actions:IssueOrder("Emergency - evacuate area 92")
				--Message ({message = "Local Police Response to Explosion"})
				Trace ("Local Police Response to Explosion")
				StartTimeSequence( "Spending Time" )
				plugin:Later( function() fsm:ChangeState( "Local Police Respond" ) end )
			end
		),
		Wait ( "Local Police Respond","Local Firefighter Call to Respond", 20),

 		AtState( "Local Firefighter Call to Respond",
			function()
				Display( { message = "Laguna Seca Firefighters Responding to Explosion, Extinguishing Fire and Evacuating"})
				actions:IssueOrder("Emergency - evacuate area  85")
				actions:IssueOrder("Emergency - evacuate area 86")
				actions:IssueOrder("Emergency - evacuate area 87")
				--Message ({message = "Local Firefighter Response to Explosion"})
				Trace ("Local Firefighter Response to Explosion")
				plugin:Later( function() fsm:ChangeState( "Local Firefighter Respond" ) end )
			end
		),
		Wait ( "Local Firefighter Respond","Local Medical Call to Respond", 20),

 		AtState( "Local Medical Call to Respond",
			function()
				Display( { message = "Laguna Seca Medical Responding to Explosion, Providing First Aid"})
				actions:IssueOrder("Emergency - provide first aid 79")
				actions:IssueOrder("Emergency - provide first aid 80")
				actions:IssueOrder("Emergency - provide first aid 81")
				--Message ({message = "Local Medical Providing First Aid"})
				Trace ("Local Medical Providing First Aid")
				plugin:Later( function() fsm:ChangeState( "Local Emergency Response Underway" ) end )
			end
		),
		Wait ( "Local Emergency Response Underway","Population Move", 30),

 		AtState( "Population Move",
			function()
				Display( { message = "Police beginning evacuation of population to safe area"})
				actions:IssueOrder("Population - move to safe area")
				--Message ({message = "Population moving to safe area"})
				Trace ("Population moving to safe area")
				plugin:Later( function() fsm:ChangeState( "Evacuate Population to Safe Area" ) end )
			end
		),
		Wait ( "Evacuate Population to Safe Area","Police Follow", 20),

 		AtState( "Police Follow",
			function()
				Display( { message = "Police escorting population to safe area"})
				actions:IssueOrder("Emergency - follow population 91")
				actions:IssueOrder("Emergency - follow population 90")
				actions:IssueOrder("Emergency - follow population 92")
				--Message ({message = "Police escorting population to safe area"})
				Trace ("Population moving to safe area")
				plugin:Later( function() fsm:ChangeState( "Police Escort to Safe Area" ) end )
			end
		),
		Wait ( "Police Escort to Safe Area","Time to Prompt for EOC", 20),

		AtState( "Time to Prompt for EOC",
			function()
                Dialog ({id = "EOCAlertPrompt", message = "To alert EOC to the bombing, click OK", buttons = {"OK","Cancel"}})
				plugin:Later( function() fsm:ChangeState( "EOC Prompt Done" ) end )
				--Message ({message = "Prompt User to Call EOC"})
				Trace ("Prompt User to Call EOC, sim paused")
			end
 		),

		{
			events.client:UserChose(),
			{ "EOC Prompt Done" },
			function( dialog, answer )
				if dialog == "EOCAlertPrompt" then
					if answer == "OK" then
					sim:Resume()
					plugin:Later( function() fsm:ChangeState( "EOC Alerted" ) end )
					--Message ({message = "EOC Prompted, sim unpaused"})
					Trace ("EOC Prompted, sim unpaused")
					end
				end
			end
		},
		AtState( "EOC Alerted",
			function()
				Display( { message = "Medical units moving to safe area"})
				actions:IssueOrder("move to safe area 80")
				actions:IssueOrder("move to safe area 79")
				actions:IssueOrder("move to safe area 81")
                Display( { message = "EOC Has Been Alerted, Units Being Dispatched from Monterey and Salinas"})
				plugin:Later( function() fsm:ChangeState( "Regional Response" ) end )
				--Message ({message = "EOC sending regional emergency units"})
				Trace ("EOC sending regional emergency units")
			end
 		),
		Wait ( "Regional Response","Monterey Response", 20),

		AtState( "Monterey Response",
			function()
				Display( { message = "Mobilizing units from Monterey"})
				actions:IssueOrder("Monterey Police 31 Emergency - block road")
				actions:IssueOrder("Monterey Police 32 move")
				actions:IssueOrder("Monterey Police 30 move")
				actions:IssueOrder("Monterey Fire 28 Emergency - evacuate area")
				actions:IssueOrder("Monterey Fire 24 Emergency - evacuate area")
				actions:IssueOrder("Monterey Fire 27 Emergency - evacuate area")
				actions:IssueOrder("Monterey Fire 26 Emergency - evacuate area")
				actions:IssueOrder("Monterey Fire 25 Emergency - evacuate area")
				actions:IssueOrder("Monterey Medical 19 Emergency - provide first aid")
				actions:IssueOrder("Monterey Medical 20 Emergency - provide first aid")
				actions:IssueOrder("Monterey Medical 21 Emergency - provide first aid")
				plugin:Later( function() fsm:ChangeState( "Monterey Responding" ) end )
				--Message ({message = "Monterey Responding"})
				Trace ("Monterey Responding")
			end
 		),
		Wait ( "Monterey Responding","Salinas Response", 20),

		AtState( "Salinas Response",
			function()
				Display( { message = "Mobilizing units from Salinas"})
				actions:IssueOrder("Salinas Police 49 Emergency - block road")
				actions:IssueOrder("Salinas Police 50 move")
				actions:IssueOrder("Salinas Police 48 move")
				actions:IssueOrder("Salinas Fire 46 Emergency - evacuate area")
				actions:IssueOrder("Salinas Fire 42 Emergency - evacuate area")
				actions:IssueOrder("Salinas Fire 45 Emergency - evacuate area")
				actions:IssueOrder("Salinas Fire 44 Emergency - evacuate area")
				actions:IssueOrder("Salinas Fire 43 Emergency - evacuate area")
				actions:IssueOrder("Salinas Medical 37 Emergency - provide first aid")
				actions:IssueOrder("Salinas Medical 38 Emergency - provide first aid")
				actions:IssueOrder("Salinas Medical 39 Emergency - provide first aid")
				plugin:Later( function() fsm:ChangeState( "Salinas Responding" ) end )
				--Message ({message = "Monterey Responding"})
				Trace ("Salinas Responding")
			end
 		),
		Wait ( "Salinas Responding","Time to Do NBC Prompt", 20),

		AtState( "Time to Do NBC Prompt",
			function()
                Dialog ({id = "NBCAlertPrompt", message = "To Alert EOC of NBC danger, click OK", buttons = {"OK","Cancel"}})
				plugin:Later( function() fsm:ChangeState( "NBC Prompt Done" ) end )
				--Message ({message = "Prompt User to Alert NBC"})
				Trace ("Prompt User to Alert NBC, sim paused")
			end
 		),

		{
			events.client:UserChose(),
			{ "NBC Prompt Done" },
			function( dialog, answer )
				if dialog == "NBCAlertPrompt" then
					if answer == "OK" then
					sim:Resume()
					plugin:Later( function() fsm:ChangeState( "NBC Alert Done" ) end )
					--Message ({message = "NBC Alert Done, sim unpaused"})
					Trace ("NBC Alert Done, sim unpaused")
					end
				end
			end
		},
		AtState( "NBC Alert Done",
			function()
				Display( { message = "Issuing Orders for NBC Suits"})
				actions:IssueOrder("NBC suit on")
				plugin:Later( function() fsm:ChangeState( "NBC Suits Ordered" ) end )
				--Message ({message = "NBC Suits Ordered"})
				Trace ("NBC Suits Ordered")
			end
 		),
		Wait ( "NBC Suits Ordered","Alert Downwind Population", 20),

		AtState( "Alert Downwind Population",
			function()
				Display( { message = "EOC Issuing Press Release for Downwind Population to Shelter Emplace"})
				plugin:Later( function() fsm:ChangeState( "NBC Emergency Alert" ) end )
				--Message ({message = "NBC Emergency Alert"})
				Trace ("NBC Emergency Alert")
			end
 		),
		Wait ( "NBC Emergency Alert","Dispatch NBC Units", 10),

		AtState( "Dispatch NBC Units",
			function()
				Display( { message = "Dispatching NBC units from Monterey and Salinas"})
				actions:IssueOrder("Monterey 33 NBC - operate a decontamination site")
				actions:IssueOrder("Salinas 51 NBC - operate a decontamination site")
				plugin:Later( function() fsm:ChangeState( "NBC Units Dispatched" ) end )
				--Message ({message = "NBC Units Dispatched"})
				Trace ("NBC Units Dispatched")
			end
 		),
		{
            events.agents:AgentEnters(DeconSite),
            { "NBC Units Dispatched" },
            function( entity)
				if entity:GetTeam() == "Regional  Emergency Services" then
					Display( { message = "NBC Units arrived at decontamination Site"})
					plugin:Later( function() fsm:ChangeState( "Manned Decontamination Site" ) end )
					--Message ({message = "Manned Decontamination Site"})
					Trace ("Manned Decontamination Site")
				end
            end
        },

		Wait ( "Manned Decontamination Site","Population Move to Decontamination Site", 10),

		AtState( "Population Move to Decontamination Site",
			function()
				Display( { message = "Population moving to decontamination site"})
				actions:IssueOrder("Population - move to decon site")
				--Message ({message = "Population moving to decontamination site"})
				Trace ("Population moving to decontamination site")
				plugin:Later( function() fsm:ChangeState( "Population to decon site" ) end )
			end
		),

		Wait ( "Population to decon site","Cue Medevac Helicopter Flyover", 10),

		AtState( "Cue Medevac Helicopter Flyover",
			function()
				Display( { message = "Medevac Helicopter Flying Over"})
				actions:IssueOrder("medevac 22 move")
				--Message ({message = "Medevac Helicopter Flying Over"})
				Trace ("Medevac Helicopter Flying Over")
				plugin:Later( function() fsm:ChangeState( "Medevac Flyover" ) end )
			end
		),

		Wait ( "Medevac Flyover","Initial Ambulances Leave", 10),

		AtState( "Initial Ambulances Leave",
			function()
				Display( { message = "Ambulances depart for Salinas Valley Memorial Hospital"})
				actions:IssueOrder("ambulance 19 move")
				actions:IssueOrder("ambulance 37 move")
				actions:IssueOrder("ambulance 20 move")
				actions:IssueOrder("ambulance 38 move")
				--Message ({message = "Initial Ambulances depart for Salinas Valley Memorial Hospital"})
				Trace ("mbulances depart for Salinas Valley Memorial Hospital")
				plugin:Later( function() fsm:ChangeState( "Ambulance Depart" ) end )
			end
		),



	}
	DeclareEvents( eventTable )
end
