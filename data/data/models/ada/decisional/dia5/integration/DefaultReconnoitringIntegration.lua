-------------------------------------------------------------------------------
-- Reconnoitring Implementation :
-- @author GGE & PSN
-- @created 2010-04-20
--
-- This file is part of a MASA library or program.
-- Refer to the included end-user license agreement for restrictions.
--
-- Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
-------------------------------------------------------------------------------

integration.isInRecoRange = function( target )
    return integration.distance( meKnowledge, target ) < DEC_Reconnoissance_MajorComponentMinDistance()
end