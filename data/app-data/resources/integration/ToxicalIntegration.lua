integration.isDetectedToxicalPlume = function( agent )
    -- $$$ GGE Wait lgy DEC function meanwhile test with NBC
    local objects = {}
    objects = integration.getCollidingObjectsFromType( "nbc cloud" )
    if #objects > 0 then
        return true
    else
        return false
    end
end