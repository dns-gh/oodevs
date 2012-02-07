function integration.getAvoidingPositionsFor( entity )
    return { CreateProxyKnowledge( sword.military.world.AvoidingArea, entity ) }
end

function integration.getObjectsInArea( localisation, objectTypes )
    local lstObjects = DEC_ObjectKnowledgesInZone( localisation, objectTypes )
    if not lstObjects then
        return {}
    end
    
    local res = {}
    local nObjects = #lstObjects
    local CreateKnowledge = CreateKnowledge
    for i = 1, nObjects do
        local object = lstObjects[ i ]
        res[#res + 1] = CreateKnowledge( integration.ontology.types.object, object )
    end
    return res
end

function integration.getSafetyPositions( radius, safetyDistance )
    -- Warning: If this method is called with different parameters, the simulation cache will be invalidated so
    -- it's good practice not to call it with different parameters for the same brain.
    local points = DEC_FindSafetyPositions( radius, safetyDistance )
    local result = {}
    local CreateKnowledge = CreateKnowledge
    local nPoints = #points
    for i = 1, nPoints do
        local point = points[ i ]
        result[ #result + 1 ] = CreateKnowledge( integration.ontology.types.point, point )
    end
    return result
end

function integration.getCrowdsInArea( area )
    local allRes = {}
    local crowds = DEC_Connaissances_Populations()
    local CreateKnowledge = CreateKnowledge
    local DEC_ConnaissancePopulation_EstDansZone = DEC_ConnaissancePopulation_EstDansZone 
    local nCrowd = #crowds
    for i = 1, nCrowd do
        local crowd = crowds[ i ]
        if DEC_ConnaissancePopulation_EstDansZone( crowd, area.source ) then
            allRes[ #allRes + 1 ] = CreateKnowledge( sword.military.world.Crowd, crowd )
        end
    end
    local nRes = #allRes
    for j = 1, nRes do
        local crowd = allRes[ j ]
        if not DEC_ConnaissancePopulation_EstDansZone( crowd.source, area.source ) then
            allRes[ j ] = nil
        end
    end
    return allRes
end

function integration.getNearbyTerrorists()
    local terrorists = DEC_Connaissances_TerroristsAProximite( 600 )    
    local newResult = {}
    local nTerrorists = #terrorists
    for i = 1, nTerrorists do
        local terrorist = terrorists[i]
        if not exists( newResult, terrorist ) then
            newResult[ #newResult + 1 ] = CreateKnowledge( sword.military.world.Platoon, terrorist )
        end
    end
    return newResult
end


integration.getImplantationObjects = function( area, nbAreas )
    local objectKn = {}
    local existingObject
    local localisation
    local barycentre
    local subAreas = DEC_Geometry_SplitLocalisation( area.source, nbAreas, nil )
    for _, subArea in pairs( subAreas.first ) do
        barycentre = DEC_Geometrie_CalculerBarycentreLocalisation( subArea )
        localisation = DEC_Geometrie_ConvertirPointEnLocalisation( barycentre )
        existingObject = integration.obtenirObjetProcheDe( localisation,  eTypeObjectZoneImplantationCanon, 10 )
        local object = DEC_CreateDynamicGenObject( S_TypeObject_ToString( eTypeObjectZoneImplantationCanon ), localisation, 0  )
        local toto = CreateKnowledge( sword.military.world.EngineerObject, object )
        toto.knowledge = existingObject
        objectKn[#objectKn+1] = toto
    end
    return objectKn
end
