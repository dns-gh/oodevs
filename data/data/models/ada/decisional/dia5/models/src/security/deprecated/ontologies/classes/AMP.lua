return
{
    getScoreAsMeanToTreat = function( self )
        return self:getProximity() -- by default, the nearest element will be the best.
    end,
    canAnimateIt = function( self )
        return true -- by default means that the unit animate an element only if it is reached.
    end,
    canInstallIt = function( self )
        return true  -- by default means that the unit install an element only if it is reached.
    end,
    canDischargeInIt = function( self )
         return true  -- by default means that the unit discharge in the element only if it is reached.
    end
}