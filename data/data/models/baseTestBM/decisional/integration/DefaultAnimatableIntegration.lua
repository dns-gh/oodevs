--Default Animatable Implementation

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
end,
integration.startAnimateIt = function( object )
    object.actionAnimation = DEC_StartAnimerObjet( object.source )
    return true
end,
integration.stopAnimateIt = function( object )
    object.actionAnimation = DEC_StopAction( object.actionAnimation )
end,

