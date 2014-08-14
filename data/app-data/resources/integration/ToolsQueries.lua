--- Filters the given knowledges by keeping only those reaching for at least one of
--- the given objectives, and returns the resulting list
-- @param knowledges Table of knowledges, each defining an "isReachingFor" method
-- @param objectives Table of objectives
-- @return List of filtered knowledges
integration.tableIsReachingFor = function( knowledges, objectives )
    local res = {}
    for _, knowledge in pairs( knowledges ) do
        for _, objective in pairs( objectives ) do
            if objective:isValid() and knowledge:isReachingFor(objective) then
                res [ #res + 1 ] = knowledge
                break
            end
        end
    end
    return res
end

--- Filters the given knowledges by keeping only those destroying for at least one of
--- the given objectives, and returns the resulting list
-- @see integration.tableIsNeutralizingFor
-- @param knowledges Table of knowledges, each defining an "isDestroyingFor" method
-- @param objectives Table of objectives
-- @return List of filtered knowledges
integration.tableIsDestroyingFor = function( knowledges, objectives )
    local res = {}
    for _, knowledge in pairs( knowledges ) do
        for _, objective in pairs( objectives ) do
            if objective:isValid() and knowledge:isDestroyingFor( objective ) then
                res [ #res + 1 ] = knowledge
                break
            end
        end
    end
    return res
end

--- Filters the given knowledges by keeping only those neutralizing for at least one of
--- the given objectives, and returns the resulting list
-- @see integration.tableIsDestroyingFor
-- @param knowledges Table of knowledges, each defining an "isNeutralizingFor" method
-- @param objectives Table of objectives
-- @return List of filtered knowledges
integration.tableIsNeutralizingFor = function( knowledges, objectives )
    local res = {}
    for _, knowledge in pairs( knowledges ) do
        for _, objective in pairs( objectives ) do
            if objective:isValid() and knowledge:isNeutralizingFor(objective) then
                res [ #res + 1 ] = knowledge
                break
            end
        end
    end
    return res
end

integration.query = {}

local FragOrderType = function ( FragOrders )
    for parameterName, parameterValue in pairs( FragOrders ) do
        if parameterName ~= "orderType" then
            myself.FragOrders[parameterName] = nil
            return eFragOrderIsIncorrect
        elseif parameterValue == "Rep_OrderConduite_Pion_IndirectFire" then
            return eFragOrderIsIndirectFire
        end
        myself.FragOrders[parameterName] = nil -- unknown : normally never happens
        return eFragOrderIsUnknown
    end
end

local treatIndirectFireFragOrder = function ( )
    local params = {}
    for parameterName, parameterValue in pairs( myself.FragOrders ) do
        if( type(parameterValue) == "table" ) then
            params.entity = parameterValue.entity
            params.munition = parameterValue.munition
            params.interventionType = parameterValue.interventionType
            params.requester = parameterValue.requester
        end
    end

    myself.FragOrders = nil
    return params
end

local isApplyFireOrder = function( order )
    local orderType = integration.getAnyType( order )
    local taskKnowledge = integration.taskKnowledge[ orderType ]
    if taskKnowledge and taskKnowledge.isApplyFire then
        return taskKnowledge:isApplyFire()
    end
    if orderType == "platoon.combat.support.art.tasks.AppliquerFeux" 
        or orderType == "Rep_OrderConduite_Pion_AppliquerFeux" 
        or orderType == "france.military.platoon.combat.support.art.tasks.AppliquerFeux" then
        return true
    end
    return false
end

--- Returns the parameters of the first received fire fragmentary order
-- Removes the fragmentary order from the list of received fragmentary orders.
-- The fragmentary orders with the following DIA types are considered fire orders :
-- <ul> <li> "platoon.combat.support.art.tasks.AppliquerFeux" </li>
-- <li> "Rep_OrderConduite_Pion_AppliquerFeux" </li>
-- <li> "france.military.platoon.combat.support.art.tasks.AppliquerFeux" </li> </ul>
-- Returns nil if no fire fragmentary order was retrieved
-- @return Table with "entities", "munition" and "interventionType" as keys,
-- or nil if no fire fragmentary order was retrieved
integration.query.getFirstFireOrder = function( )
    local ordres_recus = DEC_GetOrdersCategory()
    local integration = integration
    for _, x in pairs( ordres_recus or emptyTable ) do
        if isApplyFireOrder( x ) then
            local res = CreateKnowledge( integration.ontology.types.fragOrder, x )
            DEC_RemoveFromOrdersCategory( x )
            DEC_DeleteRepresentation( x )
            return res:getParameters()
        end
    end

    if myself.FragOrders ~= nil then
        local fragOrderType = FragOrderType( myself.FragOrders )
        if fragOrderType == eFragOrderIsIncorrect or fragOrderType == eFragOrderIsUnknown then
            return nil
        end
        if fragOrderType == eFragOrderIsIndirectFire then
            return treatIndirectFireFragOrder( )
        end
    end

    return nil
end

--- Returns the knowledge of the first fragmentary order with the given type
-- Removes the fragmentary order from the list of received fragmentary orders.
-- Returns nil if no fragmentary order with the given type can be retrieved
-- @param fragOrderType String, the DIA type of the fragmentary order
-- @return Fragmentary order knowledge, or nil if no fragmentary order with the given type was retrieved
integration.query.getFirstFragOrderFromType = function( fragOrderType )
    local ordres_recus = DEC_GetOrdersCategory()
    local integration = integration
    for _, x in pairs( ordres_recus or emptyTable ) do
        if integration.getAnyType( x ) == fragOrderType then
            local res = CreateKnowledge( integration.ontology.types.fragOrder, x )
            DEC_RemoveFromOrdersCategory( x )
            DEC_DeleteRepresentation( x )
            return res
        end
    end
    for k,x in pairs( myself.fragOrders or emptyTable ) do
        if integration.getAnyType( x.source ) == fragOrderType then
            myself.fragOrders[k] = nil
            return x
        end
    end
    return nil
end

--- Returns a list of nearby enemy knowledges
-- An agent knowledge is deemed nearby if its "isNearby" method (that it must
-- define) returns the boolean value True.
-- @return List of agent knowledges
integration.query.getNearbyPlatoons = function( )
    local nearbyPlatoons = {}
    local destroyable = {}
    DEC_Connaissances_Destroyable( destroyable )
    for i = 1, #destroyable do
        local platoon = destroyable[i]
        if platoon:isNearby() then
            nearbyPlatoons[ #nearbyPlatoons + 1 ] = platoon
        end
    end
    return nearbyPlatoons
end

--- Splits the current area of responsibility into the given number of sub-areas,
--- and returns a list of those sub-areas of responsibility
-- @param nbSubAORs Integer, the number of sub-areas of responsibility
-- @return List of simulation areas of responsibility
integration.query.getFuseaux = function( nbSubAORs )
    return DEC_DecouperFuseau( nbSubAORs )
end

--- Returns the appropriate number of units to put in the first
--- echelon of a company with the given number of echelons.
-- Note that this method can only be called by a company.
-- If the provided number of echelon is equal to zero, then this method will return
-- the number of platoons in the company.
-- @param nbrEchelon Integer, the number of echelons of the company
-- @return Integer, the number of units to put in the first echelon
integration.query.getNbrFront = function( nbrEchelon )
    if nbrEchelon == NIL or nbrEchelon == 0 then
        return #( DEC_Automate_PionsAvecPC() ) -- all
    end
    local pions = DEC_Automate_PionsMelee()
    return  math.max(1, ( #pions )/ nbrEchelon )
end

--- Returns the total number of engineer units in this company.
-- Note that this method can only be called by a company.
-- @return Integer, the number of engineer units in this company.
integration.query.getNbrGEN = function()
    return #( DEC_Automate_PionsGenie() ) -- all
end

--- Filters the given entities by keeping only those without a mission,
--- and returns the resulting list
-- @param Table of directia agent knowledges, all of them defining a "hasMission" method
-- @return List of directia agent knowledges
integration.query.getUnitWithoutMission = function( entities )
    local result = {}
    for _, unit in pairs( entities ) do
        if not unit:hasMission() then
            result[ #result + 1 ] = unit
        end
    end
    return result
end

--- Returns a list of reaching positions for the given table of reachable elements.
-- The returned positions are guaranteed to be trafficable.
-- @param reachableElements Table of reachable knowledges, all of them defining a "getMyPosition" and a "getPositions" method
-- @return List of point knowledges
integration.query.getPositionsToReach = function( reachableElements )
    local currentPositions = {}
    local CreateKnowledge = CreateKnowledge
    for _, element in pairs ( reachableElements ) do
        local point = element:getMyPosition() -- reaching element
        if point:isReachingFor( element ) then
            currentPositions[ #currentPositions + 1 ] = point
        else -- cas des BU non traficable par exemple.
            local simlocalisations = element:getPositions()  -- Sim localisations, not reaching elements
            for _, localisation in pairs ( simlocalisations ) do -- create point as reaching elements
                point = CreateKnowledge( integration.ontology.types.point, localisation )
                if point:isReachingFor( element ) then
                    currentPositions[ #currentPositions + 1 ] = point
                end
            end
        end
    end
    return currentPositions
end

--- Returns the knowledge of the nearest position around the given reachable element.
-- @param element Knowledge of a reachable element (e.g. area), defining a "getPositions" method
-- @return Point knowledge
integration.query.getNearestPointFor = function( element ) -- reachable
    local positions = element:getPositions()
    local nearest = positions[ 1 ]
    local DEC_Geometrie_DistanceBetweenPoints = DEC_Geometrie_DistanceBetweenPoints
    local myPosition = meKnowledge:getPosition()
    local nearestDistance = DEC_Geometrie_DistanceBetweenPoints( myPosition, nearest )
    for i = 2, #positions do
        local position = positions[i]
        local currentDistance = DEC_Geometrie_DistanceBetweenPoints( myPosition, position )
        if currentDistance < nearestDistance then
            nearest = position
            nearestDistance = currentDistance
        end
    end
    return CreateKnowledge( integration.ontology.types.point, nearest )
end

--- Returns the knowledge of given unit's company
-- Returns nil if an error occurs in this method
-- @param unit Directia agent
-- @return Company knowledge, or nil if an error occurs in this method
integration.GetSuperiorKnowledge = function( unit ) 
    local result, company = pcall( DEC_GetAutomate, unit.source )
    if result then
        return CreateKnowledge( integration.ontology.types.automat, company )
    end
    return nil
end

--- Returns a list of all the knowledges of the subordinates of the provided company.
-- The returned subordinates may be filtered by their role, according to the role parameter.
-- The HQ unit may or may not be among the returned subordinates, according to the withHQ parameter.
-- All the knowledges of subordinates that define a "isDestroyed" method will not be in the resulting
-- list if this "isDestroyed" method returns true when called upon the knowledge.
-- @param company Company knowledge
-- @param role String, the name of the role to filter the subordinates with.
-- If this parameter equals "none", nil or NIL, then no filtering takes place.
-- @param withHQ Boolean, whether or not the HQ unit will be included in the list of subordinates.
-- @return List of directia agent knowledges
integration.getEntitiesFromAutomat = function ( company, role, withHQ )
    local integration = integration
    local CreateKnowledge = CreateKnowledge
    local temp
    if withHQ then
        temp = integration.getSubordinateAgentsFromCommander( company )
    else
        temp = DEC_Automate_PionsDeAutomateSansPC( company.source )
    end
    local knowledges = {}
    for i = 1, #temp do
        local pion = temp[i]
        local knowledge = CreateKnowledge( integration.ontology.types.agent, pion )
        local fun = function() return knowledge:isDestroyed() end
        local ok, isDestroyed = pcall( fun )
        if ok then
            if not isDestroyed then
                knowledges[ #knowledges + 1 ] = knowledge
            end
        else
            knowledges[ #knowledges + 1 ] = knowledge
        end
    end

    if role ~= "none" and role ~= NIL and role ~= nil then
        return integration.filterPionWithRole( knowledges, role )
    else
        return knowledges
    end
 end

--- Returns a list of all the knowledges of the subordinates of the provided company, so long
-- as communication between those subordinates and the HQ unit is possible (i.e. a subordinate in 
-- a jamming area will not be returned by this method, a destroyed HQ unit will result in an empty list, etc.)
-- The returned subordinates may be filtered by their role, according to the role parameter.
-- The HQ unit may or may not be among the returned subordinates, according to the withHQ parameter.
-- All the knowledges of subordinates that define a "isDestroyed" method will not be in the resulting
-- list if this "isDestroyed" method returns true when called upon the knowledge.
-- @param company Company knowledge
-- @param role String, the name of the role to filter the subordinates with.
-- If this parameter equals "none", nil or NIL, then no filtering takes place.
-- @param withHQ Boolean, whether or not the HQ unit will be included in the list of subordinates.
-- @return List of directia agent knowledges
integration.getEntitiesFromAutomatCommunication = function ( company, role, withHQ )
    local integration = integration
    local CreateKnowledge = CreateKnowledge
    local temp
    if withHQ then
        temp = DEC_Automate_PionsDeAutomateAvecPCCommunication( company.source )
    else
        temp = DEC_Automate_PionsDeAutomateSansPCCommunication( company.source )
    end
    local knowledges = {}
    for i = 1, #temp do
        local pion = temp[i]
        local knowledge = CreateKnowledge( integration.ontology.types.agent, pion )
        local fun = function() return knowledge:isDestroyed() end
        local ok, isDestroyed = pcall( fun )
        if ok then
            if not isDestroyed then
                knowledges[ #knowledges + 1 ] = knowledge
            end
        else
            knowledges[ #knowledges + 1 ] = knowledge
        end
    end

    if role ~= "none" and role ~= NIL and role ~= nil then
        return integration.filterPionWithRole( knowledges, role )
    else
        return knowledges
    end
 end
 
--- Deprecated
integration.getEntitiesFromBatallion = function ()
    local temp = DEC_Automate_AutomatesSubordonnes()
    local integration = integration
    local CreateKnowledge = CreateKnowledge
    
    local knowledges = {}
    for i = 1, #temp do
        local company = temp[i]
        knowledges[ #knowledges + 1 ] = CreateKnowledge( integration.ontology.types.automat, company )
    end
    return knowledges
end

--- Returns a list of all the knowledges of operational subordinates of the provided company.
-- Communication constraints may apply, according to the noCommunication parameter.
-- The returned subordinates may be filtered by their role, according to the role parameter.
-- The HQ unit may or may not be among the returned subordinates, according to the withHQ parameter.
-- All the knowledges of subordinates that define a "isDestroyed" method will not be in the resulting
-- list if this "isDestroyed" method returns true when called upon the knowledge.
-- @see integration.getEntitiesFromAutomat
-- @see integration.getEntitiesFromAutomatCommunication
-- @param company Company knowledge
-- @param role String, the name of the role to filter the subordinates with.
-- If this parameter equals "none", nil or NIL, then no filtering takes place.
-- @param withHQ Boolean, whether or not the HQ unit will be included in the list of subordinates.
-- @param noCommunication Boolean, communication between every returned subordinates and the HQ unit must be
-- guaranteed if and only if this parameter is set to false
-- @return List of directia agent knowledges
integration.getOperationnalEntitiesFromAutomat = function ( company, role, withHQ, noCommunication )
    local entities
    if not noCommunication then -- default case
        entities = integration.getEntitiesFromAutomatCommunication( company, role, withHQ )
    else
        entities = integration.getEntitiesFromAutomat( company, role, withHQ )
    end
    local operationalEntities = {}
    for i = 1, #entities do
        if entities[i].isOperational and entities[i]:isOperational() then
            operationalEntities[ #operationalEntities + 1 ] = entities[i]
        end
    end
    return operationalEntities
 end

--- Returns a list of all the agent knowledges inside the given objective (if the objective is an area
--- or an urban block), or inside a circle with its center as the center of the given objective, and its
--- radius as the provided radius (or 600 meters by default).
-- @param objective Knowledge, either an area or an urban block or a knowledge defining a "getPosition" method
-- @param radius Float, the radius (in meters, 600 by default) of the circle (in case the objective parameter is neither an area nor an urban block)
-- @return List of directia agent knowledges
integration.getDestroyableInObjective = function( objective, radius )
    local integration = integration
    local CreateKnowledge = CreateKnowledge
    local res = {}
    local enemies = {}
    if masalife.brain.core.class.isOfType( objective, integration.ontology.types.area ) then
        enemies = integration.getAgentKnowledgesInArea( objective.source ) 
    elseif masalife.brain.core.class.isOfType( objective, integration.ontology.types.urbanBlock ) then
        enemies = DEC_Connaissances_UnitesEnnemiesVivantesDansBlocUrbain( objective.source )           
    else
        enemies = integration.getKnowledgesLivingAgentsInCircle( objective:getPosition(), radius or 600 )
    end
    
    for i = 1, #enemies do
        res[ #res + 1 ] = CreateKnowledge( integration.ontology.types.agentKnowledge, enemies[i] )
    end
    return res
end

--- Returns a list of all the terrorist agent knowledges inside the given objective (if the objective is 
--- an area or an urban block), or inside a circle with its center as the center of the given objective,
--- and its radius as the provided radius (or 600 meters by default).
-- @see integration.getDestroyableInObjective
-- @param objective Knowledge, either an area or an urban block or a knowledge defining a "getPosition" method
-- @param radius Float, the radius (in meters, 600 by default) of the circle (in case the objective parameter is neither an area nor an urban block)
-- @return List of directia agent knowledges
integration.getTerroristsInObjective = function( objective )
    local integration = integration
    local CreateKnowledge = CreateKnowledge
    local res = {}
    local localisation = nil
    if masalife.brain.core.class.isOfType( objective, integration.ontology.types.area ) then
        localisation = objective.source
    elseif masalife.brain.core.class.isOfType( objective, integration.ontology.types.urbanBlock ) then
        localisation = integration.polylineUrbanBlock( objective.source )         
    else
        localisation = DEC_Geometrie_CreerLocalisationCercle( objective:getPosition(), 600 )
    end
    local units = DEC_Connaissances_UnitesDansZone( myself, localisation )
    local DEC_ConnaissanceAgent_EstTerroriste = DEC_ConnaissanceAgent_EstTerroriste
    for i = 1, #units do
        local unit = units[i]
        if DEC_ConnaissanceAgent_EstTerroriste( unit ) and DEC_ConnaissanceAgent_EnAgent( unit ) ~= myself and not DEC_ConnaissanceAgent_EstTransporte( unit ) then
            res[ #res + 1 ] = CreateKnowledge( integration.ontology.types.agentKnowledge, unit )
        end
    end
    return res
end

--- Returns the HQ unit of the given company
-- @param company Company knowledge
-- @return Directia agent
integration.getPCOfAutomate = function( company )
    return CreateKnowledge( integration.ontology.types.agent, DEC_Automate_PionPCDeAutomate( company.source ) )
end

--- Returns a list of all crossing sites in the given area
-- @param area Area knowledge
-- @return List of object knowledges (crossing sites)
integration.query.getSiteFranchissementDansZone = function( area )
    local integration = integration
    local CreateKnowledge = CreateKnowledge
    local allRes = {}
    local obstacles = {}
    obstacles = DEC_ObjectKnowledgesInZone( area.source, { eTypeObjectCrossingSite } )
    for i = 1, #obstacles do
        allRes[ #allRes + 1 ] = CreateKnowledge( integration.ontology.types.object, obstacles[i] )
    end
    return allRes
end

--- Get the list of continuous and discontinuous bridges inside an area
-- @param area. area knowledge
-- @return list of knowledges ( bridges )
integration.query.getPontoonBridgesInArea = function( area )
    local allRes = {}
    local pontoonBridges = {}
    pontoonBridges = DEC_ObjectKnowledgesInZone( area.source, { eTypeObjectContinuousPontoonBridge, eTypeObjectDiscontinuousPontoonBridge } )
    for i = 1, #pontoonBridges do
        allRes[ #allRes + 1 ] = CreateKnowledge( sword.military.world.Object, pontoonBridges[ i ] )
    end
    return allRes
end	

--- Returns an engagement point for air defense artillery against a given enemy
-- This entity must define a "getPosition" method
-- @param eni Directia agent knowledge
-- @param projectileSpeed Float, projectile speed (in m/s)
-- @return Simulation point
integration.positionInterception = function( eni, projectileSpeed )
    return DEC_Geometrie_PositionInterception( eni.source, meKnowledge:getPosition(), projectileSpeed )
end

--- Returns a list of fire positions in a given area for air defense artillery
-- @param deploymentMethod Integer, the deployment method. The possible values are :
-- <ul> <li> eDeploiementEn_carre (square formation) </li>
-- <li> eDeploiementEn_triangle (vee formation) </li>
-- <li> eDeploiementNasse_trois_sections (wedge formation (3 platoons)) </li>
-- <li> eDeploiementNasse_quatre_sections (wedge formation (4 platoons)) </li>
-- <li> eDeploiementDouble_rideau (double screen formation) </li>
-- <li> eDeploiementSimple_rideau (simple screen formation) </li> </ul>
-- @param area Area knowledge
-- @param angle Float, the firing angle (degrees, between 0 and 180)
integration.getFirePositions = function( deploymentMethod, area, angle )
    local integration = integration
    local CreateKnowledge = CreateKnowledge
    local nombrePositions = #( integration.getUnitsWithoutHQCommunication() )
    local directionEnnemi = DEC_GetDirectionEnnemi( DEC_GetRawMission( meKnowledge.source ) )
    local firePositions = {}
    if deploymentMethod == eDeploiementEn_carre then
        firePositions = DEC_Geometrie_PosDeploiementASAOmni( nombrePositions, area:getPosition(), M_POLY_DOCTRINE_DISTANCE_MAX_ENTRE_SECTIONS * 0.707 )
    elseif deploymentMethod == eDeploiementEn_triangle then
        firePositions = DEC_Geometrie_PosDeploiementASAOmni( nombrePositions, area:getPosition(), ( M_POLY_DOCTRINE_DISTANCE_MAX_ENTRE_SECTIONS / 2 ) * 1.155 )
    elseif deploymentMethod == eDeploiementNasse_trois_sections then
        firePositions = DEC_Geometrie_PosDeploiementMistralNasse( nombrePositions, area:getPosition(), angle, M_POLY_DOCTRINE_DISTANCE_POINT_A_DEFENDRE, M_POLY_DOCTRINE_DISTANCE_MAX_ENTRE_SECTIONS, directionEnnemi )
    elseif deploymentMethod == eDeploiementNasse_quatre_sections then
        firePositions = DEC_Geometrie_PosDeploiementMistralNasse( nombrePositions, area:getPosition(), angle, M_POLY_DOCTRINE_DISTANCE_POINT_A_DEFENDRE, M_POLY_DOCTRINE_DISTANCE_MAX_ENTRE_SECTIONS, directionEnnemi )
    elseif deploymentMethod == eDeploiementDouble_rideau then
        firePositions = DEC_Geometrie_PosDeploiementDoubleRideau( nombrePositions, area:getPosition(), M_POLY_DOCTRINE_DISTANCE_POINT_A_DEFENDRE, M_POLY_DOCTRINE_DISTANCE_ENTRE_RIDEAUX, M_POLY_DOCTRINE_DISTANCE_MAX_ENTRE_SECTIONS, directionEnnemi )
    elseif deploymentMethod == eDeploiementSimple_rideau then
        firePositions = DEC_Geometrie_PosDeploiementMistralNasse( nombrePositions, area:getPosition(), 180, M_POLY_DOCTRINE_DISTANCE_POINT_A_DEFENDRE, M_POLY_DOCTRINE_DISTANCE_MAX_ENTRE_SECTIONS, directionEnnemi )
    else
        firePositions = DEC_Geometrie_PosDeploiementASAOmni( nombrePositions, area:getPosition(), M_POLY_DOCTRINE_DISTANCE_MAX_ENTRE_SECTIONS * 0.707 )
    end
    local returnPositions = {}
    for i = 1, #firePositions do
        returnPositions[i] = CreateKnowledge( integration.ontology.types.point, firePositions[i] )
    end
    return returnPositions
end

--- Returns a list of dangerous enemies to neutralize when supporting the given allied units
-- @param friends List of directia agents, friendly units to support
-- @return List of directia agent knowledges
integration.query.getEnemiesToIndirectFireWhenSupport = function( friends, onlyRespondFire )
    local DEC_Connaissances_UnitesPrenantAPartieSurAmi = DEC_Connaissances_UnitesPrenantAPartieSurAmi
    local DEC_Connaissances_UnitesEnnemiesVivantesPercuesParPion = DEC_Connaissances_UnitesEnnemiesVivantesPercuesParPion
    local integration = integration
    local CreateKnowledge = CreateKnowledge
    local enemies = {}
    local simEnemies = {}
    for i = 1, #friends do
        if onlyRespondFire then
            simEnemies = DEC_Connaissances_UnitesPrenantAPartieSurAmi(friends[i].source )
        else
            simEnemies = DEC_Connaissances_UnitesEnnemiesVivantesPercuesParPion( friends[i].source )
        end
        for j = 1, #simEnemies do
            local eny = CreateKnowledge( integration.ontology.types.agentKnowledge, simEnemies[j] )
            if not eny:isTransported() and not exists( enemies, eny ) then
                enemies[ #enemies + 1 ] = eny
            end
        end
    end
    return enemies
end

--- Returns the position of this entity's current area of responsibility nearest to the objective.
-- Returns the objective barycenter if it is inside the area of responsibility
-- @param objective Knowledge (e.g. area, point...) defining a "getPosition" method returning its barycenter
-- @return Point knowledge
integration.query.getNearestPositionInAOR = function( objective )
    local target = objective:getPosition()
    local simPos = DEC_Geometrie_CalculerPointProcheLocalisationNonClippeeDansFuseau( DEC_Geometrie_ConvertirPointEnLocalisation( target ) )
    return CreateKnowledge( integration.ontology.types.point, simPos )
end

--- Returns a list of agent knowledges to support, from the given company
-- This method returns all the subordinates from the first echelon. If there are none,
-- this method returns all the subordinates instead.
-- @param company Company knowledge
-- @return List of agent knowledges
integration.query.getEntitiesToCombatSupportTask = function( company )
    local integration = integration
    local subordinates = integration.getEntitiesFromAutomatCommunication( company, "none", true )
    
    local entitiesInFirstEchelon = integration.filterPionWithEchelon( subordinates, eEtatEchelon_First )
    if #entitiesInFirstEchelon > 0 then
        return entitiesInFirstEchelon
    end
    
    -- If there are no units in the first echelon, return all the subordinates
    return subordinates
end

--- Filters the given entities by keeping only the valid and operational ones,
--- and returns the resulting list
-- @param List of directia agent knowledges, all of them defining "isValid" and "isOperational" methods
-- @return List of directia agent knowledges
integration.query.getEntitiesToCombatSupportPlatoonsTask = function( platoons )
    local platoonsToSupport = {}
    for i = 1, #platoons do
        if platoons[i]:isValid() and platoons[i]:isOperational() then -- operational and valid platoons only
            platoonsToSupport[ #platoonsToSupport + 1 ] = platoons[i]
        end
    end

    return platoonsToSupport
end

integration.getNearestElement = function( elements )
    local nearestElement = elements[ 1 ] -- init
    for _, element in pairs ( elements ) do
        if integration.distance( meKnowledge, element ) > integration.distance( meKnowledge, nearestElement ) then
            nearestElement = element
        end
    end
    return nearestElement
end

--- Convert a list of elements (companies and units) into a list of valid agents :
-- <ul> <li> Companies will be converted to the list of its valid subordinates </li>
-- <li> Agent knowledges will be converted to agents </li> </ul>
-- @param elements List of company knowledges and/or DirectIA agent knowledges.
-- Agent knowledges must define a "isValid" method returning true if the knowledge is valid,
-- false otherwise.
-- @param removeDuplicates Whether or not duplicates should be removed from the returned list
-- @return List of valid DirectIA agents
integration.getAgentFromListOfElements = function ( elements, removeDuplicates )
    local entities = {}
    for i = 1, #elements do
        if masalife.brain.core.class.isOfType( elements[i], integration.ontology.types.automat ) then -- it can be a company
            local entitiesFromAutomat = integration.getEntitiesFromAutomat( elements[i], "none", true )
            for j = 1, #entitiesFromAutomat do
                if entitiesFromAutomat[j]:isValid() and ( ( not removeDuplicates ) or ( not utilities.exists( entities, entitiesFromAutomat[j] ) ) ) then
                    entities[ #entities + 1 ] = entitiesFromAutomat[j]
                end
            end
        else -- we can support units
            if elements[i]:isValid() then
                local agentFromKnowledge = integration.getAgentFromKnowledge( elements[i] )
                if ( not removeDuplicates ) or ( not utilities.exists( entities, agentFromKnowledge ) ) then
                    entities[ #entities + 1 ] = agentFromKnowledge
                end
            end
        end
    end
    return entities
end

------------------------------------------------------------------
--- DECLARATIONS ENSURING BACKWARDS COMPATIBILITY
------------------------------------------------------------------

--- Deprecated
integration.query.getDestroyable = function( )
    local res = {}
    DEC_Connaissances_Destroyable( res )
    return res    
end

--- Deprecated, use integration.getKnowledgeHQ instead
integration.query.getPCUnit = function()
    return CreateKnowledge( integration.ontology.types.agent, integration.commanderGetHQUnit() )
end

--- Deprecated
integration.query.getPositionsToFollow = function( elementToFollow, distanceMin, distanceMax )
    return { CreateProxyKnowledge( military.world.ReachingArea, elementToFollow, 
                              { distanceMin = distanceMin, distanceMax = distanceMax } ) }
end

--- Deprecated
function integration.getAvoidingPositionsFor( entity )
    return { CreateProxyKnowledge( integration.ontology.types.avoidingArea, entity ) }
end

--- Deprecated
integration.getOperationnalEntitiesDespiteJammedFromAutomat = integration.getOperationnalEntitiesFromAutomat
