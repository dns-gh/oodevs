-- Deprecated
function integration.getAvoidingPositionsFor( entity )
    return { CreateProxyKnowledge( integration.ontology.types.avoidingArea, entity ) }
end

--- Returns all the objects with at least one of the provided types in the given area.
-- @see Types.lua for the object types enumeration
-- @param localisation Simulation area
-- @param objectTypes List, the sought object types (e.g. eTypeObjectMines, eTypeObjectLinearMinedArea...)
-- @return List of object knowledges
function integration.getObjectsInArea( localisation, objectTypes )
    local integration = integration
    local lstObjects = DEC_ObjectKnowledgesInZone( localisation, objectTypes )
    if not lstObjects then
        return {}
    end
    
    local res = {}
    local CreateKnowledge = CreateKnowledge
    for i = 1, #lstObjects do
        res[ i ] = CreateKnowledge( integration.ontology.types.object, lstObjects[ i ] )
    end
    return res
end

--- Returns a list of safety positions defined as :
-- <ul> <li> crossroads at the given radius of this entity </li>
-- <li> positions along roads at the given safety distance of the previously found crossroads </li> </ul>
-- This method can only be called by an agent.
-- @see integration.findSafetyPositions
-- @param radius Float, the radius around the entity (in meters)
-- @param safetyDistance Float, the safety distance around the crossroads (in meters)
-- @return List of point knowledges
function integration.getSafetyPositions( radius, safetyDistance )
    -- Warning: If this method is called with different parameters, the simulation cache will be invalidated so
    -- it's good practice not to call it with different parameters for the same brain.
    local points = integration.findSafetyPositions( radius, safetyDistance )
    local result = {}
    local CreateKnowledge = CreateKnowledge
    for i = 1, #points do
        result[ i ] = CreateKnowledge( integration.ontology.types.point, points[ i ] )
    end
    return result
end

--- Returns all the crowds in the given area.
-- This method can only be called by an agent or by a company.
-- @see integration.getCrowds
-- @param area Area knowledge
-- @return List of crowd knowledges
function integration.getCrowdsInArea( area )
    local allRes = {}
    local crowds = integration.getCrowds()
    local CreateKnowledge = CreateKnowledge
    local DEC_ConnaissancePopulation_EstDansZone = DEC_ConnaissancePopulation_EstDansZone 
    for i = 1, #crowds do
        local crowd = crowds[ i ]
        if DEC_ConnaissancePopulation_EstDansZone( crowd, area.source ) then
            allRes[ #allRes + 1 ] = CreateKnowledge( integration.ontology.types.population, crowd )
        end
    end
    return allRes
end

--- Returns a list of all terrorist agent knowledges at the given distance of this entity.
-- This method can only be called by an agent.
-- @param distance Float, the distance (in meters, 600 by default).
-- @return List of agent knowledges
function integration.getNearbyTerrorists( distance )
    local terrorists = DEC_Connaissances_TerroristsAProximite( distance or 600 )    
    local newResult = {}
    for i = 1, #terrorists do
        newResult[ i ] = CreateKnowledge( integration.ontology.types.agentKnowledge, terrorists[ i ] )
    end
    return newResult
end

--- Returns a list of deployment areas close to the given area.
-- This method can only be called by an agent.
-- @param area Area knowledge
-- @param nbAreas Integer, the number of area subdivisions searched
-- during the execution of the method
-- @return List of object knowledges
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
                break
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
                reportFunction( eRC_TrafficablePositionFindingInZone )
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

--- Returns a list of all objects of the given type colliding with this entity.
-- This method can only be called by an agent.
-- @see Types.lua for the object types enumeration
-- @param objectType String, the sought object type (e.g. eTypeObjectMines, eTypeObjectLinearMinedArea...)
-- @return List of object knowledges
function integration.getCollidingObjectsFromType( objectType )
    local lstObjects = DEC_Connaissances_CollisionsObjetsDeType( objectType )
    if not lstObjects then
        return {}
    end
    
    local res = {}
    local integration = integration
    local CreateKnowledge = CreateKnowledge
    for i = 1, #lstObjects do
        res[ i ] = CreateKnowledge( integration.ontology.types.object, lstObjects[ i ] )
    end
    return res
end

--- Returns a list of all detected agent knowledges in
-- this entity's current area of responsibility.
-- This method can only be called by an agent.
-- @return List of agent knowledges
integration.unitesDetecteesDansFuseau = function()
    return DEC_Connaissances_UnitesDetecteesDansFuseau()
end

--- Convert a list of elements (companies and units) into a list of valid agents :
-- <ul> <li> Companies will be converted to the list of its valid subordinates </li>
-- <li> Agent knowledges will be converted to agents </li> </ul>
-- @param elements List of company knowledges and/or DirectIA agent knowledges.
-- Agent knowledges must define a "isValid" method returning true if the knowledge is valid,
-- false otherwise.
-- @return List of valid DirectIA agents
integration.getAgentFromListOfElements = function ( elements )
    local entities = {}
    for i = 1, #elements do
        if masalife.brain.core.class.isOfType( elements[i], integration.ontology.types.automat ) then -- it can be a company
            local entitiesFromAutomat = integration.getEntitiesFromAutomat( elements[i], "none", true)
            for j = 1, #entitiesFromAutomat do
                if entitiesFromAutomat[j]:isValid() then
                    entities[#entities + 1] = entitiesFromAutomat[j]
                end
            end
        else -- wa can support units
            if elements[i]:isValid() then
                entities[#entities + 1] = integration.getAgentFromKnowledge(elements[i])
            end
        end
    end
    return entities
end
