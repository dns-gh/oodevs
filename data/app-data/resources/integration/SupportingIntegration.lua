local defaultPhForPositionTranslation = 0.5 -- used in getPositionTranslatedDirFromFriend
local defaultPhForSupportPosition = 0.7 -- used in integration.getPositionToSupportFriend
local defaultPhForMinRange = 0.9 -- used in integration.firingRangeToSupport
local defaultPhForMaxRange = 0.7 -- used in integration.firingRangeToSupport

--- Returns firing ranges for this entity to support an agent.
--- This method returns a table with indirect fire or direct fire ranges, whichever has
--- the greatest maximal firing range.
-- Note that this method can only be called by an agent.
-- @param phForMinRange Float (between 0 and 1), the wanted probability to hit for the
-- minimal range for direct fire (optional, default value is defaultPhForMinRange = 0.9)
-- @param phForMaxRange Float (between 0 and 1), the wanted probability to hit for the
-- maximal range for direct fire (optional, default value is defaultPhForMaxRange = 0.7)
-- @return List of three elements :
-- <ul> <li> The minimum firing range for this entity to support an agent </li>
-- <li> The maximum firing range for this entity to support an agent </li>
-- <li> Whether or not the ranges in this table pertain to indirect fire </li> </ul>
integration.firingRangeToSupport = function( phForMinRange, phForMaxRange )
    local tirIndirect = { DEC_Tir_PorteeMaxTirIndirectSansChoisirMunition() / 3, DEC_Tir_PorteeMaxTirIndirectSansChoisirMunition() / 2, true }
    local tirDirect = { DEC_Tir_PorteeMaxPourTirer( phForMinRange or defaultPhForMinRange ) / 2, 
                        DEC_Tir_PorteeMaxPourTirer( phForMaxRange or defaultPhForMaxRange ) / 2, false }
    if tirIndirect[2] > tirDirect[2] then
        return tirIndirect
    else
        return tirDirect
    end
end

--- Returns the greatest direct fire range with the given probability to hit
-- Note that this method can only be called by an agent.
-- @param ph Float (between 0 and 1), the desired probability to hit
-- @return Float, distance in meters
integration.firingRangeWithDirectFires = function( ph )
    return DEC_Tir_PorteeMaxPourTirer( ph )
end

--- Returns the indirect fire range
-- Note that this method can only be called by an agent.
-- @return Float, distance in meters
integration.firingRangeWithIndirectFires = function()
    return DEC_Tir_PorteeMaxTirIndirectSansChoisirMunition()
end

--- Returns the support distance necessary for the given agent
--- to remain coordinated with this company.
-- The returned distance is the indirect fire range (if applicable),
-- or the fire range with the wanted probability to hit.
-- Note that this method can only be called by a company.
-- @param agent Simulation agent
-- @param pH Float (between 0 and 1), the desired probability to hit (for direct fire)
-- @return Float, distance in meters
integration.getSupportDistanceToCoordination = function( agent, pH )
    local rangeDistance = DEC_Tir_PorteeMaxTirIndirectSansChoisirMunition( agent )  -- indirect fire case
    if rangeDistance <= 0 then -- direct fire case
        rangeDistance = DEC_Tir_PorteeMaxPourTirer( agent, pH )
    end
    return rangeDistance
end

local getPositionTranslatedDirFromFriend = function( friendToSupport, firingTypeEnumeration, ph ) 
    local integration = integration
    if integration.hasMission( meKnowledge.source ) then
        local rangeDistance = 0
        if firingTypeEnumeration == eIndirectFires then
            rangeDistance = integration.getMaxRangeIndirectFireWithoutSelectAmmo() / 2
        else
            rangeDistance = integration.firingRangeWithDirectFires( ph or defaultPhForPositionTranslation )
        end
        local mission = DEC_GetRawMission( meKnowledge.source )
        local dir = integration.getDangerousDirection( mission )
        local friendPos = friendToSupport:getPosition()
        return DEC_Geometrie_PositionTranslateDir( friendPos, dir, - rangeDistance )
    end
    return nil
end

--- Returns position to support friend (a position behind the unit to support along the danger direction).
-- Returns nil if this entity has no mission.
-- @see integration.getPositionInAORToSupportFriend
-- @param friendToSupport Directia agent or Directia agent knowledge, the unit to support
-- @param firingTypeEnumeration Integer, whether the position should be computed for indirect or direct fire. 
-- @param pH Float (between 0 and 1), the desired probability to hit (optional, default value is defaultPhForSupportPosition = 0.7) 
-- The possible values are :
-- <ul> <li> eDirectFires </li>
-- <li> eIndirectFires </li> </ul>
-- @return Point knowledge, or nil if this entity has no mission
integration.getPositionToSupportFriend = function( friendToSupport, firingTypeEnumeration, ph )
    if firingTypeEnumeration then
        return getPositionTranslatedDirFromFriend( friendToSupport, firingTypeEnumeration )
    else -- backward compatibility
        local integration = integration
        local rangeDistance = integration.getMaxRangeIndirectFireWithoutSelectAmmo() / 2  -- indirect fire case
        if rangeDistance <= 0 then -- direct fire case
            rangeDistance = DEC_Tir_PorteeMaxPourTirer( ph or defaultPhForSupportPosition ) / 2
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

--- Returns the simulation source of a position to support friend, guaranteed to be inside the area of responsibility
-- (as opposed to the position returned by integration.getPositionToSupportFriend)
-- @see integration.getPositionToSupportFriend
-- @param friendToSupport Directia agent (works also for Directia agent knowledges), the allied unit to support
-- @param firingTypeEnumeration Integer, whether the position should be computed for indirect or direct fire. 
-- The possible values are :
-- <ul> <li> eDirectFires </li>
-- <li> eIndirectFires </li> </ul>
-- @return Point knowledge, or nil if this entity has no mission
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

------------------------------------------------------------------
--- DECLARATIONS ENSURING BACKWARDS COMPATIBILITY
------------------------------------------------------------------

--- Deprecated
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