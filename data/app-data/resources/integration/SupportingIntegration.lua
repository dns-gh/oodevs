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
    local tirIndirect = { DEC_Tir_PorteeMaxTirIndirectSansChoisirMunition() / 3, DEC_Tir_PorteeMaxTirIndirectSansChoisirMunition() / 2, true }
    local tirDirect = { DEC_Tir_PorteeMaxPourTirer( 0.9 ) / 2, DEC_Tir_PorteeMaxPourTirer( 0.7 ) / 2, false }
    if tirIndirect[2] > tirDirect[2] then
        return tirIndirect
    else
        return tirDirect
    end
end

integration.firingRangeWithDirectFires = function( ph )
    return DEC_Tir_PorteeMaxPourTirer( ph )
end

integration.firingRangeWithIndirectFires = function()
    return DEC_Tir_PorteeMaxTirIndirectSansChoisirMunition()
end

integration.computeSupportFriendEfficiency = function( friendToSupport )
    local rangeDistance = integration.firingRangeToSupport( friendToSupport )
    if not rangeDistance[3] then -- direct fire case
        if integration.hasMission( meKnowledge.source ) then
            local mission = DEC_GetRawMission( meKnowledge.source )
            local dir = integration.getDangerousDirection( mission )
            local friendPos = friendToSupport:getPosition()
            local positionToSupport = DEC_Geometrie_PositionTranslateDir( friendPos, dir, - rangeDistance[2] )
            local position = CreateKnowledge( integration.ontology.types.point, positionToSupport )
            return integration.normalizedInversedDistance( position, self ) / 100
        end
        return 0
    else --indirect fire case (stay on firing range)
        local distance = integration.distance( meKnowledge, friendToSupport )
        if distance > rangeDistance[1] and distance < rangeDistance[2] then
            return 1
        else 
            return 0
        end
    end
end

integration.getSupportDistanceToCoordination = function( agent, pH )
    local rangeDistance = DEC_Tir_PorteeMaxTirIndirectSansChoisirMunition( agent )  -- indirect fire case
    if rangeDistance <= 0 then -- direct fire case
        rangeDistance = DEC_Tir_PorteeMaxPourTirer( agent, pH )
    end
    return rangeDistance
end

-- return the point projection into the entity danger direction vector
integration.getPositionAlongDangerDirection = function( entity, point )
    return DEC_Geometrie_PositionAdvanceAlongDangerDirection( entity.source, point )
end

local getPositionTranslatedDirFromFriend = function( friendToSupport, firingTypeEnumeration ) 
    local integration = integration
    if integration.hasMission( meKnowledge.source ) then
        local rangeDistance = 0
        if firingTypeEnumeration == eIndirectFires then
            rangeDistance = integration.getMaxRangeIndirectFireWithoutSelectAmmo() / 2
        else
            rangeDistance = integration.firingRangeWithDirectFires( 0.5 ) -- direct fire case pH = 0.5
        end
        local mission = DEC_GetRawMission( meKnowledge.source )
        local dir = integration.getDangerousDirection( mission )
        local friendPos = friendToSupport:getPosition()
        return DEC_Geometrie_PositionTranslateDir( friendPos, dir, - rangeDistance )
    end
    return nil
end

-- Returns position to support friend : opposite position from a friend over the danger direction
-- @param friendToSupport : The allied unit to support (agent knowledge type)
-- @param firingTypeEnumeration : enumeration indicated if position should be computed for indirect or direct fires unit
integration.getPositionToSupportFriend = function( friendToSupport, firingTypeEnumeration )
    if firingTypeEnumeration then
        return getPositionTranslatedDirFromFriend( friendToSupport, firingTypeEnumeration )
    else -- backward compatibility
        local integration = integration
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
end

-- Returns the C++ source of a position to support friend, guaranteed to be inside the AOR
-- (as opposed to the position returned by integration.getPositionToSupportFriend)
-- @param friendToSupport : The allied unit to support (agent knowledge type)
-- @param firingTypeEnumeration : enumeration indicated if position should be computed for indirect or direct fires unit
integration.getPositionInAORToSupportFriend = function( friendToSupport, firingTypeEnumeration )

    -- Split the AOR in sections (no need to do it again if it has already been done in the current mission)
    myself.areasInAOR = myself.areasInAOR or integration.splitAORInSections( 100 )
    local areas = myself.areasInAOR

    -- Get the position to support (that might be outside of the AOR)
    local positionToSupport = getPositionTranslatedDirFromFriend( friendToSupport, firingTypeEnumeration )

    -- Find a position at the center of one of the sections of the AOR (therefore necessarily inside the AOR),
    -- so that it is close to the position to support (e.g. find the closest position)
    local bestDistance = math.huge
    local bestPosition
    for i=1, #areas do
        local currentPosition = DEC_Geometrie_CalculerBarycentreLocalisation( areas[i] )
        local currentDistance = integration.computeDistance( positionToSupport, currentPosition )
        if currentDistance < bestDistance then
            bestPosition = currentPosition
            bestDistance = currentDistance
        end
    end
    return bestPosition
end