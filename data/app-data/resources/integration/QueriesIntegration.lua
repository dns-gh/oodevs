function integration.getAvoidingPositionsFor( entity )
    return { CreateProxyKnowledge( world.AvoidingArea, entity ) }
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
    local points = integration.getSafetyPositions( radius, safetyDistance )
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
    local crowds = integration.getCrowds()
    local CreateKnowledge = CreateKnowledge
    local DEC_ConnaissancePopulation_EstDansZone = DEC_ConnaissancePopulation_EstDansZone 
    local nCrowd = #crowds
    for i = 1, nCrowd do
        local crowd = crowds[ i ]
        if DEC_ConnaissancePopulation_EstDansZone( crowd, area.source ) then
            allRes[ #allRes + 1 ] = CreateKnowledge( world.Crowd, crowd )
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
            newResult[ #newResult + 1 ] = CreateKnowledge( world.Platoon, terrorist )
        end
    end
    return newResult
end


integration.getImplantationObjects = function( area, nbAreas )
    local objectKn = {}
    local existingObject
    local localisation
    local barycentre
    local nonTrafficablePosition = false
    local positions = {}
    local index = 0
    local subAreas = DEC_Geometry_SplitLocalisation( area.source, nbAreas, nil )
    for _, subArea in pairs( subAreas.first ) do
        index = index + 1
        barycentre = DEC_Geometrie_CalculerBarycentreLocalisation( subArea )
        local platoons = integration.getAgentsWithHQ()
        for i = 1, #platoons do -- Est ce que ce point est trafficable pour tous les pions de l'automate
            if not integration.isPointInUrbanBlockTrafficableForPlatoon( platoons[i],barycentre ) then
                nonTrafficablePosition = true
            end
        end
        if nonTrafficablePosition then -- Ce point n'est pas trafficable pour au moins un pion de l'automate.
            positions = integration.getPointPositions(CreateKnowledge( world.Point, barycentre ))
            local positionInLocalisation = {}
            for j = 1, #positions do
                if integration.isPointInLocalisation(CreateKnowledge( world.Point, positions[j] ), area) then
                    positionInLocalisation[#positionInLocalisation + 1] = positions[j]
                end
            end
            local resultIndex = index % ( #positionInLocalisation + 1 )
            if positionInLocalisation[resultIndex] then
                localisation = DEC_Geometrie_ConvertirPointEnLocalisation( positionInLocalisation[resultIndex] )
            elseif next(positionInLocalisation) then
                localisation = DEC_Geometrie_ConvertirPointEnLocalisation( next(positionInLocalisation) )
            else
                meKnowledge:RC( eRC_TrafficablePositionFindingInZone )
                localisation = DEC_Geometrie_ConvertirPointEnLocalisation( barycentre )
            end
        else 
            localisation = DEC_Geometrie_ConvertirPointEnLocalisation( barycentre )
        end
        
        local name = integration.getName( meKnowledge.source )
        local eTypeObject = eTypeObjectGunArtilleryDeploymentArea
        if string.find(name, "COBRA") ~= nil then
            eTypeObject = eTypeObjectCOBRADeploymentArea
        elseif string.find(name, "LRM") ~= nil then
            eTypeObject = eTypeObjectMrlsDeploymentArea
        elseif string.find(name, "SAM") ~= nil or string.find(name, "Mortier") ~= nil then
            eTypeObject = eTypeObjectMortarDeploymentArea
        end
        
        existingObject = integration.obtenirObjetProcheDe( localisation,  eTypeObject, 10 )
        local object = DEC_CreateDynamicGenObject( eTypeObject, localisation, 0  )
        local toto = CreateKnowledge( world.EngineerObject, object )
        toto.knowledge = existingObject
        objectKn[#objectKn+1] = toto
        
    end
    return objectKn
end

function integration.getCollidingObjectsFromType( type)
    local lstObjects = DEC_Connaissances_CollisionsObjetsDeType( type )
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
