--- Returns the priority to reconnoiter the given knowledge.
-- @param target Knowledge defining a "proximityLevel" method returning a float
-- @return Float between 0 and 1, 0 meaning the lowest priority and 1 meaning the highest priority
integration.generic_reconnaissancePriority = function( target )
    return math.max( target:proximityLevel(), 1 ) / 100
end

--- Deprecated
integration.me_canReconnoitreIt = function( target )
    return meKnowledge.computeReconnaissanceCapability 
           and meKnowledge:computeReconnaissanceCapability( target, meKnowledge ) > 0 or true
end
