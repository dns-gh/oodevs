queryImplementation "getPositionsToReinforce" 
{ 
    ["execute"] = function ( params )
        local allRes = {}

        -- -------------------------------------------------------------------------------- 
        --  Get a simple tracking position with distance min and distance max
        -- if tracking position is no longer accessible, find close positions 
        -- --------------------------------------------------------------------------------
        for _, elementToReinforce in pairs( params.elementsToReinforce ) do
            local reinforcingPosition = CreateProxyKnowledge( 
                sword.military.world.ReachingArea, elementToReinforce, 
                { distanceMin = params.distanceMin } )
            if reinforcingPosition:isReachingFor( elementToReinforce ) then
                allRes[ #allRes + 1 ] = reinforcingPosition
            else -- find close positions around element to reinforce
                local simlocalisations = elementToReinforce:getPositions()  -- Sim localisations, not reaching elements
                for _, localisation in pairs ( simlocalisations ) do -- create point as reaching elements
                    point = CreateKnowledge( sword.military.world.Point, localisation )
                    if point:isReachingFor( element ) then
                        allRes[ #allRes + 1 ] = point
                    end
                end
            end
        end
        return allRes
    end
}