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
    integration.pionRC( eRC_EnContournementObstacle )
    object[myself] = object[myself] or {}
    object[myself].actionBypass = DEC_StartCreerContournementObjet( object.source )
    actionCallbacks[ object[myself].actionBypass ] = function( arg ) object[myself].actionBypassState = arg end
    
    integration.pionRC( eRC_DebutTravaux )
end

integration.updateBypassIt = function( object )
    if object[myself].actionBypassState == eActionObjetImpossible then
        return false 
    elseif object[myself].actionBypassState == eActionObjetManqueDotation then
        return false 
    elseif object[myself].actionBypassState == eActionObjetPasDeCapacite then
        return false 
    end
    return true
end

integration.stopBypassIt = function( object )
    object[myself] = object[myself] or {}
    if object[myself].actionBypassState == eActionObjetTerminee then
        integration.pionRC( eRC_FinTravaux )
        object[myself].actionBypass = DEC__StopAction( object[myself].actionBypass )
        object[myself].actionBypassState = nil
        return true
    else
        object[myself].actionBypass = DEC__StopAction( object[myself].actionBypass )
        object[myself].actionBypassState = nil
        DEC_Trace( "pause work bypass" )
        return false
    end
    object[myself].actionBypass = DEC__StopAction( object[myself].actionBypass )
    object[myself].actionBypassState = nil
    return true
end

integration.canBuildBypassingObject = function( entity, objectType )
    return DEC_Agent_AgentPeutConstruireContournementObjet( entity, objectType )
end
