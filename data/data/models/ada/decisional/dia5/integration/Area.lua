------------------------------------------------------------------
--- AREA SPECIFIC INTEGRATION METHODS
------------------------------------------------------------------
integration.decontaminateArea = function( area )            
    DEC_DecontaminerZone( area.source )           
    return true
end
