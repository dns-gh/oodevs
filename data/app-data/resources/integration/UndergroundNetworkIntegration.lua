integration.EnterInUndergroundNetwork = function( network )
    local returnError = DEC_Agent_SeDissimulerDansReseauSouterrain( myself, network.source)
    if returnError == eUndergroundNetworkInvalid then
        if not myself.alreadySend then
            meKnowledge:RC( eRC_NotActivatedUndergroundNetwork )
            myself.alreadySend = true
        end
        return false
    end
    return true
end
integration.ExitFromUndergroundNetwork = function()
    local returnError = DEC_Agent_SortirDuReseauSouterrain( myself ) -- /!\ after first tick, if agent has exit, it returns "eUndergroundNetworkInvalid" 
    if returnError == eUndergroundNetworkInvalid then
        if not myself.alreadySend then
            meKnowledge:RC( eRC_NotActivatedUndergroundNetwork )
            myself.alreadySend = true
        end
        return false
    end
    return true
end
integration.EnableUndergroundNetwork = function( self )
    DEC_RC( eRC_UndergroundNetworkExitActivated )
    DEC_ConnaissanceObjet_ActiverIssueDeReseauSouterrain( self.source )
    return true
end
integration.DisableUndergroundNetwork = function( self )
    DEC_RC( eRC_UndergroundNetworkExitDeactivated )
    DEC_ConnaissanceObjet_DesactiverIssueDeReseauSouterrain( self.source )
    return true
end

integration.StartPassThroughUndergroundNetwork = function( exitIssue )
    integration.setStealth( true ) -- when entering into the network, the agent is hidden.
    exitIssue[myself] = exitIssue[myself] or {}
    exitIssue[myself].actionPassThrough = DEC_StartTraverserReseauSouterrain( exitIssue.source )
    actionCallbacks[ exitIssue[myself].actionPassThrough ] = function( arg ) exitIssue[myself].actionPassThroughState = arg end
    exitIssue[myself].firstTime = true
end
integration.UpdatePassThroughUndergroundNetwork = function( exitIssue )
    if exitIssue[myself].actionPassThroughState == eActionNotAllowed then
        if exitIssue[myself].firstTime then
            meKnowledge:RC( eRC_BadUndergroundNetwork )
            exitIssue[myself].firstTime = false
        end
        return false
    elseif exitIssue[myself].actionPassThroughState == eActionFinished then
        return true
    elseif exitIssue[myself].firstTime and exitIssue[myself].actionPassThroughState ~= nil then
        local timeMoving = DEC_Agent_TempsPourTraverserReseauSouterrain( myself, exitIssue.source )
        meKnowledge:RC( eRC_TimeInUndergroundNetwork, tostring(math.ceil(timeMoving/60)) )
        exitIssue[myself].firstTime = false
    end
end
integration.StopPassThroughUndergroundNetwork = function( exitIssue )
    integration.setStealth( false ) -- the agent is no more hidden.
    exitIssue[myself] = exitIssue[myself] or {}
    exitIssue[myself].actionPassThrough = DEC__StopAction( exitIssue[myself].actionPassThrough )
    exitIssue[myself].actionPassThroughState = nil
end

integration.enterAndExitInSameUndergroundNetwork = function( enterIssue, exitIssue )
    return DEC_ConnaissanceObjet_IssuesDuMemeReseauSouterrain( enterIssue.source, exitIssue.source )
end

integration.getResourceNetworkNodesInZone = function( agent, area )
    if agent == nil then
        return DEC_ResourceNetwork_NodesInZone( area )
    else
        return DEC_ResourceNetwork_NodesInZone( agent, area )
    end
end
