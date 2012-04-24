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

integration.isInFiringRange = function( target )
    return integration.distance( meKnowledge, target ) < DEC_Tir_PorteeMaxPourTirerSurUnite( target.source, 0.7)
end