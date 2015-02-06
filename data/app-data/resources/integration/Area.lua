------------------------------------------------------------------
--- AREA SPECIFIC INTEGRATION METHODS
------------------------------------------------------------------

--- Allows the unit to decontaminate an area (depollute CBRN agents), a report is sent.
-- @param area an Area knowledge
-- @return Boolean, always return true
integration.decontaminateArea = function( area )            
    _DEC_DecontaminerZone( myself, area.source )
    reportFunction(eRC_ZoneDecontaminee )        
    return true
end

-- $$$ MIA To merge with military
--- Returns the barycenter of the provided area knowledge
-- Note that the barycenter may be outside of the area
-- @param area an Area knowledge
-- @return simulation position
integration.getAreaPositionSecu = function( area )
    area.getAreaPositionResult = area.getAreaPositionResult or DEC_Geometrie_CalculerBarycentreLocalisation( area.source )
    return area.getAreaPositionResult
end

-- $$$ MIA To merge with military
--- Returns a list of positions that the agent can reach in the provided area knowledge
-- The returned positions are guaranteed to be trafficable.
-- @param area an Area knowledge
-- @return list of simulation positions
integration.getAreaPositionsSecu = function( area )
    area.getAreaPositionsResult = area.getAreaPositionsResult or DEC_Geometrie_CalculerTrafficablePointPourPoint( integration.getAreaPosition( area ) )
    return area.getAreaPositionsResult
end