--Default Removable Implementation
integration.isMined = function( object )
    return DEC_ConnaissanceObjet_EstValorise( object.source ) == eTristate_True and true or false
end


--- Return if the unit has the capacity to remove the selected object
-- @param knowledge on an object
-- @author PSN
-- @release 2010-03-30
integration.canRemoveIt = function( object )
  if integration.isMined( object ) then
     return DEC_Agent_PeutDevaloriserObjet( object.source )
  else
    return DEC_Agent_PeutDetruireObjet( object.source )
  end
end

--- Return current advancement of object removal
-- @param knowledge on an object
-- @author PSN
-- @release 2010-03-30
integration.removalLevel = function( object )
    return ( 100 - DEC_ConnaissanceObjet_NiveauConstruction( object.source ) * 100 )
end

integration.startRemoveIt = function( object )
    object[myself] = object[myself] or {} 
    object[myself].actionRemove = DEC_StartDetruireObjet( object.source )
    actionCallbacks[ object[myself].actionRemove ] = function( arg ) object[myself].actionRemoveState = arg end
    
    meKnowledge:RC( eRC_DebutDegagement, object.source )
end

integration.updateRemoveIt = function( object )
    if object[myself].actionRemoveState == eActionObjetImpossible then
        DEC_Trace( "impossible works" ) 
    elseif object[myself].actionRemoveState == eActionObjetManqueDotation then
        DEC_Trace( "not enough dotation" ) 
    elseif object[myself].actionRemoveState == eActionObjetPasDeCapacite then
        DEC_Trace( "no capacity" ) 
    end
end

integration.stopRemoveIt = function( object )
    object[myself] = object[myself] or {} 
    if object[myself].actionRemoveState == eActionObjetTerminee then
        meKnowledge:RC( eRC_FinDegagement )
    else
        DEC_Trace( "pause work remove" )
    end
    
    object[myself].actionRemove = DEC__StopAction( object[myself].actionRemove )
end

integration.isInAvoidanceArea = function( object )
    return DEC_ConnaissanceObjet_PointEstDansZoneEvitement( meKnowledge:getPosition(), object.source )
end

integration.isInEffectArea = function( object )
    return DEC_ConnaissanceObjet_PointEstProcheZoneEffet( meKnowledge:getPosition(), object.source, 100 )
end
