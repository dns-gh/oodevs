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

integration.getPositionToSupportFriend = function( friendToSupport )
    local position
    local rangeDistance = DEC_Tir_PorteeMaxTirIndirectSansChoisirMunition() / 2  -- indirect fire case
    if rangeDistance <= 0 then -- direct fire case
        rangeDistance = DEC_Tir_PorteeMaxPourTirer( 0.7 ) / 2
    end
    if DEC_HasMission( meKnowledge.source ) then
        local mission = DEC_GetRawMission( meKnowledge.source )
        local dir = DEC_GetDirectionDanger( mission )
        local friendPos = friendToSupport.source:DEC_Agent_Position()
        local positionToSupport = DEC_Geometrie_PositionTranslateDir( friendPos, dir, - rangeDistance )
        return CreateKnowledge( sword.military.world.Point, positionToSupport )
    end
    return nil
end