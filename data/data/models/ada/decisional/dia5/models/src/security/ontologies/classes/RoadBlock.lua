return
{
    canAnimateIt = function( self )
        return true -- by default means that the unit animate an element only if it is reached.
    end,
    canInstallIt = function( self )
        return true  -- by default means that the unit install an element only if it is reached.
    end,
    installIt = function( self )
        return self:buildIt( self:getPositions() )
    end
}