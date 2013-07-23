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

--- Return current percentage of object animation
-- @param knowledge on an object
-- @author MGD
-- @release 2010-02-15
integration.animationLevel = function( object )
    return DEC_ConnaissanceObjet_NiveauAnimation( object.source ) * 100
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
