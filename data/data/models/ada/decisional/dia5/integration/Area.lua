------------------------------------------------------------------
--- AREA SPECIFIC INTEGRATION METHODS
------------------------------------------------------------------
integration.decontaminateArea = function( area )            
    DEC_DecontaminerZone( area.source )
    integration.pionRC( eRC_DecontaminationDone )        
    return true
end