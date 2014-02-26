--Default Removable Implementation

--- Return if the unit is mined
-- @param knowledge on an object
-- @author DDA
-- @release 2012-03-06
integration.isMined = function( object )
    return DEC_ConnaissanceObjet_EstValorise( object.source ) == eTristate_True and true or false
end

--- Return if the unit is 100% mined
-- @param knowledge on an object
-- @author NMI
-- @release 2013-06-19
integration.isFullyMined = function( object )
    return DEC_ObjectKnowledge_IsFullMined( object.source ) == eTristate_True and true or false
end

--- Return if the unit has the capacity to demine the selected object
-- @param knowledge on an object
-- @author DDA
-- @release 2012-03-06
integration.canDemineIt = function( object )
    return DEC_Agent_PeutDevaloriserObjet( object.source )
end

integration.startDemineIt = function( object )
    object[myself] = object[myself] or {} 
    object[myself].actionDemine = DEC_StartDevaloriserObjet( object.source )
    actionCallbacks[ object[myself].actionDemine ] = function( arg ) object[myself].actionDemineState = arg end
    
    reportFunction(eRC_DebutDevalorisation, object.source )
end

integration.updateDemineIt = function( object )
    if object[myself].actionDemineState == eActionObjetImpossible then
        DEC_Trace( "impossible works" )
        return false 
    elseif object[myself].actionDemineState == eActionObjetManqueDotation then
        DEC_Trace( "not enough dotation" ) 
        return false
    elseif object[myself].actionDemineState == eActionObjetPasDeCapacite then
        DEC_Trace( "no capacity" )
        return false 
    end
    return true
end

integration.stopDemineIt = function( object )
    object[myself] = object[myself] or {} 
    object[myself].actionDemine = DEC__StopAction( object[myself].actionDemine )
    if object[myself].actionDemineState == eActionObjetTerminee then
        reportFunction(eRC_FinDevalorisation, object.source )
        object[myself].actionDemineState = nil
        return true
    else
        object[myself].actionDemineState = nil
        DEC_Trace( "pause work demine" )
        return false
    end
end
--- Return if the unit has the capacity to remove the selected object
-- @param knowledge on an object
-- @author PSN
-- @release 2010-03-30
integration.canRemoveIt = function( object )
    return masalife.brain.core.class.isOfType( object, integration.ontology.types.object ) 
           and DEC_Agent_PeutDetruireObjet( object.source ) or true  -- return false if object is not valid.
end

-- $$$ MIA merge with military: return true if agent can remove the objet depending on its physical capabilities
integration.canRemoveItSecu = function( object )
    object.canRemoveit = masalife.brain.core.class.isOfType( object, integration.ontology.types.object ) 
                         and DEC_Agent_PeutDetruireObjet( object.source ) or true
    return object.canRemoveit
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
    reportFunction(eRC_DebutTravauxObjet, object.source )
end

integration.updateRemoveIt = function( object )
    if object[myself].actionRemoveState == eActionObjetImpossible then
        DEC_Trace( "impossible works" )
        return false 
    elseif object[myself].actionRemoveState == eActionObjetManqueDotation then
        DEC_Trace( "not enough dotation" )
        return false 
    elseif object[myself].actionRemoveState == eActionObjetPasDeCapacite then
        DEC_Trace( "no capacity" )
        return false 
    end
    return true
end

integration.stopRemoveIt = function( object )
    object[myself] = object[myself] or {}
    object[myself].actionRemove = DEC__StopAction( object[myself].actionRemove )
    if object[myself].actionRemoveState == eActionObjetTerminee then
        reportFunction(eRC_FinDegagement )
        object[myself].actionRemoveState = nil
        return true
    else
        object[myself].actionRemoveState = nil
        DEC_Trace( "pause work remove" )
        return false
    end
end

integration.isInAvoidanceArea = function( object )
    return DEC_ConnaissanceObjet_PointEstDansZoneEvitement( meKnowledge:getPosition(), object.source )
end

integration.isInEffectArea = function( object, distance )
    return DEC_ConnaissanceObjet_PointEstProcheZoneEffet( meKnowledge:getPosition(), object.source, distance or 100 )
end
--- Return if the unit is avoidable
-- @param knowledge on an object
-- @author DDA
-- @release 2012-04-27
integration.isAvoidable = function( object )
    return DEC_ObjectKnowledge_HasCapacity( object.source, "avoidable" )
end
-- ============================================================================
-- Object deconstruction SECU
-- comments: -- $$$ MIA TEMP SECURITY à merger avec military
-- Différence avec military: le contrat n'est pasle même. Ici on renvoie vrai 
-- quand l'action est terminée, que l'on est contruit ou pas l'objet. On renvoie faux
-- quand la construction est en cours. 
-- ============================================================================
integration.updateRemoveItSecu = function( object )
    if object[myself].actionRemoveState == eActionObjetImpossible then
        reportFunction( eRC_ConstructionObjetImpossible )
        DEC_Trace( "impossible works" )
        return true 
    elseif object[ myself ].actionRemoveState == eActionObjetManqueDotation then
        DEC_Trace( "not enough dotation" )
         reportFunction( eRC_PasDotationConstructionObjet )
        return true 
    elseif object[ myself ].actionRemoveState == eActionObjetPasDeCapacite then
        DEC_Trace( "no capacity" )
        reportFunction( eRC_PasDotationConstructionObjet )
        return true 
    elseif object[ myself ].actionRemoveState == eActionObjetTerminee then
        reportFunction(eRC_FinDegagement )
        return true
    end
    return false
end

integration.stopRemoveItSecu = function( object )
    if not object[ myself ].actionRemoveState == eActionObjetTerminee then
        reportFunction(eRC_FinDegagement )
    end
    object[ myself ] = object[ myself ] or {} 
    object[ myself ].actionRemove = DEC__StopAction( object[myself].actionRemove )
    return true
end

integration.startDecontructItUrbanBlock = function( urbanBlock )
    urbanBlock[ myself ] = urbanBlock[ myself ] or {} 
    urbanBlock[ myself ].actionRemove = DEC_DeteriorateUrbanBlock( urbanBlock.source, 0 )
    actionCallbacks[ urbanBlock[ myself ].actionRemove ] = function( arg ) 
        urbanBlock[ myself ].actionRemoveState = arg 
    end
    reportFunction(eRC_DebutTravaux )
end