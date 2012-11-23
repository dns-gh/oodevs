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

integration.firingRangeToSupport = function( target )
    local tirIndirect = { DEC_Tir_PorteeMaxTirIndirectSansChoisirMunition() / 3, DEC_Tir_PorteeMaxTirIndirectSansChoisirMunition() / 2 }
    local tirDirect = { DEC_Tir_PorteeMaxPourTirer( 0.9 ) / 2, DEC_Tir_PorteeMaxPourTirer( 0.7 ) / 2 }
    if tirIndirect[2] > tirDirect[2] then
        return tirIndirect
    else
        return tirDirect
    end
end

integration.getPositionToSupportFriend = function( friendToSupport )
    local rangeDistance = integration.getMaxRangeIndirectFireWithoutSelectAmmo() / 2  -- indirect fire case
    if rangeDistance <= 0 then -- direct fire case
        rangeDistance = DEC_Tir_PorteeMaxPourTirer( 0.7 ) / 2
    end
    if integration.hasMission( meKnowledge.source ) then
        local mission = DEC_GetRawMission( meKnowledge.source )
        local dir = integration.getDangerousDirection( mission )
        local friendPos = friendToSupport:getPosition()
        local positionToSupport = DEC_Geometrie_PositionTranslateDir( friendPos, dir, - rangeDistance )
        return CreateKnowledge( integration.ontology.types.point, positionToSupport )
    end
    return nil
end