return
{
    getScoreAsMeanToTreat = function( self )
        return self:getProximity() -- by default, the nearest element will be the best.
    end,
    canDischargeInIt = function( self )
         return true  -- by default means that the unit discharge in the element only if it is reached.
    end
}