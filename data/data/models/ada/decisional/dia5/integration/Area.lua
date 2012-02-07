------------------------------------------------------------------
--- AREA SPECIFIC INTEGRATION METHODS
------------------------------------------------------------------
integration.decontaminateArea = function( area )            
    DEC_DecontaminerZone( area.source )          
    -- meKnowledge:RC( eRC_??? )   
    return true
end
