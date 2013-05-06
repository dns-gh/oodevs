-------------------------------------------------------------------------------
-- Identifying Implementation :
-- @author GGE & PSN
-- @created 2010-04-23
--
-- This file is part of a MASA library or program.
-- Refer to the included end-user license agreement for restrictions.
--
-- Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
-------------------------------------------------------------------------------

integration.isInIdentificationRange = function( target )
    return integration.distance( meKnowledge, target ) < ( DEC_Reconnoissance_MajorComponentMinDistance() * 0.9 )
end