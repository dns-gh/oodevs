-------------------------------------------------------------------------------
-- Animatable Implementation : 
-- Regroup function to manage artillery field
-- @author MGD
-- @created 2010-02-18
-- @modified MGD 2010-04-19
--
-- This file is part of a MASA library or program.
-- Refer to the included end-user license agreement for restrictions.
--
-- Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
-------------------------------------------------------------------------------

--- Return if the unit has animation capacity
-- @param knowledge on an object
-- @author MGD
-- @release 2010-02-15
integration.canAnimateIt = function( object )
  return DEC_ConnaissanceObjet_PeutEtreAnime( object.source )
end

-- Returns the current percentage of object animation.
-- Regarding the simulation, a 'animated object' is an object that is fully active.
-- This happens when objects are not 'workable' (see capacity definition in authoring tool)
-- or when 'workable' objects are fully animated  (number of current animators is equal 
-- to number max of animators).
-- In this method, the "animation level" is seen as the number of units that work onto the object.
-- So, if no unit is currently working onto the provided object, the expected value is '0'. 
-- Note that, in such a case, the 'DEC_ConnaissanceObjet_NiveauAnimation' function returns '1' because
-- the object is considered as fully animated by the simulation (it doesn't need to be animated to be active).
integration.animationLevel = function( object )
    if DEC_ConnaissanceObjet_PeutEtreAnime( object.source ) then -- animation level depends on the number of animating units.
        return DEC_ConnaissanceObjet_NiveauAnimation( object.source ) * 100 -- /!\ return 1 if object cannot be animated.
    else -- the object is active but no agent is working onto it.
        return 0
    end
end

integration.startAnimateIt = function( object )
    object.actionAnimation = DEC__StartAnimerObjet( object.source )
    meKnowledge:RC( eRC_DebutAnimationObjet, object.source )
    return true
end
integration.stopAnimateIt = function( object )
    object.actionAnimation = DEC__StopAction( object.actionAnimation )
    if integration.animationLevel( object ) == 0 then
        meKnowledge:RC( eRC_FinAnimationObjet, object.source )
    end
    return false
end

-- Chanel crowd 

integration.channelIt = function ( area )
    DEC_Agent_CanaliserPopulation( area.source )
end
