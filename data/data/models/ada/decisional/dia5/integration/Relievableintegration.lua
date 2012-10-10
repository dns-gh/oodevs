-------------------------------------------------------------------------------
-- Relievable Implementation : 
-- Regroup function to manage reinfrocement
-- @author GGE
-- @created 2010-09-07
--
-- This file is part of a MASA library or program.
-- Refer to the included end-user license agreement for restrictions.
--
-- Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
-------------------------------------------------------------------------------


---Return if the unit can relieve the selected unit
-- @param knowledge on an object
-- @author GGE
-- @release 2010-09-07
integration.canRelieveIt = function( object )
    return DEC_PeutReleverPion( object.source )
end

---Return the unit relieve the selected unit
-- @param knowledge on an object
-- @author GGE
-- @release 2010-09-07
integration.relieveIt = function( object )
    if DEC_ReleverPion( object.source ) then
        meKnowledge:RC( eRC_Releve, object.source )
        return true
    end
    return false
end
