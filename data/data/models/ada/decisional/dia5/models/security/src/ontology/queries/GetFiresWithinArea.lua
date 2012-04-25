-- ****************************************************************************
-- GetFiresWithinArea
-- Get all known fires within a given area
-- ****************************************************************************
queryImplementation "GetFiresWithinArea" 
{ 
    [ "execute" ] = function ( params )
        local allRes = {}
        local simFires = integration.getObjectsKnowledgeInZoneWithCapacity( "burn", params.area )
        for i = 1, #simFires do
            allRes[ i ] = CreateKnowledge( ontology.classes.Object, simFires[ i ] )
        end
        -- No fires in the area
        if not next( allRes ) then
            meKnowledge:sendReport( eRC_NoFiresWithinArea )
        end
        return allRes
    end
}