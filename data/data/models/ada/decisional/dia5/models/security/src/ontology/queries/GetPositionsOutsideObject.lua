-- ****************************************************************************
-- GetPositionsOutsideObjects
-- Get all known crowds within a given area
-- ****************************************************************************
queryImplementation "GetPositionsOutsideObjects" 
{ 
    ["execute"] = function ( params )
        local allRes = {}

        -- $$$ MIA TODO

        -- $$$ MIA TEMPORARY IMPLEM:
        local trackingPosition = CreateProxyKnowledge( ontology.classes.TrackingPosition, params.object, { distanceMin = 300 } )
        allRes[ #allRes + 1 ] = trackingPosition

        -- affichePositions( allRes ) -- $$$ MIA: to debug. Remove for release
        -- NO position
        if not next( allRes ) then
            meKnowledge:sendReport( eRC_NoPositionsToReachTargets )
        end
        return allRes
    end
}