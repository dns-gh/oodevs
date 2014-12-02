-------------------------------------------------------------------------------
-- Destroying Implementation :
-- @author MGD
-- @created 2010-04-08
--
-- This file is part of a MASA library or program.
-- Refer to the included end-user license agreement for restrictions.
--
-- Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
-------------------------------------------------------------------------------

--- Returns if the given target is located inside the maximum range of the agent weapon systems allowing to hit it with a certain probability.
-- This method can only be called by an agent.
-- @param target a Directia agent knowledge.
-- @param ph the probability to hit.
-- @return Boolean, whether or not the target is on firing range
integration.isInFiringRange = function( target, ph )
    if not ph then -- Scipio compatibility
        ph = integration.getDefaultPH( target )
    end
    return integration.distance( meKnowledge, target ) < integration.getMaxRangeToFireOnAgent( target.source, ph )
end