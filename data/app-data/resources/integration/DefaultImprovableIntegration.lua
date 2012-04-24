--Default Improvable Implementation

--- Return if the unit has the capacity to improve the selected object
-- @param knowledge on an object
-- @author MGD
-- @release 2010-02-22
integration.canImproveIt = function( object )
    object[myself] = object[myself] or {} 
    if object[myself] then
        return object[myself].actionImprovementState ~= eActionObjetPasDeCapacite
    else
        return DEC_Agent_PeutValoriserObjet( object.source )
    end
end

integration.hasEnoughtDotationForImprovement = function( object )
    object[myself] = object[myself] or {} 
    if object[myself] then
        return object[myself].actionImprovementState ~= eActionObjetManqueDotation
    else
        return false
    end
end

integration.canBeImproved = function( object )
    return DEC_ConnaissanceObjet_PeutEtreValorise( object.source )
end

integration.improvementLevel = function( object )
    return DEC_ConnaissanceObjet_NiveauValorisation( object.source ) * 100
end

integration.startImproveIt = function( object )
    object[myself] = object[myself] or {} 
    object[myself].actionImprovement = DEC_StartValoriserObjet( object.source )
    actionCallbacks[ object[myself].actionImprovement ] = function( arg ) 
        object[myself].actionImprovementState = arg
    end
    meKnowledge:RC( eRC_DebutValorisation, object.source )
end

integration.updateImproveIt = function( object )
    if object[myself].actionImprovementState == eActionObjetImpossible then
        DEC_Trace( "impossible works" ) 
    elseif object[myself].actionImprovementState == eActionObjetManqueDotation then
        DEC_Trace( "not enough dotation" ) 
    elseif object[myself].actionImprovementState == eActionObjetPasDeCapacite then
        DEC_Trace( "no capacity" ) 
    end
end

integration.stopImproveIt = function( object )
    object[myself] = object[myself] or {} 
    if object[myself].actionImprovementState == eActionObjetTerminee then
        meKnowledge:RC( eRC_FinValorisation, object.source )
    else
        DEC_Trace( "pause work improve" )
    end
    object[myself].actionImprovement = DEC__StopAction( object[myself].actionImprovement )
end

