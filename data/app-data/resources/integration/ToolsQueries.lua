-------------------------------------------------------------------------------
-- Queries Predicate Tools Implementation :
-- @author DDA
-- @created 2010-04-08
-- @modified MGD 2010-04-29
--
-- This file is part of a MASA library or program.
-- Refer to the included end-user license agreement for restrictions.
--
-- Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
-------------------------------------------------------------------------------
integration.tableIsReachingFor = function( knowledges, objectives)
    local res = {}
    for _, knowledge in pairs( knowledges ) do
        for _, objective in pairs( objectives ) do
            if objective:isValid() and knowledge:isReachingFor(objective) then
                res [ #res  + 1 ] = knowledge
                break
            end
        end
    end
    return res
end

integration.tableIsDestroyingFor = function( knowledges, objectives )
    local res = {}
    for _, knowledge in pairs( knowledges ) do
        for _, objective in pairs( objectives ) do
            if objective:isValid() and knowledge:isDestroyingFor(objective) then
                res [ #res  + 1 ] = knowledge
                break
            end
        end
    end
    return res
end

integration.tableIsNeutralizingFor = function( knowledges, objectives )
    local res = {}
    for _, knowledge in pairs( knowledges ) do
        for _, objective in pairs( objectives ) do
            if objective:isValid() and knowledge:isNeutralizingFor(objective) then
                res [ #res  + 1 ] = knowledge
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

integration.query.getDestroyable = function( )
    local res = {}
    DEC_Connaissances_Destroyable( res )
    return res    
end

integration.query.getNearbyPlatoons = function( )
    local nearbyPlatoons = {}
    local destroyable = {}
    DEC_Connaissances_Destroyable( destroyable )
    local nDestroyable = #destroyable
    for i = 1, nDestroyable do
        local platoon = destroyable[i]
        if platoon:isNearby() then
            nearbyPlatoons[ #nearbyPlatoons + 1 ] = platoon
        end
    end
    return nearbyPlatoons
end


-- TODO MGD clean all fragOrder queries
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

-- -------------------------------------------------------------------------------- 
-- Return a list of n fuseau
-- @author LMT
-- @release 2010-09-23
-- --------------------------------------------------------------------------------
integration.query.getFuseaux = function( nombre )
    local fuseaux = {}
    fuseaux = DEC_DecouperFuseau( nombre )
    return fuseaux
end

-- -------------------------------------------------------------------------------- 
-- Return the number of units in PE
-- @author LMT
-- @release 2010-10-01
-- --------------------------------------------------------------------------------
integration.query.getNbrFront = function( nbrEchelon )
    if nbrEchelon == NIL or nbrEchelon == 0 then
        return #( DEC_Automate_PionsAvecPC() ) -- all
    end
    local pions = DEC_Automate_PionsMelee()
    return  math.max(1, ( #pions )/ nbrEchelon )
end

-- -------------------------------------------------------------------------------- 
-- Return the number of GEN units
-- @author LMT
-- @release 2012-01-18
-- --------------------------------------------------------------------------------
integration.query.getNbrGEN = function()
    return #( DEC_Automate_PionsGenie() ) -- all
end

-- -------------------------------------------------------------------------------- 
-- Return units without mission
-- @author LMT
-- @release 2010-11-25
-- --------------------------------------------------------------------------------
integration.query.getUnitWithoutMission = function( entities )
    local result = {}
    for _,unit in pairs( entities ) do
        if not unit:hasMission() then
            result[#result + 1] = unit
        end
    end
    return result
end


-- -------------------------------------------------------------------------------- 
-- Return trafficable positions for elements
-- @author LMT
-- @release 2010-11-05
-- --------------------------------------------------------------------------------
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

-- -------------------------------------------------------------------------------- 
-- Return nearest trafficable position for an element
-- @author MIA
-- @release 2010-11-08
-- --------------------------------------------------------------------------------
integration.query.getNearestPointFor = function( element ) -- reachable
    local positions = element:getPositions()
    local nearest = positions[ 1 ]
    local DEC_Geometrie_DistanceBetweenPoints = DEC_Geometrie_DistanceBetweenPoints
    local nPositions = #positions
    local myPosition = meKnowledge:getPosition()
    local nearestDistance = DEC_Geometrie_DistanceBetweenPoints( myPosition, nearest )
    for i = 2, nPositions do
        local position = positions[i]
        local currentDistance = DEC_Geometrie_DistanceBetweenPoints( myPosition, position )
        if currentDistance < nearestDistance then
            nearest = position
            nearestDistance = currentDistance
        end
    end
    return CreateKnowledge( integration.ontology.types.point, nearest )
end
 
  -- -------------------------------------------------------------------------------- 
-- Return PC
-- @author LMT
-- @release 2010-11-26
-- --------------------------------------------------------------------------------
integration.query.getPCUnit = function()
    return CreateKnowledge( integration.ontology.types.agent, integration.commanderGetHQUnit() )
end

integration.GetSuperiorKnowledge = function( unit ) 
    local result, automat = pcall( DEC_GetAutomate, unit.source )
    if result then
        return CreateKnowledge( integration.ontology.types.automat, automat )
    end
    return nil
end

-- -------------------------------------------------------------------------------- 
-- Param a automat
-- Return a list of platoon
-- @author LMT
-- @release 2011-01-28
-- --------------------------------------------------------------------------------
integration.getEntitiesFromAutomat = function ( automat, role, withPC)
    local integration = integration
    local CreateKnowledge = CreateKnowledge
    local temp = {}
    if withPC then
        temp = integration.getSubordinateAgentsFromCommander( automat )
    else
        temp = DEC_Automate_PionsDeAutomateSansPC(automat.source)
    end
    local knowledges = {}
    local nTemp = #temp
    for i = 1, nTemp do
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

    if role ~= "none" then --TODO replace by NIL when a queries will have nullable parameters
        return integration.filterPionWithRole( knowledges, role )
    else
        return knowledges
    end
 end

-- -------------------------------------------------------------------------------- 
-- Param a automat
-- Return a list of platoon with a good communication
-- @author LMT
-- @release 2012-07-17
-- --------------------------------------------------------------------------------
integration.getEntitiesFromAutomatCommunication = function ( automat, role, withPC )
    local integration = integration
    local CreateKnowledge = CreateKnowledge
    local temp = {}
    if withPC then
        temp = DEC_Automate_PionsDeAutomateAvecPCCommunication(automat.source)
    else
        temp = DEC_Automate_PionsDeAutomateSansPCCommunication(automat.source)
    end
    local knowledges = {}
    local nTemp = #temp
    for i = 1, nTemp do
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

    if role ~= "none" then --TODO replace by NIL when a queries will have nullable parameters
        return integration.filterPionWithRole( knowledges, role )
    else
        return knowledges
    end
 end
 
 -- -------------------------------------------------------------------------------- 
-- Param a batallion
-- Return a list of automat
-- @author LMT
-- @release 2011-12-15
-- --------------------------------------------------------------------------------
integration.getEntitiesFromBatallion = function ()
    local temp = {}
    temp = DEC_Automate_AutomatesSubordonnes()
    local integration = integration
    local CreateKnowledge = CreateKnowledge
    
    local knowledges = {}
    local nTemp = #temp
    for i = 1, nTemp do
        local automat = temp[i]
        knowledges[ #knowledges + 1 ] = CreateKnowledge( integration.ontology.types.automat, automat )
    end
    return knowledges
 end
 
integration.getOperationnalEntitiesDespiteJammedFromAutomat = function ( automat, role, withPC)
    return integration.getOperationnalEntitiesFromAutomat( automat, role, withPC)
 end

 -- -------------------------------------------------------------------------------- 
-- Param a automat
-- Return a list of operationnal platoon
-- @author LMT
-- @release 2011-10-12
-- --------------------------------------------------------------------------------
integration.getOperationnalEntitiesFromAutomat = function ( automat, role, withPC, noCommunication )
    local entities
    if not noCommunication then -- default case
        entities = integration.getEntitiesFromAutomatCommunication( automat, role, withPC )
    else
        entities = integration.getEntitiesFromAutomat( automat, role, withPC )
    end
    local operationnalEntities = {}
    for i = 1, #entities do
        if entities[i]:isOperational() then
            operationnalEntities[#operationnalEntities + 1] = entities[i]
        end
    end
    return operationnalEntities
 end

-- -------------------------------------------------------------------------------- 
-- Param an objective
-- Return a list of platoon in the objective
-- @author LMT
-- @release 2011-03-08
-- --------------------------------------------------------------------------------
integration.getDestroyableInObjective = function( objective )
    local integration = integration
    local CreateKnowledge = CreateKnowledge
    local res = {}
    local enemies = {}
    if masalife.brain.core.class.isOfType( objective, integration.ontology.types.area ) then
        enemies = integration.isKnowledgesAgentsInArea( objective.source ) 
    elseif masalife.brain.core.class.isOfType( objective, integration.ontology.types.urbanBlock ) then
        enemies = DEC_Connaissances_UnitesEnnemiesVivantesDansBlocUrbain( objective.source )           
    else
        enemies = integration.getKnowledgesLivingAgentsInCircle( objective:getPosition(), 600 )
    end
    
    local nEnemies = #enemies
    for i = 1,nEnemies do
        local enemy = enemies[i]
        res[#res + 1] = CreateKnowledge( integration.ontology.types.agentKnowledge, enemy )
    end
    return res
end

-- -------------------------------------------------------------------------------- 
-- Param an objective
-- Return a list of terrorist knowledge
-- @author DDA
-- @release 2012-03-12
-- --------------------------------------------------------------------------------
integration.getTerroristsInObjective = function( objective )
    local integration = integration
    local CreateKnowledge = CreateKnowledge
    local res = {}
    local enemies = {}
    if masalife.brain.core.class.isOfType( objective, integration.ontology.types.area ) then
        enemies = integration.isKnowledgesAgentsInArea( objective.source ) 
    elseif masalife.brain.core.class.isOfType( objective, integration.ontology.types.urbanBlock ) then
        enemies = DEC_Connaissances_UnitesEnnemiesVivantesDansBlocUrbain( objective.source )           
    else
        enemies = integration.getKnowledgesLivingAgentsInCircle( objective:getPosition(), 600 )
    end
    
    local DEC_ConnaissanceAgent_EstTerroriste = DEC_ConnaissanceAgent_EstTerroriste
    local nEnemies = #enemies
    for i = 1,nEnemies do
        local enemy = enemies[i]
        if DEC_ConnaissanceAgent_EstTerroriste( enemy ) then
            res[#res + 1] = CreateKnowledge( integration.ontology.types.agentKnowledge, enemy )
        end
    end
    return res
end
-- -------------------------------------------------------------------------------- 
-- Param an automate
-- Return PC from Automate
-- @author EVH
-- @release 2011-06-08
-- --------------------------------------------------------------------------------
integration.getPCOfAutomate = function( automate )
    return CreateKnowledge( integration.ontology.types.agent, DEC_Automate_PionPCDeAutomate( automate.source ) )
end

-- -------------------------------------------------------------------------------- 
-- Param element to follow, distancemin and distancemax
-- Return list of postions
-- @author GGE
-- @release 2011-08-29
-- --------------------------------------------------------------------------------
integration.query.getPositionsToFollow = function( elementToFollow, distanceMin, distanceMax )
    return { CreateProxyKnowledge( military.world.ReachingArea, elementToFollow, 
                              { distanceMin = distanceMin, distanceMax = distanceMax } ) }
end

integration.query.getSiteFranchissementDansZone = function ( zone )
    local integration = integration
    local CreateKnowledge = CreateKnowledge
    local allRes={}
    local obstacles = {}
    obstacles = DEC_ObjectKnowledgesInZone( zone.source, { eTypeObjectCrossingSite } )
    for _, objective in pairs( obstacles ) do
        allRes[ #allRes + 1 ] = CreateKnowledge( integration.ontology.types.object, objective )
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

-- -------------------------------------------------------------------------------- 
-- Return a postion for ASA fire
-- @author LMT
-- @release 2011-11-23
-- --------------------------------------------------------------------------------
integration.positionInterception = function( eni, vitesseProjectile )
    return DEC_Geometrie_PositionInterception( eni.source, meKnowledge:getPosition(), vitesseProjectile )
end

-- -------------------------------------------------------------------------------- 
-- Return a postion for ASA fire depends of the mode
-- @author LMT
-- @release 2011-12-05
-- --------------------------------------------------------------------------------
integration.getFirePositions = function( modeDeploiement, zone, angle )
    local integration = integration
    local CreateKnowledge = CreateKnowledge
    local nombrePositions = #( integration.getUnitsWithoutHQCommunication() )
    local directionEnnemi = DEC_GetDirectionEnnemi( DEC_GetRawMission( meKnowledge.source ) )
    local firePositions = {}
    if modeDeploiement == eDeploiementEn_carre then
        firePositions = DEC_Geometrie_PosDeploiementASAOmni( nombrePositions, zone:getPosition(), M_POLY_DOCTRINE_DISTANCE_MAX_ENTRE_SECTIONS * 0.707 )
    elseif modeDeploiement == eDeploiementEn_triangle then
        firePositions = DEC_Geometrie_PosDeploiementASAOmni( nombrePositions, zone:getPosition(), ( M_POLY_DOCTRINE_DISTANCE_MAX_ENTRE_SECTIONS / 2 ) * 1.155 )
    elseif modeDeploiement == eDeploiementNasse_trois_sections then
        firePositions = DEC_Geometrie_PosDeploiementMistralNasse( nombrePositions, zone:getPosition(), angle, M_POLY_DOCTRINE_DISTANCE_POINT_A_DEFENDRE, M_POLY_DOCTRINE_DISTANCE_MAX_ENTRE_SECTIONS, directionEnnemi )
    elseif modeDeploiement == eDeploiementNasse_quatre_sections then
        firePositions = DEC_Geometrie_PosDeploiementMistralNasse( nombrePositions, zone:getPosition(), angle, M_POLY_DOCTRINE_DISTANCE_POINT_A_DEFENDRE, M_POLY_DOCTRINE_DISTANCE_MAX_ENTRE_SECTIONS, directionEnnemi )
    elseif modeDeploiement == eDeploiementDouble_rideau then
        firePositions = DEC_Geometrie_PosDeploiementDoubleRideau( nombrePositions, zone:getPosition(), M_POLY_DOCTRINE_DISTANCE_POINT_A_DEFENDRE, M_POLY_DOCTRINE_DISTANCE_ENTRE_RIDEAUX, M_POLY_DOCTRINE_DISTANCE_MAX_ENTRE_SECTIONS, directionEnnemi )
    elseif modeDeploiement == eDeploiementSimple_rideau then
        firePositions = DEC_Geometrie_PosDeploiementMistralNasse( nombrePositions, zone:getPosition(), 180, M_POLY_DOCTRINE_DISTANCE_POINT_A_DEFENDRE, M_POLY_DOCTRINE_DISTANCE_MAX_ENTRE_SECTIONS, directionEnnemi )
    else
        firePositions = DEC_Geometrie_PosDeploiementASAOmni( nombrePositions, zone:getPosition(), M_POLY_DOCTRINE_DISTANCE_MAX_ENTRE_SECTIONS * 0.707 )
    end
    local returnPositions = {}
    for i = 1, #firePositions do
        returnPositions[i] = CreateKnowledge(integration.ontology.types.point, firePositions[i])
    end
    return returnPositions
end

-- -------------------------------------------------------------------------------- 
-- Param allied units to support
-- Return list of dangerous units for fiends
-- @author GGE
-- @release 2011-08-29
-- --------------------------------------------------------------------------------
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
            if not eny:isTransported() then
                if not exists( enemies, eny ) then
                    enemies[ #enemies + 1 ] = eny
                end
            end
        end
    end
    return enemies
end

integration.query.getNearestPositionInAOR = function( objective )
    local target = objective:getPosition()
    local simPos = DEC_Geometrie_CalculerPointProcheLocalisationNonClippeeDansFuseau( DEC_Geometrie_ConvertirPointEnLocalisation( target ) )
    return CreateKnowledge( integration.ontology.types.point, simPos )
end

-- -------------------------------------------------------------------------------- 
-- Param ally commander
-- Param old supported platoons ally list
-- Return platoons to support
-- @author GGE
-- @release 2012-10-30
-- --------------------------------------------------------------------------------
integration.query.getEntitiesToCombatSupportTask = function( commander, oldSupportedPlatoonList )
    local integration = integration
    local platoonsToSupport = {}
    -- Platoon ally in first echelon
    local entitiesInFirstEchelon = integration.filterPionWithEchelon( 
          integration.getEntitiesFromAutomatCommunication( commander, "none", true), eEtatEchelon_First )
    if next(entitiesInFirstEchelon) then
        platoonsToSupport = entitiesInFirstEchelon
    else -- no platoon in first echelon, return all unit from commander
        platoonsToSupport = integration.getEntitiesFromAutomatCommunication( commander, "none", true)
    end
    --Return new elements
    return platoonsToSupport
end

-- -------------------------------------------------------------------------------- 
-- Param ally platoons
-- Param old supported platoons ally list
-- Return platoons to support
-- --------------------------------------------------------------------------------
integration.query.getEntitiesToCombatSupportPlatoonsTask = function( platoons, oldSupportedPlatoonList )
    local platoonsToSupport = {}
    for i = 1, #platoons do
        if platoons[i]:isValid() and platoons[i]:isOperational() then -- operational and valid platoons only
            platoonsToSupport[ #platoonsToSupport + 1 ] = platoons[i]
        end
    end
    --Return new elements
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
