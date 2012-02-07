--Default Bypassable Implementation

eTristate_True = 0
eTristate_False = 1
eTristate_DontKnow = 2

integration.canBypassIt = function( object )
    return DEC_Agent_PeutConstruireContournementObjet( object.source )
end

integration.canBeBypassed = function( object )
    return DEC_ConnaissanceObjet_PeutEtreContourne( object.source )
end

integration.bypassLevel = function( object )
    return DEC_ConnaissanceObjet_EstContourne( object.source ) == eTristate_True and 100 or 0
end

integration.startBypassIt = function( object )
    object[myself] = object[myself] or {}
    object[myself].actionBypass = DEC_StartCreerContournementObjet( object.source )
    actionCallbacks[ object[myself].actionBypass ] = function( arg ) object[myself].actionBypassState = arg end
    
    meKnowledge:RC( eRC_DebutTravaux )
end

integration.updateBypassIt = function( object )
    if object[myself].actionBypassState == eActionObjetImpossible then
        DEC_Trace( "impossible works" ) 
    elseif object[myself].actionBypassState == eActionObjetManqueDotation then
        DEC_Trace( "not enough dotation" ) 
    elseif object[myself].actionBypassState == eActionObjetPasDeCapacite then
        DEC_Trace( "no capacity" ) 
    end
end

integration.stopBypassIt = function( object )
    object[myself] = object[myself] or {}
    if object[myself].actionBypassState == eActionObjetTerminee then
        meKnowledge:RC( eRC_FinTravaux )
    else
        DEC_Trace( "pause work bypass" )
    end
    
    object[myself].actionBypass = DEC__StopAction( object[myself].actionBypass )
end
