-- ****************************************************************************
-- getPositionsToFollow
-- Try to find reaching positions to reach elements issued as parameters.
-- ****************************************************************************
queryImplementation "GetPositionsToFollow" 
{ 
    ["execute"] = function ( params )
        local allRes = {}

        -- -------------------------------------------------------------------------------- 
        --  Get a simple tracking position with distance min and distance max
        -- if tracking position is no longer accessible, find close positions 
        -- --------------------------------------------------------------------------------
        for _, agent in pairs( params.agents ) do
            local trackingPosition = CreateProxyKnowledge( 
                ontology.classes.concretes.TrackingPosition, agent, 
                { distanceMin = params.distanceMin } )
            if trackingPosition:isAccessible() then
                allRes[ #allRes + 1 ] = trackingPosition
            else -- find close positions around element to follow
                local simlocalisations = agent:getPositions()  -- Sim localisations, not reaching elements
                for _, localisation in pairs ( simlocalisations ) do -- create point as reaching elements
                    point = CreateKnowledge( ontology.classes.concretes.Position, localisation )
                   if point:isAccessible() then
                        allRes[ #allRes + 1 ] = point
                    end
                end
            end
        end
        -- affichePositions( allRes ) -- $$$ MIA: to debug. Remove for release
        -- NO Reaching POSITIONS to follow
        if next( params.agents ) and not next( allRes ) then
            meKnowledge:sendReport( eRC_NoPositionsToReachTargets )
        end
        return allRes
    end
}