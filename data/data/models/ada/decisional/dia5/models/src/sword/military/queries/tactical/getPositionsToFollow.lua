queryImplementation "getPositionsToFollow" 
{ 
    ["execute"] = function ( params )
        local allRes = {}

        -- -------------------------------------------------------------------------------- 
        --  Get a simple tracking position with distance min and distance max
        -- if tracking position is no longer accessible, find close positions 
        -- --------------------------------------------------------------------------------
        for _, elementToFollow in pairs( params.elementsToFollow ) do
            local trackingPosition = CreateProxyKnowledge( 
                sword.military.world.SupportingArea, elementToFollow, 
                { distanceMin = params.distanceMin } ) -- suivre à distance de tir
            if trackingPosition:isReachingFor( elementToFollow ) then
                allRes[ #allRes + 1 ] = trackingPosition
            else -- find close positions around element to follow
                local simlocalisations = elementToFollow:getPositions()  -- Sim localisations, not reaching elements
                for _, localisation in pairs ( simlocalisations ) do -- create point as reaching elements
                    point = CreateKnowledge( sword.military.world.Point, localisation )
                    if point:isReachingFor( element ) then
                        allRes[ #allRes + 1 ] = point
                    end
                end
            end
        end
        -- affichePositions( allRes )
        -- NO Reaching POSITIONS to follow
        if next( params.elementsToFollow ) and not next( allRes ) then
            meKnowledge:RC( eRC_NoPositionsToReachTargets )
        end
        return allRes
    end
}