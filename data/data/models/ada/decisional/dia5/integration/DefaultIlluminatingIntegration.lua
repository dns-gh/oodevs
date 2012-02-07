-------------------------------------------------------------------------------
-- Reconnoitring Implementation : 
-- @author GGE
-- @created 2010-06-22
--
-- This file is part of a MASA library or program.
-- Refer to the included end-user license agreement for restrictions.
--
-- Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
-------------------------------------------------------------------------------

integration.isInIlluminateRange = function( target )
    return integration.distance( meKnowledge, target ) < DEC_Agent_IlluminateRange() 
end

