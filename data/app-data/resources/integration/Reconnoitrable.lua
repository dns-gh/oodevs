-- MASA Life Brain Reconnoitrable knowledge interface
-- <br/>This file registers the modelling level implementations of the Reconnoitrable interface methods.

--------------------------------------------------------
-- @name generic_reconnaissancePriority
-- @class function
-- @description Return true if the knowledge is considered as nearby for the agent.
-- @param self The knowledge to reach.
-- @return Return true if the knowledge is near, false otherwise.
--
integration.generic_reconnaissancePriority = function( self )
    return math.max( self:proximityLevel(), 1 ) / 100
end

--------------------------------------------------------
-- @name me_canReconnoitreIt
-- @class function
-- @description Return true if the conditions to reconnoitre the knowledge are completed.
-- @param self The knowledge to reconnoitre.
-- @return True if the reconnaissance of the knowledge is possible, false otherwise.
--
integration.me_canReconnoitreIt = function( self )
    return masalife.brain.knowledge.me.body:computeReconnaissanceCapability( self, masalife.brain.knowledge.me.body ) > 0
end
