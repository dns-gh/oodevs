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

integration.firingRangeToSupport = function()
	local tirIndirect = { DEC_Tir_PorteeMaxTirIndirectSansChoisirMunition() / 3, DEC_Tir_PorteeMaxTirIndirectSansChoisirMunition() / 2 }
	local tirDirect = { DEC_Tir_PorteeMaxPourTirer( 0.9 ) / 2, DEC_Tir_PorteeMaxPourTirer( 0.7 ) / 2 }
	if tirIndirect[2] > tirDirect[2] then
		return tirIndirect
	else
	    return tirDirect
	end
end

integration.getPositionToSupportFriend = function( friendToSupport )
    local rangeDistance = DEC_Tir_PorteeMaxTirIndirectSansChoisirMunition() / 2  -- indirect fire case
    if rangeDistance <= 0 then -- direct fire case
        rangeDistance = DEC_Tir_PorteeMaxPourTirer( 0.7 ) / 2
    end
    if DEC_HasMission( meKnowledge.source ) then
        local mission = DEC_GetRawMission( meKnowledge.source )
        local dir = DEC_GetDirectionDanger( mission )
        local friendPos = friendToSupport:getPosition()
        local positionToSupport = DEC_Geometrie_PositionTranslateDir( friendPos, dir, - rangeDistance )
        return CreateKnowledge( sword.military.world.Point, positionToSupport )
    end
    return nil
end

integration.getSupportDistanceToCoordination = function( agent, pH )
    local rangeDistance = DEC_Tir_PorteeMaxTirIndirectSansChoisirMunition( agent )  -- indirect fire case
    if rangeDistance <= 0 then -- direct fire case
        rangeDistance = DEC_Tir_PorteeMaxPourTirer( agent, pH )
    end
    return rangeDistance
end

-- return the point projection into the entity danger direction vector
-- @author LMT 30/08/2013
integration.getPositionAlongDangerDirection = function( entity, point )
    return DEC_Geometrie_PositionAdvanceAlongDangerDirection( entity.source, point )
end