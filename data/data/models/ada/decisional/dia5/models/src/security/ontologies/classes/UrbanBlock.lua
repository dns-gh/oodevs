return
{
    canEvacuateIt = function( self )
        return true -- by default means that the unit evcuate an element only if it is reached.
    end,
    
    pillage = function( self )
        DEC_Trace( "Block pillaged" )
    end,
    
    canEvacuateIt = function()
        return false
    end,
    evacuateIt = function()
        return false
    end,
}