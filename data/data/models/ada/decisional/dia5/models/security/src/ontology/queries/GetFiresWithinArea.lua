-- ****************************************************************************
-- GetFiresWithinArea
-- Get all known fires within a given area
-- ****************************************************************************
queryImplementation "GetFiresWithinArea" 
{ 
    [ "execute" ] = function ( params )
        local allRes = {}
        local simfires = DEC_ObjectKnowledgesInZone( params.area.source, { "Feu urbain", "Arson/Fire" } ) -- $$$ MIA TEMP
        for _, simFire in pairs( simfires ) do
            allRes[ #allRes + 1 ] = CreateKnowledge( ontology.classes.concretes.Object, simFire )
        end

        -- affichePositions( allRes ) -- $$$ MIA: to debug. Remove for release
        -- NO Crowds in the area
        if not next( allRes ) then
            DEC_Trace( "Hello girls!" )
            meKnowledge:sendReport( eRC_NoFiresWithinArea )
        end
        return allRes
    end
}