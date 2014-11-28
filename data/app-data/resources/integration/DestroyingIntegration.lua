-------------------------------------------------------------------------------
-- Destroying Implementation :
-- @author MGD
-- @created 2010-04-08
--
-- This file is part of a MASA library or program.
-- Refer to the included end-user license agreement for restrictions.
--
-- Copyright (c) 2010 Math�matiques Appliqu�es SA (MASA)
-------------------------------------------------------------------------------

integration.isInFiringRange = function( target, ph )
    if not ph then -- Scipio compatibility
        ph = integration.getDefaultPH( target )
    end
    return integration.distance( meKnowledge, target ) < integration.getMaxRangeToFireOnAgent( target.source, ph )
end