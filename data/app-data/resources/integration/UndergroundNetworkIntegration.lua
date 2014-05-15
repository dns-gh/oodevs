--- Makes this entity enter the given underground network, if it can.
-- In order for the agent to be able to enter the given object, the following conditions must be met :
-- <ul> <li> The agent must not be underground </li>
-- <li> The given object must be valid </li>
-- <li> The agent must know the given object </li>
-- <li> The given object must have the "Underground network exit" capacity
-- set in the authoring tool </li>
-- <li> The exit must be enabled </li> </ul>
-- This method can only be called by an agent.
-- This method may display a report.
-- @see integration.ExitFromUndergroundNetwork
-- @param network Object knowledge
-- @return Boolean, whether or not the agent successfully entered the network.
integration.EnterInUndergroundNetwork = function( network )
    local returnError = DEC_Agent_SeDissimulerDansReseauSouterrain( myself, network.source )
    if returnError == eUndergroundNetworkInvalid then
        if not myself.alreadySend then
            reportFunction(eRC_NotActivatedUndergroundNetwork )
            myself.alreadySend = true
        end
        return false
    end
    return true
end

--- Makes this entity exit an underground network, if it can.
-- In order for the agent to be able to exit, the following conditions must be met :
-- <ul> <li> The agent must be underground </li>
-- <li> The agent must know a valid object located at the agent's current location,
-- with the "Underground network exit" capacity set in the authoring tool. </li>
-- <li> The exit must be enabled </li> </ul>
-- This method can only be called by an agent.
-- This method may display a report.
-- @see integration.EnterInUndergroundNetwork
-- @return Boolean, whether or not the agent successfully entered the network.
integration.ExitFromUndergroundNetwork = function()
    local returnError = DEC_Agent_SortirDuReseauSouterrain( myself ) -- /!\ after first tick, if agent has exit, it returns "eUndergroundNetworkInvalid" 
    if returnError == eUndergroundNetworkInvalid then
        if not myself.alreadySend then
            reportFunction(eRC_NotActivatedUndergroundNetwork )
            myself.alreadySend = true
        end
        return false
    end
    return true
end

--- Enables the given underground exit.
-- The enabling only works if the given object has the "Underground network exit"
-- capacity set in the authoring tool.
-- This method displays a report.
-- @see integration.DisableUndergroundNetwork
-- @param undergroundExit Object knowledge
-- @return Boolean, true
integration.EnableUndergroundNetwork = function( undergroundExit )
    DEC_RC( eRC_UndergroundNetworkExitActivated )
    DEC_ConnaissanceObjet_ActiverIssueDeReseauSouterrain( undergroundExit.source )
    return true
end

--- Disables the given underground exit.
-- The disabling only works if the given object has the "Underground network exit"
-- capacity set in the authoring tool.
-- This method displays a report.
-- @see integration.EnableUndergroundNetwork
-- @param undergroundExit Object knowledge
-- @return Boolean, true
integration.DisableUndergroundNetwork = function( undergroundExit )
    DEC_RC( eRC_UndergroundNetworkExitDeactivated )
    DEC_ConnaissanceObjet_DesactiverIssueDeReseauSouterrain( undergroundExit.source )
    return true
end

--- Starts moving through an underground network towards the given exit.
-- The agent can only move through an underground network if the agent is underground,
-- and if the given exit is valid and has the "Underground network exit" capacity
-- set in the authoring tool.
-- An agent moving through an underground network is stealthy.
-- This method can only be called by an agent.
-- @see integration.UpdatePassThroughUndergroundNetwork
-- @see integration.StopPassThroughUndergroundNetwork
-- @param exitIssue Object knowledge
integration.StartPassThroughUndergroundNetwork = function( exitIssue )
    integration.setStealth( true )
    exitIssue[myself] = exitIssue[myself] or {}
    exitIssue[myself].actionPassThrough = DEC_StartTraverserReseauSouterrain( exitIssue.source )
    actionCallbacks[ exitIssue[myself].actionPassThrough ] = function( arg ) exitIssue[myself].actionPassThroughState = arg end
    exitIssue[myself].firstTime = true
end

--- Updates the movement through an underground network towards the given exit.
-- This method can only be called by an agent.
-- May display reports
-- @see integration.StartPassThroughUndergroundNetwork
-- @see integration.StopPassThroughUndergroundNetwork
-- @param exitIssue Object knowledge
-- @return Boolean, whether or not the action is finished
integration.UpdatePassThroughUndergroundNetwork = function( exitIssue )
    if exitIssue[myself].actionPassThroughState == eActionNotAllowed then
        if exitIssue[myself].firstTime then
            reportFunction(eRC_BadUndergroundNetwork )
            exitIssue[myself].firstTime = false
        end
        return false
    elseif exitIssue[myself].actionPassThroughState == eActionFinished then
        return true
    elseif exitIssue[myself].firstTime and exitIssue[myself].actionPassThroughState ~= nil then
        local timeMoving = DEC_Agent_TempsPourTraverserReseauSouterrain( myself, exitIssue.source )
        reportFunction(eRC_TimeInUndergroundNetwork, tostring(math.ceil(timeMoving/60)) )
        exitIssue[myself].firstTime = false
    end
    return false
end

--- Stops the movement through an underground network towards the given exit.
-- The agent will stop being stealthy.
-- This method can only be called by an agent.
-- @see integration.StartPassThroughUndergroundNetwork
-- @see integration.UpdatePassThroughUndergroundNetwork
-- @param exitIssue Object knowledge
integration.StopPassThroughUndergroundNetwork = function( exitIssue )
    integration.setStealth( false )
    exitIssue[myself] = exitIssue[myself] or {}
    exitIssue[myself].actionPassThrough = DEC__StopAction( exitIssue[myself].actionPassThrough )
    exitIssue[myself].actionPassThroughState = nil
end

--- Returns true if the two given objects are exits of the same underground network, false otherwise.
-- @param enterIssue Object knowledge
-- @param exitIssue Object knowledge
-- @return Boolean 
integration.enterAndExitInSameUndergroundNetwork = function( enterIssue, exitIssue )
    return DEC_ConnaissanceObjet_IssuesDuMemeReseauSouterrain( enterIssue.source, exitIssue.source )
end