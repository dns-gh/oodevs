-------------------------------------------------------------------
---- ASKABLE INTERFACE IMPLEMENTATION
-------------------------------------------------------------------

--- Start interrogating the provided agent knowledge
-- After a delay (depending on the affinity between the agent and the target), reveals critical information
-- Critical information are filled during the preparation phase
-- The critical information will be retrieved after a call to stopInterrogatePion:
-- @see updateInterrogatePion
-- @see stropInterrogatePion 
-- @param pion Directia agent knowledge
-- @return false
integration.startInterrogatePion = function( pion )
    pion[myself] = pion[myself] or {}
    pion[myself].actionInterrogate = DEC_StartInterrogateUnit( pion.source )
    actionCallbacks[ pion[myself].actionInterrogate ] = function( arg ) pion[myself].actionInterrogateState = arg end
    return false
end

--- Continue interrogating the provided agent knowledge
-- Returns true when interrogating action is finished or impossible, false otherwise
-- @param pion Directia agent knowledge
-- @return Boolean, whether or not action is over
integration.updateInterrogatePion = function( pion )
    pion[myself] = pion[myself] or {}
    if pion[myself].actionInterrogateState == eActionObjetTerminee or pion[myself].actionInterrogateState == eActionObjetImpossible then
        return true
    end
    return false
end

--- Stop interrogating the provided agent knowledge
-- If exists and action is correctly finished the critical information is revealed.
-- @param pion Directia agent knowledge
-- @return true
integration.stopInterrogatePion = function( pion )
    pion[myself] = pion[myself] or {}
    if pion[myself].actionInterrogateState == eActionObjetTerminee then
        local criticalIntelligence = DEC_ObtenirRenseignementCritiqueSurPion( pion.source )
        if string.len( criticalIntelligence ) ~= 0 then
            reportFunction(eRC_criticalIntelligence, criticalIntelligence )
            meKnowledge:sendCriticalIntelligence( integration.GetSuperiorKnowledge( meKnowledge ), {criticalIntelligence = criticalIntelligence, RC = eRC_criticalIntelligence} )
        else
            reportFunction(eRC_criticalIntelligenceNone )
            meKnowledge:sendCriticalIntelligence( integration.GetSuperiorKnowledge( meKnowledge ), {RC = eRC_criticalIntelligenceNone} )
        end
    end
    if pion[myself].actionInterrogateState == eActionObjetImpossible then
        reportFunction(eRC_criticalIntelligenceImpossible )
    end
    pion[myself].actionInterrogate = DEC__StopAction( pion[myself].actionInterrogate )
    pion[myself].actionInterrogateState = nil
    return true
end

--- Start interrogating the provided crowd knowledge
-- After a delay (depending on the affinity between the agent and the crowd), reveals critical information
-- Critical information are filled during the preparation phase
-- The critical information will be retrieved after a call to stopInterrogateCrowd:
-- @see updateInterrogateCrowd
-- @see stropInterrogateCrowd  
-- @param crowd Crowd knowledge
-- @return false
integration.startInterrogateCrowd = function( crowd )
    crowd[myself] = crowd[myself] or {}
    crowd[myself].actionInterrogate = DEC_StartInterrogateCrowd( crowd.source )
    actionCallbacks[ crowd[myself].actionInterrogate ] = function( arg ) crowd[myself].actionInterrogateState = arg end
    return false
end

--- Continue interrogating the provided crowd knowledge
-- Returns true when interrogating action is finished, false otherwise
-- @param crowd Crowd knowledge
-- @return Boolean, whether or not action is over
integration.updateInterrogateCrowd = function( crowd )
    crowd[myself] = crowd[myself] or {}
    if crowd[myself].actionInterrogateState == eActionObjetTerminee then
        return true
    end
    return false
end

--- Stop interrogating the provided crowd knowledge
-- If exists and action is correctly finished the critical information is revealed.
-- @param crowd Crowd knowledge
-- @return true
integration.stopInterrogateCrowd = function( crowd )
    crowd[myself] = crowd[myself] or {}
    if crowd[myself].actionInterrogateState == eActionObjetTerminee then
        local criticalIntelligence = DEC_ObtenirRenseignementCritiqueSurFoule( crowd.source )
        if string.len( criticalIntelligence ) ~= 0 then
            reportFunction(eRC_criticalIntelligence, criticalIntelligence )
            meKnowledge:sendCriticalIntelligence( integration.GetSuperiorKnowledge( meKnowledge ), {criticalIntelligence = criticalIntelligence, RC =eRC_criticalIntelligence} )
        else
            reportFunction(eRC_criticalIntelligenceNone )
            meKnowledge:sendCriticalIntelligence( integration.GetSuperiorKnowledge( meKnowledge ), {RC = eRC_criticalIntelligenceNone } )
        end
    end
    crowd[myself].actionInterrogate = DEC__StopAction( crowd[myself].actionInterrogate )
    crowd[myself].actionInterrogateState = nil
    return true
end