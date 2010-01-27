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

integration.observeIt = function( point )
  --TODO
end