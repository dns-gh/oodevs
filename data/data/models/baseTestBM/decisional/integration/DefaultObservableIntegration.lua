--Default Observable Implementation

--- Compute diplomatie between brain's team and agent's team
-- @param knowledge on an agent 
-- @author MGD
-- @release 2010-01-22
integration.computeRelationAgent = function( unit )
  if( DEC_ConnaissanceAgent_EstUnEnnemi( unit.source ) == 0 ) then
    return 0
  elseif( DEC_ConnaissanceAgent_EstUnAllie( unit.source ) == 0 ) then
    return 100
  else
    return 50
  end
end

--- Compute diplomatie between brain's team and  tacticalObject's team
-- @param knowledge on an object
-- @author MGD
-- @release 2010-01-22
integration.computeRelationObject = function( object )
  if( DEC_ConnaissanceObject_EstUnEnnemi( object.source ) == 0 ) then
    return 0
  elseif( DEC_ConnaissanceObject_EstUnAllie( object.source ) == 0 ) then
    return 100
  else
    return 50
  end
end

--- Get unit perception
-- @param knowledge on an unit
-- @author SLG
-- @release 2010-01-27
integration.getAgentPerception = function( unit )
  perceptionLevel = DEC_ConnaissanceAgent_NiveauDePerceptionCourant( unit.source )
  if ( perceptionLevel == 1 ) then
    return 30
  elseif ( perceptionLevel == 2 ) then
    return 60
  elseif ( perceptionLevel == 3 ) then
    return 100
  else
    return 0   
  end
end

--- Get object perception
-- @param knowledge on an object
-- @author SLG
-- @release 2010-01-27
integration.getObjectPerception = function( object )
  perceptionLevel = DEC_ConnaissanceObjet_NiveauDePerceptionCourant( object.source )
  if ( perceptionLevel == 1 ) then
    return 30
  elseif ( perceptionLevel == 2 ) then
    return 60
  elseif ( perceptionLevel == 3 ) then
    return 100
  else
    return 0   
  end
end

integration.observeIt = function( point )
  DEC_Perception_VisionVerrouilleeSurPointXY( point.sim_pos.x, point.sim_pos.y )
end

