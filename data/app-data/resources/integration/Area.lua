------------------------------------------------------------------
--- AREA SPECIFIC INTEGRATION METHODS
------------------------------------------------------------------
integration.decontaminateArea = function( area )            
    DEC_DecontaminerZone( area.source )
    integration.pionRC( eRC_DecontaminationDone )        
    return true
end

-- $$$ MIA To merge with military
integration.getAreaPositionSecu = function( area )
    area.getAreaPositionResult = area.getAreaPositionResult or DEC_Geometrie_CalculerBarycentreLocalisation( area.source )
    return area.getAreaPositionResult
end

-- $$$ MIA To merge with military
integration.getAreaPositionsSecu = function( area )
    area.getAreaPositionsResult = area.getAreaPositionsResult or DEC_Geometrie_CalculerTrafficablePointPourPoint( integration.getAreaPosition( area ) )
    return area.getAreaPositionsResult
end