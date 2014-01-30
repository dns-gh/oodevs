--- 
-- MASA Life Brain Reconnoitrable knowledge interface
-- <br/>This file registers the modelling level implementations of the Reconnoitrable interface methods.
-- 

---
-- @description Return priority to reconnoiter a target knowledge
-- @param target The knowledge to reconnoiter
-- @return Return a priority between 0.01 and 1
-- deprecated
integration.generic_reconnaissancePriority = function( target )
    return math.max( target:proximityLevel(), 1 ) / 100
end

--------------------------------------------------------
-- @description Return true if the conditions to reconnoiter the knowledge are completed.
-- @param target The knowledge to reconnoiter.
-- @return True if the reconnaissance of the knowledge is possible, false otherwise.
-- deprecated
integration.me_canReconnoitreIt = function( target )
    return meKnowledge.computeReconnaissanceCapability 
           and meKnowledge:computeReconnaissanceCapability( target, meKnowledge ) > 0 or true
end
