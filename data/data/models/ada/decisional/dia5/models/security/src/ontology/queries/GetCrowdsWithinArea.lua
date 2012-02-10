-- ****************************************************************************
-- GetCrowdsWithinArea
-- Get all known crowds within a given area
-- ****************************************************************************
queryImplementation "GetCrowdsWithinArea" 
{ 
    ["execute"] = function ( params )
        local allRes = {}

        local simCrowds = integration.getCrowds() -- returns all known crowds around
        for _, simCrowd in pairs( simCrowds ) do
            local simCrowdPosition = integration.getHeadPosition( simCrowd )
            if DEC_Geometrie_EstPointDansLocalisation( simCrowdPosition , params.area.source ) then
                allRes[ #allRes + 1 ] = CreateKnowledge( ontology.classes.Crowd, simCrowd )
            end
        end
        -- affichePositions( allRes ) -- $$$ MIA: to debug. Remove for release
        -- NO Crowds in the area
        if not next( allRes ) then
            meKnowledge:sendReport( eRC_NoCrowdsWithinArea )
        end
        return allRes
    end
}