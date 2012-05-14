--Default Askable Implementation

integration.startInterrogatePion = function( pion )
    pion[myself] = pion[myself] or {}
    pion[myself].actionInterrogate = DEC_StartInterrogateUnit( pion.source )
    actionCallbacks[ pion[myself].actionInterrogate ] = function( arg ) pion[myself].actionInterrogateState = arg end
    return false
end

integration.updateInterrogatePion = function( pion )
    pion[myself] = pion[myself] or {}
    if pion[myself].actionInterrogateState == eActionObjetTerminee or pion[myself].actionInterrogateState == eActionObjetImpossible then
        return true
    end
    return false
end

integration.stopInterrogatePion = function( pion )
    pion[myself] = pion[myself] or {}
    if pion[myself].actionInterrogateState == eActionObjetTerminee then
        local criticalIntelligence = DEC_ObtenirRenseignementCritiqueSurPion( pion.source )
        if string.len( criticalIntelligence ) ~= 0 then
            meKnowledge:RC( eRC_criticalIntelligence, criticalIntelligence )
            meKnowledge:sendCriticalIntelligence( integration.GetSuperiorKnowledge( meKnowledge ), {criticalIntelligence = criticalIntelligence, RC = eRC_criticalIntelligence} )
        else
            meKnowledge:RC( eRC_criticalIntelligenceNone )
            meKnowledge:sendCriticalIntelligence( integration.GetSuperiorKnowledge( meKnowledge ), {RC = eRC_criticalIntelligenceNone} )
        end
    end
    if pion[myself].actionInterrogateState == eActionObjetImpossible then
        meKnowledge:RC( eRC_criticalIntelligenceImpossible )
    end
    pion[myself].actionInterrogate = DEC__StopAction( pion[myself].actionInterrogate )
    pion[myself].actionInterrogateState = nil
    return true
end

integration.startInterrogateCrowd = function( crowd )
    crowd[myself] = crowd[myself] or {}
    crowd[myself].actionInterrogate = DEC_StartInterrogateCrowd( crowd.source )
    actionCallbacks[ crowd[myself].actionInterrogate ] = function( arg ) crowd[myself].actionInterrogateState = arg end
    return false
end

integration.updateInterrogateCrowd = function( crowd )
    crowd[myself] = crowd[myself] or {}
    if crowd[myself].actionInterrogateState == eActionObjetTerminee then
        return true
    end
    return false
end

integration.stopInterrogateCrowd = function( crowd )
    crowd[myself] = crowd[myself] or {}
    if crowd[myself].actionInterrogateState == eActionObjetTerminee then
        local criticalIntelligence = DEC_ObtenirRenseignementCritiqueSurFoule( crowd.source )
        if string.len( criticalIntelligence ) ~= 0 then
            meKnowledge:RC( eRC_criticalIntelligence, criticalIntelligence )
            meKnowledge:sendCriticalIntelligence( integration.GetSuperiorKnowledge( meKnowledge ), {criticalIntelligence = criticalIntelligence, RC =eRC_criticalIntelligence} )
        else
            meKnowledge:RC( eRC_criticalIntelligenceNone )
            meKnowledge:sendCriticalIntelligence( integration.GetSuperiorKnowledge( meKnowledge ), {RC = eRC_criticalIntelligenceNone } )
        end
    end
    crowd[myself].actionInterrogate = DEC__StopAction( crowd[myself].actionInterrogate )
    crowd[myself].actionInterrogateState = nil
    return true
end