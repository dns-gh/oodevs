-------------------------------------------------------------------------------
-- Supporting Implementation :
-- @author DDA
-- @created 2010-05-03
--
-- This file is part of a MASA library or program.
-- Refer to the included end-user license agreement for restrictions.
--
-- Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
-------------------------------------------------------------------------------

integration.isInMiddleFiringRange = function( target )
    return integration.distance( meKnowledge, target ) < DEC_Tir_PorteeMaxPourTirer( 0.7 )/2
end