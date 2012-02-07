-- ****************************************************************************
-- getPositionsToReach
-- Try to find reaching positions for elements to reach issued as paramters.
-- ****************************************************************************
queryImplementation "getPositionsToReach" 
{ 
    [ "execute" ] = function ( params )
        local newResult = {}

        -- -------------------------------------------------------------------------------- 
        --  Simple position
        -- --------------------------------------------------------------------------------
        for _, element in pairs ( params.elementsToReach ) do
            local position = element:getMyPosition() -- reaching element
            if position:isReachingFor( element ) then
                newResult[ #newResult + 1 ] = position
            end -- cas des BU non traficable par exemple
        end

        -- -------------------------------------------------------------------------------- 
        --  No positions, try to find accessible ones
        -- --------------------------------------------------------------------------------
        if not next( newResult ) then
            for _, elementToReach in pairs ( params.elementsToReach ) do
                local simPositions = elementToReach:getPositions()
                for _, simPos in pairs( simPositions ) do
                    local kPos = CreateKnowledge( sword.military.world.Point, simPos )
                    if kPos:isReachingFor( elementToReach ) then
                        newResult[ #newResult + 1 ] = kPos
                    end
                end
            end
        end
       -- affichePositions( newResult )
        -- NO Reaching POSITIONS
        if next( params.elementsToReach ) and not next( newResult ) then
             meKnowledge:RC( eRC_NoPositionsToReachTargets )
        end
        return newResult
    end
}