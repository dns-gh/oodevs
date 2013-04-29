-------------------------------------------------------------------------------
-- generic.knowledge.Positionnable Implementation :
-- Regroup function to manage platoon equipement
-- @author MGD
-- @created 2010-06-03
--
-- This file is part of a MASA library or program.
-- Refer to the included end-user license agreement for restrictions.
--
-- Copyright (c) 2010 Math�matiques Appliqu�es SA (MASA)
-------------------------------------------------------------------------------

--Positionnable Implementation
integration.takeUpPosition = function( reachable )
    myself.location = reachable
    if DEC_Agent_NiveauInstallation() >= eNiveauInstallation_Poste then -- corresponding to E_PostureType::ePosturePoste
        -- meKnowledge:RC( eRC_EnPosteFaceADirectionDangereuse )
        return true
    end
    return false
end

integration.takeUpPositionObject = function( object )
    if( DEC_ConnaissanceObjet_PeutEtreOccupe( object.source ) ) then
        myself.actionOccupy = DEC__StartOccuperObjet( object.source )
        return true
    else
        -- meKnowledge:RC( eRC_ObjetDejaOccupe, object.source ) @TODO MGD : Use takeUpPosition feedback failed after refactor of MoveAndTakePosition with MIA
        DEC_Trace( "eRC_ObjetDejaOccupe" )
        return false
    end
end


--- Indicate if the unit is posted
-- @param the unit
-- @return bool
-- @author MGD
-- @created 2010-06-03
integration.isPosted = function( self )
    if DEC_Agent_NiveauInstallation() >= eNiveauInstallation_Poste then
        return true
    end
    return false
end

--- Indicate if the unit is parked
-- @param the unit
-- @return bool
-- @author LMT
-- @created 2012-02-18
integration.isParked = function( self )
    if DEC_Agent_NiveauInstallation() >= eNiveauInstallation_PosteAmenage then
        return true
    end
    return false
end