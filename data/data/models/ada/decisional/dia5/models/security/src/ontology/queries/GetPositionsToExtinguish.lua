-- ****************************************************************************
-- GetPositionsToExtinguish
-- Get a position to extinguish the fire
-- ****************************************************************************
queryImplementation "GetPositionsToExtinguish" 
{ 
    [ "execute" ] = function ( params )
        local result = {}
        result[ #result + 1 ] = params.fire:getPositionToExtinguish()
        if not next( result ) then
            meKnowledge:sendReport( eRC_NoKeyPositionsFoundInArea )
        end
        return result
    end
}