queryImplementation "getPositionsToRemove" 
{ 
    ["execute"] = function ( params )
        local allRes = knowledgeManager.getQueryResult( "getPositionsToRemove" )
        if not next( allRes ) then
            allRes = {}
            local point, objectPosition, scaledObject
            for _, element in pairs ( params.elementsToRemove ) do
                if element:isValid() then -- when the mission is given a second time by lead, the element could be already removed     
                    scaledObject = DEC_Geometrie_AgrandirLocalisation(  element:getLocalisation() , 50 )
                    objectPosition = DEC_Geometrie_ComputeNearestBorder( meKnowledge:getPosition(), scaledObject )
                    point = CreateKnowledge( world_elements.Point, objectPosition )
                    allRes[ #allRes + 1 ] = point
                end
            end
            knowledgeManager.setQueryResult( "getPositionsToRemove", allRes )
        end
        return allRes
    end
}