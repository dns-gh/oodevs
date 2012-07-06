-- ****************************************************************************
-- GetPositionsForWork
-- Get a position outside the given work (object, plannedObject and urbanBlocks)
-- ****************************************************************************
queryImplementation "GetPositionsForWork" 
{ 
    [ "execute" ] = function ( params )
        local result = {}
        result[ #result + 1 ] = params.work:getDestinationForWork()
        if not next( result ) then
            meKnowledge:sendReport( eRC_NoKeyPositionsFoundInArea )
        end
        return result
    end
}