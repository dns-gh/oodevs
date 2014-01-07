-------------------------------------------------------------------
---- ANIMATABLE INTERFACE IMPLEMENTATION
-------------------------------------------------------------------

-- ----------------------------------------------------------------
-- Animate Object
-- ----------------------------------------------------------------
--- Returns true if unit has the capacity to animate the provided object, false otherwise
-- @param object an object knowledge
-- @return Boolean, whether or not this entity can animate the object knowledge
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
-- @param object an object knowledge
-- @return Boolean, whether or not this entity can animate the object knowledge
integration.animationLevel = function( object )
    if DEC_ConnaissanceObjet_PeutEtreAnime( object.source ) then -- animation level depends on the number of animating units.
        return DEC_ConnaissanceObjet_NiveauAnimation( object.source ) * 100 -- /!\ return 1 if object cannot be animated.
    else -- the object is active but no agent is working onto it.
        return 0
    end
end

--- Start animating an object. A report is sent
-- @param object Object knowledge
-- @return true
integration.startAnimateIt = function( object )
    object.actionAnimation = DEC__StartAnimerObjet( object.source )
    meKnowledge:RC( eRC_DebutAnimationObjet, object.source )
    return true
end

--- Stop animating an object. A report is sent
-- @param object Object knowledge
-- @return true
integration.stopAnimateIt = function( object )
    object.actionAnimation = DEC__StopAction( object.actionAnimation )
    if integration.animationLevel( object ) == 0 then
        meKnowledge:RC( eRC_FinAnimationObjet, object.source )
    end
    return false
end

-- ----------------------------------------------------------------
-- Channel Crowd
-- ----------------------------------------------------------------
--- Allows the unit to channel crowd in an area
-- @param area Area knowledge
-- @return true
integration.channelIt = function ( area )
    DEC_Agent_CanaliserPopulation( area.source )
end
