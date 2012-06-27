-- GetResourceNodesWithinArea
-- Try to find Resource nodes within an area passed as parameter.

-- >/!\ Tester que si un UB/object contient pls noeuds, que leur dessactivation
-- ne requiert qu'une seule visite de l'UB/object
-- ****************************************************************************
queryImplementation "GetResourceNodesWithinArea" 
{
    ["execute"] = function ( params )
        local allRes = {}
        local resourceNodes = DEC_ResourceNetwork_NodesInZone( meKnowledge.source, params.area.source )
        for _, resourceNode in pairs( resourceNodes ) do
            allRes[ #allRes + 1 ] = CreateKnowledge( ontology.classes.ResourceNode, resourceNode )
        end
        -- affichePositions( allRes ) -- $$$ MIA: to debug.
        if not next( allRes ) then
            meKnowledge:sendReport( eRC_NoResourceNodesWithinArea )
        end
        return allRes
    end
}
