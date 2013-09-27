function integration.getAvoidingPositionsFor( entity )
    return { CreateProxyKnowledge( integration.ontology.types.avoidingArea, entity ) }
end

function integration.getObjectsInArea( localisation, objectTypes )
    local integration = integration
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
            allRes[ #allRes + 1 ] = CreateKnowledge( integration.ontology.types.population, crowd )
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

function integration.getNearbyTerrorists( distance )
    local minDistance = distance
    if minDistance == nil then -- not issued
        minDistance = 600
    end
    local terrorists = DEC_Connaissances_TerroristsAProximite( minDistance )    
    local newResult = {}
    local nTerrorists = #terrorists
    for i = 1, nTerrorists do
        local terrorist = terrorists[ i ]
        if not exists( newResult, terrorist ) then
            newResult[ #newResult + 1 ] = CreateKnowledge( integration.ontology.types.agentKnowledge, terrorist )
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
    local DEC_Geometrie_ConvertirPointEnLocalisation = DEC_Geometrie_ConvertirPointEnLocalisation
    local CreateKnowledge = CreateKnowledge
    local DEC_CreateDynamicGenObject = DEC_CreateDynamicGenObject
    local integration = integration
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
            positions = integration.getPointPositions( CreateKnowledge( integration.ontology.types.point, barycentre ) )
            local positionInLocalisation = {}
            for j = 1, #positions do
                if integration.isPointInLocalisation( CreateKnowledge( integration.ontology.types.point, positions[j] ), area ) then
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
        
        local name = integration.getName( meKnowledge )
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
        local toto = CreateKnowledge( integration.ontology.types.genObject, object )
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
    local integration = integration
    local CreateKnowledge = CreateKnowledge
    for i = 1, nObjects do
        local object = lstObjects[ i ]
        res[#res + 1] = CreateKnowledge( integration.ontology.types.object, object )
    end
    return res
end

integration.unitesDetecteesDansFuseau = function()
    return DEC_Connaissances_UnitesDetecteesDansFuseau()
end

-- Convert a list of elements (companies and units) into a list of valid agents
-- @param a list of elements like companies or units
-- @return a list of valid agents
integration.getAgentFromListOfElements = function ( elements )
    local entities = {}
    for _, element in pairs( elements ) do
        if masalife.brain.core.class.isOfType( element, integration.ontology.types.automat ) then -- it can be a company
            local entitiesFromAutomat = integration.getEntitiesFromAutomat( element, "none", true)
            for j = 1, #entitiesFromAutomat do
                if entitiesFromAutomat[j]:isValid() then
                    entities[#entities + 1] = entitiesFromAutomat[j]
                end
            end
        else -- wa can support units
            if element:isValid() then
                entities[#entities + 1] = integration.getAgentFromKnowledge(element)
            end
        end
    end
    return entities
end
