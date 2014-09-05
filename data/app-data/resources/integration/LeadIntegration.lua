--- Compare two elements with their efficiency
-- @param w1 First element
-- @param w2 Second element
-- @return True if the efficiency of w1 is better than w2's
-- @author MGD
-- @release 2010-12-03
local function comp( w1,w2 )
    return ( w1.efficiency > w2.efficiency )
end

-- Return the returnList without element from list
-- @param list : element to remove
-- @param returnList : list where we want to remove
-- @author LMT
-- @release 2011-01-13
removeFromListForLead = function( list, returnList )
    for _, listElement in pairs( list ) do
        for i, elemListReturn in pairs( returnList ) do
            if listElement.entity == elemListReturn then
                table.remove( returnList, i )
                break
            end
        end
    end
    return returnList
end

-- set echelon for elements in bestUnits
-- @params bestUnits : list of element
-- @params echelon
-- @author LMT
-- @release 2011-01-13
integration.setEchelon = function( bestUnits, echelon )
    local integration = integration
    for i = 1, #bestUnits do
        integration.setEchelonState( bestUnits[i].entity.source, echelon )
    end
end

-- Listen front element  in order to manage automat feedback
-- @params frontElement : realize the main effect
-- @author LMT
-- @release 2011-01-14
integration.manageFeedback = function( frontElement )
    local nbUnitInOtherAutomat = 0
    local integration = integration
    for _, elem in pairs (frontElement) do
      integration.ListenFrontElement( elem.entity )
      if integration.getCommander( elem.entity.source ) == meKnowledge then
      -- On verifie que le pion est toujours sous l'automate
          nbUnitInOtherAutomat = nbUnitInOtherAutomat + 1
      end
    end
    -- si tous les pions ont change d'automate (par exemple pour la mission preparer bascule), on remonte une fin de mission
    if nbUnitInOtherAutomat == #frontElement then
        myself.feedback = true
    end
end

-- Fill workMap liste. If no one can construct an obstacle, someone who knows the task will do it
-- @author LMT
-- @release 2010-08-26
integration.fillWorkMap = function()
    local integration = integration
    local myself = myself
    local CreateKnowledge = CreateKnowledge
    for taskName, taskWorkMap in pairs( myself.leadData.workMap ) do
        local platoonHasTask = myself.leadData.platoonHasTask[taskName]
        for obstacle, builders in pairs( taskWorkMap ) do
            if not next(builders) then
                builders = builders or {}
                for _, platoon in pairs( platoonHasTask[obstacle] ) do
                    builders[ #builders + 1 ] = platoon
                end
            end
            -- Sort builders
            for i=1, #builders-1 do
                local builderI = CreateKnowledge( integration.ontology.types.body, builders[i] )
                for j=i+1, #builders do
                    local builderJ = CreateKnowledge(integration.ontology.types.body, builders[j] )
                    if integration.distance(obstacle, builderI ) > integration.distance(obstacle, builderJ ) then
                        local temp = builders[j]
                        builders[j] = builders[i]
                        builders[i] = temp
                    end
                end
            end
        end
    end
end

--- Fill the unit task parameter
-- @param bestList Units sorted by efficiency
-- @param companyTask The company's task
-- @param params Parameters from the company's task
-- @param nbrFront The number of element that we want in this echelon
-- @author MGD
-- @release 2010-12-03
fillParameters = function( bestList, companyTask, params, nbrFront, context, isMain, objectif )
    local bestListNbrFront = {}
    local findFront = 1
    local current = 1
    params.maxNbrFront = math.min( #bestList, nbrFront )
    while findFront <= nbrFront and current <= params.maxNbrFront do
       local paramsPion = bestList[current].task:fillParameters( companyTask, params, bestList[current].entity, context, objectif, bestList[current].taskName ) -- Save the parameter needed for the platoon task
       if paramsPion then
          if isMain then          
              F_Pion_SeteEtatEchelon( bestList[current].entity.source, eEtatEchelon_First )
          end   
          bestListNbrFront[findFront] = bestList[current]
          bestListNbrFront[findFront].params = paramsPion
          findFront = findFront+1
       end
       current = current+1
    end
    return bestListNbrFront
end

-- @param entities A list of PlatoonAlly
-- @param tasks A list of Task that we want to give
-- @param companyTask The company's task
-- @param effect The effect of the company's task
-- @param params Parameters from the company's task
-- @param isMain True if tasks are for the first echelon
-- @return List of entities than can do tasks, sorted by efficiency
-- @author MGD
-- @modifier LMT
-- @release 2010-12-03
findBests = function( entities, tasks, companyTask , params, nbrFront, context, isMain, objectif )
    local bestList = {}
    local size = 1
    local bestTask = nil
    local bestTaskName = nil
    local bestParams = nil
    for _, entity in pairs( entities ) do
        local efficiencyFind = false
        local bestEfficiency = -1
        local efficiency  = -1
        local nTasks = #tasks
        for i = 1, nTasks do
            local taskName = tasks[i]
            local task = integration.RetrievePionTask( entity, taskName ) -- Save the task object if  the entity can do it
            if task then
                efficiency = entity:computePhysicalEfficiencyForEffect( task:getPionEfficiency() ) -- Efficiency for this entity to do this task
                if efficiency > bestEfficiency then
                    efficiencyFind = true
                    bestEfficiency = efficiency
                    bestTask = task
                    bestTaskName = taskName
                end
            end
        end
        if efficiencyFind then
            bestList[size] = { entity = entity,
                               task = bestTask,
                               taskName = bestTaskName,
                               efficiency = bestEfficiency }
            size = size + 1
        end
    end
    table.sort( bestList, comp ) -- Sort the list in order to have the couple entity/task with the best efficiency in first
    if isMain then
        if companyTask.distributeObjectives then
            companyTask:distributeObjectives( bestList, params )
        end
        myself.leadData.nbrWithMainTask = #bestList
    end
    return fillParameters( bestList, companyTask, params, nbrFront, context, isMain, objectif )
end

-- @param entities A list of Company
-- @param tasks A list of Task that we want to give
-- @param companyTask The batallion's task
-- @param params Parameters from the batallion's task
-- @author LMT
-- @release 2011-12-15
fillParametersAutomat = function( bestList, companyTask, params, nbrFront )
    local bestListNbrFront = {}
    local findFront = 1
    local current = 1
    params.maxNbrFront = math.min( #bestList, nbrFront )
    while findFront <= nbrFront and current <= params.maxNbrFront do
       local paramsPion = bestList[current].task:fillParameters( companyTask, params, bestList[current].entity ) -- Save the parameter needed for the platoon task
       if paramsPion then
          bestListNbrFront[findFront] = bestList[current]
          bestListNbrFront[findFront].params = paramsPion
          findFront = findFront+1
       end
       current = current+1
    end
    return bestListNbrFront
end

-- @param entities A list of Company
-- @param tasks A list of Task that we want to give
-- @param companyTask The batallion's task
-- @param params Parameters from the batallion's task
-- @author LMT
-- @release 2011-12-15
findBestsAutomat = function( entities, tasks, companyTask , params, nbrFront )
    local bestList = {}
    local size = 1
    for _, entity in pairs( entities ) do
        local nTasks = #tasks
        for i = 1, nTasks do
            local taskName = tasks[i]
            local task = integration.RetrieveAutomatInBatallionTask( entity, taskName ) -- Save the task object if  the entity can do it
            if task then
                bestList[size] = { entity = entity,
                               task = task,
                               taskName = taskName }
                size = size + 1
            end
        end
    end
    return fillParametersAutomat( bestList, companyTask, params, nbrFront )
end

-- @param entities A list of PlatoonAlly
-- @param tasks A list of Task that we want to give
-- @param companyTask The company's task
-- @param effect The effect of the company's task
-- @param params Parameters from the company's task
-- @param isMain True if tasks are for the first echelon
-- @return List of entities than can do tasks, sorted by efficiency
-- @author MGD
-- @modifier LMT
-- @release 2010-12-03
integration.findBestsGEN = function( entities, tasks, companyTask , params, nbrFront, retrogradeContext, isMain, objectif )
    local bestList = {}
    local size = 1
    local bestTask = nil
    local bestTaskName = nil
    local bestParams = nil
    local noEntityHaveMainTask = true 
    local integration = integration
    local myself = myself
    myself.leadData.nbUnitsByObstacle ={}
    myself.leadData.platoonHasTask = {}
    for _, entity in pairs( entities ) do
        local efficiencyFind = false
        local bestEfficiency = -1
        local efficiency  = -1
        for _, taskName in pairs( tasks ) do
            local task = integration.RetrievePionTask( entity, taskName ) -- Save the task object if  the entity can do it
            if task then
                noEntityHaveMainTask = false
                efficiency = entity:computePhysicalEfficiencyForEffect( task:getPionEfficiency() ) -- Efficiency for this entity to do this task
                if efficiency > bestEfficiency then
                    efficiencyFind = true
                    bestEfficiency = efficiency
                    bestTask = task
                    bestTaskName = taskName
                end
                if efficiencyFind then
                    if isMain then
                        myself.leadData.obstacles = task:getObstaclesParameter( companyTask, params )
                        myself.leadData.nbrTotalObstacle = #myself.leadData.obstacles
                        for _, obstacle in pairs ( myself.leadData.obstacles) do
                            myself.leadData.nbUnitsByObstacle[ obstacle ] = myself.leadData.nbUnitsByObstacle[ obstacle ] or 0
                            myself.leadData.workMap[taskName] = myself.leadData.workMap[taskName] or {}
                            local workMap = myself.leadData.workMap[taskName]
                            workMap[obstacle] = workMap[obstacle] or {}
                            local currentMap = workMap[obstacle]
                            local hasDotation = task:hasDotation( entity, obstacle )
                            local canDoIt = task:canDoIt( entity, obstacle )

                            myself.leadData.platoonHasTask[taskName] = myself.leadData.platoonHasTask[taskName] or {}
                            local platoonHasTask = myself.leadData.platoonHasTask[taskName]
                            platoonHasTask[obstacle] = platoonHasTask[obstacle] or {}
                            local platoons = platoonHasTask[obstacle]
                            platoons[ #platoons + 1 ] = entity.source
                            
                            if hasDotation and canDoIt then
                                currentMap[#currentMap + 1] = entity.source
                                myself.leadData.nbUnitsByObstacle[ obstacle ] = myself.leadData.nbUnitsByObstacle[ obstacle ] + 1
                            end
                        end
                     end
                     bestList[size] = { entity = entity,
                                        task = bestTask,
                                        taskName = bestTaskName,
                                        efficiency = bestEfficiency }
                     size = size + 1
                end
            end
        end
    end
    if not noEntityHaveMainTask then
        local maxNbrFront = 0
        if  next(myself.leadData.nbUnitsByObstacle) then
            for _, obstacle in pairs ( myself.leadData.obstacles ) do
                maxNbrFront =  math.max( myself.leadData.nbUnitsByObstacle[ obstacle ], maxNbrFront )
            end
        else
            maxNbrFront = nbrFront
        end
        if isMain then
            -- methode qui rempli la liste si les aucun pion n'a de dotation pour construire un obstacle donn�
            integration.fillWorkMap()
            if myself.leadData.workMap then
                params.maxNbrFront = math.min( maxNbrFront, nbrFront )
                if params.maxNbrFront~= 0 then
                    integration.optimizeWorkMap( params.maxNbrFront, myself.leadData.nbrTotalObstacle )
                end
            end
            if companyTask.distributeObjectives then
                companyTask:distributeObjectives( bestList, params )
            end
            myself.leadData.nbrWithMainTask = #bestList
        end
        return fillParameters( bestList, companyTask, params, nbrFront, retrogradeContext, isMain, objectif )
    end
    return {}
end

-- Find which unit will do the task
-- @params self
-- @params tasks : a list of platoon taks*
-- @params nbrFront : how many platoon can take this task
-- @params echelon : echelon number
-- @params isMain : whether or not the tasks are the main tasks
-- @params findBestsFunction : the function used to find the best units for the tasks
-- @params disengageTask : the name of the disengage task given to the non operational entities
-- @author LMT
-- @release 2011-01-13
integration.issueMission = function( self, tasks, nbrFront, echelon, entities, isMain, findBestsFunction, disengageTask )
    local tasks = explode( ";", tasks )
    entities = entities or self.entitiesWithoutMission
    local integration = integration
    local myself = myself
    local meKnowledge = meKnowledge
    
    if disengageTask then
        -- NMI whenever a mission is given, non operational entities should not receive the mission,
        -- and receive the Disengage mission instead (of course this doesn't apply if the mission
        -- given in the first place is Disengage, that's what the next "if" is for)
        if not exists( tasks, disengageTask ) then
            operationalEntities = {}
            nonOperationalEntities = {}
            for _, entity in pairs( entities ) do
                if exists ( self.operationnalEntities, entity ) then
                    operationalEntities[ #operationalEntities + 1 ] = entity
                else
                    nonOperationalEntities[ #nonOperationalEntities + 1 ] = entity
                end
            end
            
            if next( nonOperationalEntities ) then
                entities = operationalEntities
                integration.issueMission( self, disengageTask, 1, eEtatEchelon_None, nonOperationalEntities, false, findBestsFunction, disengageTask )
            end
        end
    end
    
    local bestUnits = findBestsFunction( entities, tasks, self.companyTask, self.parameters, nbrFront, context, isMain ) --Save the nbrFront best couple unit/tasks
    self.entitiesWithoutMission = removeFromListForLead( bestUnits, self.entitiesWithoutMission )
    integration.setEchelon( bestUnits, echelon )
    local hqUnit = integration.query.getPCUnit()
    for i = 1, #bestUnits do
        local elem = bestUnits[i]
        if not integration.isLogisticConvoy( elem.entity.source ) then
            myself.leadData.paramsGiven[ elem.entity ] = elem.params
            if elem.entity == hqUnit then
                self.hqTask = elem.taskName
            end
            meKnowledge:sendTaskToPion( elem.entity, elem.taskName, elem.params, echelon ) -- Send the task to the unit
        end
    end
    return bestUnits
end

local isHQTaskUsingRelievingUnit = function( self, hqUnit, unit, param )
    if unit == hqUnit then
        for i = 1, #self.bestUnits do
            if self.bestUnits[i].entity == param then
                return true
            end
        end
    end
    return false
end

local manageAddedAndDeletedUnits = function( self, findBestsFunction, disengageTask, restartMissionIfTacticallyDestroyed )
    local integration = integration
    local myself = myself
    local meKnowledge = meKnowledge
    local redone = false

    -- If the HQ is jammed, the company can not manage added and deleted units
    local HQunit = integration.getKnowledgeHQ()
    if ( not HQunit ) or integration.agentEstBrouille( HQunit ) then
        return
    end

    myself.leadData.dynamicEntityTasks = myself.leadData.dynamicEntityTasks or {}
    self.listenFrontElementInitialized = false
    local oldEntities = self.parameters.commandingEntities
    local newEntities = integration.getEntitiesFromAutomatCommunication( meKnowledge, "none", self.params.withPC )
    local newOperationnalEntities = integration.getOperationnalEntitiesFromAutomat( meKnowledge, "none", self.params.withPC )

    local echelons = integration.getPionsInEchelons( newEntities )
    local pionsPE = echelons[1]
    local pionsSE = echelons[2]
    local pionsEE = echelons[3]

    -- if their is a unit in the first echelon that end his mission, we considere him like a SE unit
    for _, entity in pairs( pionsPE ) do
        if not integration.hasMission(entity.source) and entity:isOperational() then
            pionsSE[#pionsSE + 1] = entity
        end
    end

    local tasksForSE = ( ( self.params.supportTasks and self.params.supportTasks ~= NIL ) and ( self.params.supportTasks..";" ) or "" ) 
                            .. self.params.defaultTask
    local hqUnit = integration.query.getPCUnit()

    -- if a unit has been removed from automaton, we dispatch again the missions
    for _, entity in pairs( oldEntities ) do
        if ( not exists( newEntities, entity ) ) and ( not integration.isLogisticConvoy( entity.source ) ) then
            if self.params.restartMissionIfDead and self.params.restartMissionIfDead ~= NIL then
                self:create()
            else
                myself.leadData.addedOrDeletedUnit = true
                local tasks = myself.leadData.dynamicEntityTasks[entity] or self.params.mainTasks -- main tasks have to be given before support tasks
                self.bestUnits = integration.issueMission ( self, self.params.mainTasks, 1, eEtatEchelon_First, pionsSE, false, findBestsFunction, disengageTask )
                for element, params in pairs (myself.leadData.paramsGiven) do
                    for _, param in pairs (params) do
                        if type(param) == "table" then
                            for _, paramElement in pairs (param) do
                                if paramElement == entity then
                                    local tasks = myself.leadData.dynamicEntityTasks[entity] or tasksForSE
                                    integration.issueMission ( self, tasks, 1, integration.getEchelonState(element.source), {element}, false, findBestsFunction, disengageTask )
                                elseif isHQTaskUsingRelievingUnit( self, hqUnit, element, paramElement ) then
                                    integration.issueMission ( self, self.hqTask, 1, integration.getEchelonState( element.source ), {element}, false, findBestsFunction, disengageTask )
                                end
                            end
                        end
                        if param == entity then
                            local tasks = myself.leadData.dynamicEntityTasks[entity] or tasksForSE
                            integration.issueMission ( self, tasks, 1, integration.getEchelonState(element.source), {element}, false, findBestsFunction, disengageTask )
                        end
                    end
                end
            end
            if meKnowledge.pionsToAwaitSource then
                meKnowledge.pionsToAwaitSource[ entity.source ] = nil
            end
            redone = true
            self.listenFrontElementInitialized = true
            integration.initializeListenFrontElement()
            break
        end
    end
    
    
    if disengageTask then
        -- Si un pion devient tactiquement detruit
        for _, entity in pairs( self.operationnalEntities ) do
            if not exists( newOperationnalEntities, entity ) then
                meKnowledge.pionsToAwaitSource[ entity.source ] = nil
                if myself.leadData.pionsLima1 and myself.leadData.pionsLima1[ entity ] then
                    myself.leadData.pionsLima1[ entity ] = nil
                elseif myself.leadData.pionsLima2 and myself.leadData.pionsLima2[ entity ] then
                    myself.leadData.pionsLima2[ entity ] = nil
                end
                if restartMissionIfTacticallyDestroyed then
                    self:create()
                else
                    integration.issueMission ( self, disengageTask, 1, eEtatEchelon_None, { entity }, false, findBestsFunction, disengageTask )
                end
            end
        end
    end

    -- Si un pion est de nouveau operationnel et/ou un pion a été ajouté à l'automate
   if #newOperationnalEntities > #self.operationnalEntities then
        for i=1, #newOperationnalEntities do
            local entity = newOperationnalEntities[i]
            if not exists( self.operationnalEntities, entity ) then
                -- Si le pion n'a pas de mission ou si c'est de la LOG on donne Se deployer; si c'est un pion Convoi, il ne faut pas le prendre en compte
                if not integration.isLogisticAutomat() and not integration.isLogisticConvoy( entity.source ) then 
                    myself.leadData.addedOrDeletedUnit = true
                    redone = true
                    local tasksForNewEntity = ""
                    if myself.taskParams.echelonNumber == 1 or self.nbrFront > #echelons[1] then
                          tasksForNewEntity = ( myself.leadData.dynamicEchelonTasks or emptyTable )[ eEtatEchelon_First ]
                                        or self.params.mainTasks..";"..( ( self.params.supportTasks and self.params.supportTasks ~= NIL )
                                                                           and ( self.params.supportTasks..";" )
                                                                           or "" ) 
                                                                     ..self.params.defaultTask
                          integration.issueMission ( self, tasksForNewEntity, 1, eEtatEchelon_First, { entity }, false, findBestsFunction, disengageTask )
                    else
                          tasksForNewEntity = ( myself.leadData.dynamicEchelonTasks or emptyTable )[ eEtatEchelon_Second ]
                                        or ( ( self.params.supportTasks and self.params.supportTasks ~= NIL ) 
                                                and ( self.params.supportTasks..";" ) 
                                                or "" ) 
                                        .. self.params.defaultTask
                          integration.issueMission ( self, tasksForNewEntity, 1, eEtatEchelon_Second, { entity }, false, findBestsFunction, disengageTask )
                    end
                end
            end
        end
    end
    if not redone then
        myself.leadData.addedOrDeletedUnit = false
    end

    self.operationnalEntities = newOperationnalEntities -- on enregistre la liste pour le tic suivant
    self.parameters.commandingEntities = newEntities
end

-- Optimize workMap list (obstacle/platoons)
-- @author LMT
-- @release 2010-08-26
integration.optimizeWorkMap = function( nbrFront, nbrTotalObstacle )
    local nombreAffectation = {}
    local nbrObstacleParBuilder = ( nbrTotalObstacle / nbrFront )
    for _, taskWorkMap in pairs( myself.leadData.workMap ) do
        local nbObstacleDistribue = 0
        for obstacle, builders in pairs( taskWorkMap ) do
            nbObstacleDistribue = nbObstacleDistribue +1
            local chosenBuilder = builders[1]
            nombreAffectation[chosenBuilder] = nombreAffectation[chosenBuilder] or 0
            nombreAffectation[chosenBuilder] = nombreAffectation[chosenBuilder] + 1
            local nextObstacle = obstacle
            nextObstacle = next(taskWorkMap, nextObstacle ) -- car on ne veut pas toucher au builders[obstacle]
            while ( nextObstacle ) do
                 local nextBuilders = taskWorkMap[ nextObstacle ]
                 local nextSize = #nextBuilders
                 if nextSize > 1 then
                     for i,currentBuilder in pairs( nextBuilders ) do
                          if currentBuilder == chosenBuilder and ( nextSize > (nbrTotalObstacle - nbObstacleDistribue) or nombreAffectation[chosenBuilder] >= nbrObstacleParBuilder) then
                             local index = i
                             while index < nextSize do
                                 nextBuilders[index] = nextBuilders[index+1]
                                 index = index + 1
                             end
                             nextBuilders[index] = nil
                             break
                         end
                     end
                 end
                 nextObstacle = next( taskWorkMap, nextObstacle )
            end
        end
    end
end

-- Manage the end of the commander mission
integration.manageEndMission = function( self )
    if myself.feedback and ( not self.companyTask.canMissionEnd or self.companyTask:canMissionEnd() ) then
        if self.params.disableWhenDone then
            self.Feedback( self.feedbacks.done )
            return
        else
            reportOnceFunction( eRC_FinMission )
        end
        local integration = integration
        for i = 1, #self.parameters.commandingEntities do
           integration.setEchelonState( self.parameters.commandingEntities[i].source, eEtatEchelon_None )
        end
        myself.feedback = false
    end
end


--- Manage dynamic tasks for Lead skills
-- @param self: The leading skill
-- @param findBestsFunction: The "find bests" method used to find the best units in integration.issueMission (for example : findBests)
-- @param disengageTask : the name of the disengage task given to the non operational entities
-- @author LMT
-- @release 2013-08-13
integration.manageDynamicTask = function(self, findBestsFunction, disengageTask)
    myself.leadData.dynamicEntityTasks = myself.leadData.dynamicEntityTasks or {}
    local integration = integration
    local myself = myself
    myself.leadData.dynamicEchelonTasks = myself.leadData.dynamicEchelonTasks or {}
    if self.companyTask.isDynamic and self.companyTask:isDynamic() then
        if self.companyTask.readyToGiveDynamicTasks then
            local dynamicEntitiesAndTasks = self.companyTask:readyToGiveDynamicTasks( self )
            if dynamicEntitiesAndTasks then
                for i=1, #dynamicEntitiesAndTasks do
                    local dynamicEntities = dynamicEntitiesAndTasks[i].entities
                    local dynamicTasks = dynamicEntitiesAndTasks[i].tasks

                    for _, entity in pairs( dynamicEntities or emptyTable ) do
                       integration.ListenFrontElement( entity )
                        if not myself.leadData.dynamicEntityTasks[entity] then
                            myself.leadData.dynamicEntityTasks[entity] = dynamicTasks
                        end
                    end
                    
                    local dynamicEchelon = dynamicEntitiesAndTasks[i].echelon 
                                             or ( dynamicEntities[ 1 ] and integration.getEchelonState( dynamicEntities[ 1 ].source ) )
                                             or eEtatEchelon_None
                    integration.issueMission ( self, dynamicTasks, #dynamicEntities, dynamicEchelon, dynamicEntities, false, findBestsFunction, disengageTask )
                    if myself.leadData.dynamicEchelonTasks[dynamicEchelon] then
                        myself.leadData.dynamicEchelonTasks[dynamicEchelon] = myself.leadData.dynamicEchelonTasks[dynamicEchelon]..";"..dynamicTasks
                    else
                        myself.leadData.dynamicEchelonTasks[dynamicEchelon] = dynamicTasks
                    end
                end
            end
        end
    end
end


-- Distribute obstacles among units 
-- @author NMI
-- @release 2013-05-15
integration.distributeObstacles = function( self )
    myself.leadData.platoonHasTask = {}
    myself.leadData.obstacles = self.companyTask:getObstaclesParameter( myself.taskParams )
    self.obstaclesLeftToAssign = copyTable( myself.leadData.obstacles )
    local obstacles = myself.leadData.obstacles
    local secondTime = false
    local thirdTime = false
    local noGEN = true
    local integration = integration
    local myself = myself
    
    -- The engineer units will build the obstacles.
    -- If they didn't build all the obstacles (for some reason), the other units will build
    -- the remaining obstacles.
    -- If there are no engineer units, the other units will build the remaining obstacles.
    
    -- self.tasks contains the tasks responsible for building the obstacles
    
    if integration.query.getNbrGEN() > 0 then
        if self.params.engineerTask == nil or self.params.engineerTask == NIL then
            self.params.engineerTask = ""
        end
        -- If there are engineer units, they will build the obstacles.
        -- The special case where there are obstacles that cannot be built by the engineer units
        -- is considered later.
        self.tasks = explode( ";", self.params.engineerTask )
        noGEN = false
    else
        -- Else, it's up to the units with the main tasks
        self.tasks = explode( ";", self.params.mainTasks )
    end
   
    -- Main loop of assignment of the obstacles

    repeat
        if secondTime then
            -- If there are obstacles that cannot be built by the engineer units,
            -- we assign the remaining obstacles to the units with the main tasks
            self.tasks = explode( ";", self.params.mainTasks )
            obstacles = self.obstaclesLeftToAssign
        end
        for _, taskName in pairs( self.tasks ) do
            for _, entity in pairs( self.operationnalEntities ) do
                local task = integration.RetrievePionTask( entity, taskName )
                if task then
                    for index, obstacle in pairs ( obstacles ) do
                        myself.leadData.workMap[taskName] =  myself.leadData.workMap[taskName] or {}
                        local workMap =  myself.leadData.workMap[taskName]
                        workMap[obstacle] = workMap[obstacle] or {}
                        local currentMap = workMap[obstacle]
                        local hasDotation = task:hasDotation( entity, obstacle )
                        local canDoIt = task:canDoIt( entity, obstacle )
                        
                        myself.leadData.platoonHasTask[taskName] = myself.leadData.platoonHasTask[taskName] or {}
                        local platoonHasTask =  myself.leadData.platoonHasTask[taskName]
                        platoonHasTask[obstacle] = platoonHasTask[obstacle] or {}
                        local platoons = platoonHasTask[obstacle]
                        platoons[ #platoons + 1 ] = entity.source
                        
                        if hasDotation and canDoIt then
                            currentMap[#currentMap + 1] = entity.source
                            self.obstaclesLeftToAssign[ index ] = nil
                        end
                    end
                end
            end
        thirdTime = secondTime
        secondTime = true
        end
    -- If there are still units to be built, we'll assign the obstacles to the other units.
    -- That is what the repeat loop is for, there is no other reason to repeat otherwise.
    until noGEN or thirdTime or #self.obstaclesLeftToAssign == 0
       
    -- Filling the Work Map if no unit has dotations
    integration.fillWorkMap()
    
    -- Optimizing the Work Map
    if myself.leadData.workMap then
        if self.nbrFront~= 0 then
            integration.optimizeWorkMap( self.nbrFront, #myself.leadData.obstacles )
        end
    end
end



-- Returns an allocation of elements to be assigned to other elements to assign to, with respect to
-- a criterion to be defined (for example, distance) and under certain additional constraints
-- (for example, dotations).
-- Please note that for each element in elementsToAssignTo, only one element from elementsToBeAssigned
-- will be allocated, but for each element in elementsToBeAssigned, it is possible to have multiple elements
-- from elementsToAssignTo allocated. Usually, elementsToAssignTo is larger than elementsToBeAssigned.
-- For example, if one wants to allocate positions to subordinates, so that every subordinate can have only
-- one position, and positions can be shared by multiple subordinates, then this function can be called with
-- elementsToAssignTo being the list of subordinates and elementsToBeAssigned being the list of positions.
-- Please note that in this type of situation (subordinates being given parameters of a company mission),
-- calling the function with the list of subordinates in elementsToBeAssigned instead of elementsToAssignTo
-- can be pertinent.
-- For example, if one wants to allocate urban blocks to subordinates so that the subordinates can have
-- multiple urban blocks assigned but the urban block can not have multiple subordinates assigned to them, then
-- this function can be called with elementsToAssignTo being the list of urban blocks and elementsToBeAssigned
-- the list of subordinates.
-- Please also note that the algorithm implemented in this function is obviously NOT optimal and can even in
-- extremely rare cases (i.e. not obtained in SWORD) fail to deal correctly with constraint satisfaction.
-- Resource allocation and automatic scheduling are indeed hard problems needing complex algorithms to be
-- solved optimally.
-- @param elementsToAssignTo : The elements to assign to
-- @param elementsToBeAssigned : The elements to be assigned
-- @param maximization : Optional, defines whether the function to optimize must be maximized or minimized.
-- It is minimized by default.
-- @param functionToOptimize : Optional, defines the criterion used to decide which elements to be assigned are
-- best for a given element. Returns a numeric value that is then compared to previously obtained values.
-- By default, returns the distance between the two elements.
-- @param additionalConstraints : Optional, a function that defines additional constraints for an assignment
-- between two elements to be possible. By default, there is no constraint.
-- @author NMI
-- @release 2013-08-30
integration.allocateElements = function( elementsToAssignTo, elementsToBeAssigned, maximization, functionToOptimize, additionalConstraints )
    if not functionToOptimize then
        functionToOptimize = function(a,b) return integration.computeDistance( a:getPosition(), b:getPosition() ) end
    end
    
    if not additionalConstraints then
        additionalConstraints = function(a,b) return true end
    end
    
    -- A copy is made of elementsToBeAssigned because the copy will be modified
    -- and the parameter need to remain intact for it may be copied again
    local elementsToBeAssignedCopy = copyTable( elementsToBeAssigned )
    
    -- The table containing the final allocation.
    -- It will be returned at the end of the function.
    local allocation = {}
    
    -- For each element, find the closest objective
    for i=1, #elementsToAssignTo do
        -- bestValue will keep track of the value returned by the
        -- function to optimize when called with the current best element
        local bestValue = maximization and -math.huge or math.huge
        local currentValue
        local bestElement
        local bestIndex
        
        -- If the copy of elementsToBeAssigned has been completely emptied, then
        -- copy the original parameter again
        if #elementsToBeAssignedCopy == 0 then
            elementsToBeAssignedCopy = copyTable( elementsToBeAssigned )
        end
        
        for j=1, #elementsToBeAssignedCopy do
            if additionalConstraints( elementsToAssignTo[i], elementsToBeAssignedCopy[j] ) then
                -- For each element to be assigned, compute the function to optimize
                currentValue = functionToOptimize( elementsToAssignTo[i], elementsToBeAssignedCopy[j] )
                -- If the current value beats the bestValue, then the bestElement changes.
                -- This test depends on whether we are minimizing or maximizing the function to optimize.
                if maximisation and currentValue > bestValue or currentValue < bestValue then
                    bestValue = currentValue
                    bestElement = elementsToBeAssignedCopy[ j ]
                    bestIndex = j
                end
            end
        end
        
        -- If an element respecting the additional constraints was found, then...
        if bestElement then
            -- The current entity is assigned the best objective
            allocation[ elementsToAssignTo[i] ] = bestElement
            
            -- The assigned objective is removed from elementsToBeAssignedCopy
            -- to make sure that the objectives are fairly allocated.
            table.remove( elementsToBeAssignedCopy, bestIndex )
        end
    end
    
    return allocation
end

--- Generic create for Lead skills
-- @param self: The leading skill
-- @param functionsToExecute: A table of potential functions to execute if needed (with self as the only parameter)
-- @param findBestsFunction: The "find bests" method used to find the best units in integration.issueMission (for example : findBests)
-- @param disengageTask : The name of the disengage task given to the non operational entities
-- @param givePCTask : Whether or not the PC task should be issued
-- @param giveEngineerTask : Whether or not the engineer task should be issued
-- @param giveMainTask : Whether or not the main task should be issued
-- @param giveSupportTask : Whether or not the support task should be issued
-- @param givePEITask : Whether or not the PEI task should be issued
-- @param giveDefaultTask : Whether or not the default task should be issued
-- @author NMI
-- @release 2013-07-05
integration.leadCreate = function( self, functionsToExecute, findBestsFunction, disengageTask,
                                    givePCTask, giveEngineerTask, giveMainTask, giveSupportTask, givePEITask, giveDefaultTask )
    local integration = integration
    local myself = myself
    integration.initializeListenFrontElement()
    myself.newTask = false
    self.parameters = myself.taskParams
    self.parameters.commandingEntities = integration.getEntitiesFromAutomatCommunication( meKnowledge, "none", self.params.withPC )
    self.operationnalEntities = integration.getOperationnalEntitiesFromAutomat( meKnowledge, "none", self.params.withPC )
    if #self.operationnalEntities == 0 then
        self.Feedback( self.feedbacks.done )
        return
    end
    for _, pion in pairs( self.parameters.commandingEntities ) do
        integration.setNeedReinforcement( pion, nil, nil, false )
    end
    -- Init
    local Activate = Activate
    if self.params.additionalStage then
        myself.leadData = myself.leadData or {} 
    else
        myself.leadData = {}
    end
    myself.leadData.paramsGiven = {}
    myself.leadData.workMap = {}
    myself.leadData.tasksGiven = {}
    myself.leadData.finishedSubordinates = {}
    myself.leadData.taskError = false
    self.bestUnits = {}
    self.entitiesWithoutMission = copyTable( self.parameters.commandingEntities )
    self.initialSupportingUnits = {}
    myself.feedback = false
    self.companyTask = integration.RetrieveAutomateTask( meKnowledge, self.params.companyTask )
    if self.params.mainTasks then
        meKnowledge.nbPionsMain = integration.nbPlatoonsHaveTask( self.parameters.commandingEntities, self.params.mainTasks, true )
    end
    self.companyTask:init( self.params, self.parameters )
    
    meKnowledge.pionsToAwait = {}
    meKnowledge.pionsToAwaitSource = {}
    self.sendSupport = true
    self.screenPosition  = false
    
    if myself.leadData.taskError then -- Impossible for automat to give tasks because one mandatory parameter for units cannot be given 
       self.Feedback( self.feedbacks.done )
       return
    end

    if self.params.everyoneMustBeAble and self.params.everyoneMustBeAble ~= NIL then
        local mainTasks = explode( ";", self.params.mainTasks )
        for i = 1, #self.parameters.commandingEntities do
            for j = 1, #mainTasks do
                if not integration.RetrievePionTask( self.parameters.commandingEntities[i], mainTasks[j] ) then
                    reportOnceFunction( eRC_EveryoneMustBeAble )
                    myself.feedback = true
                end
            end
        end
    end
    
    if self.params.forbidFlyingAgentsFromLanding == true then
        self.entitiesToAllowToLand = {}
        for i = 1, #self.parameters.commandingEntities do
            integration.SendMessage( "DeactivateBehaviour", self.parameters.commandingEntities[i], { behaviour = "landWhenStationary" }, { type = "dynamic" } )
            self.entitiesToAllowToLand[i] = self.parameters.commandingEntities[i]
        end
    end

    if self.params.allInFront and self.params.allInFront ~= NIL then
        self.nbrFront = #self.parameters.commandingEntities -- All platoons in front
    else
        self.nbrFront = self.companyTask:getNbrFront() -- Ask how many platoon will be in the front group. Number giving by the user or by default.
    end
    self.progressionInAOR = {}

    for _, unit in pairs( self.parameters.commandingEntities ) do
      unit:setHasMission(false)
    end

    -- Execute the provided functions if any
    for _,func in pairs( functionsToExecute or emptyTable ) do
        func( self )
    end

    -- Initialisation des missions pion
    -- Le pion PC rejoint le meetingPoint
    if givePCTask and self.params.pcTasks and self.params.pcTasks ~= NIL then
        self.parameters.pcObjective = self.params.pcObjective
        local HQUnit = integration.query.getPCUnit()
        if HQUnit then
            integration.issueMission ( self, self.params.pcTasks, 1, eEtatEchelon_Reserve, { HQUnit }, false, findBestsFunction, disengageTask )
        end
    end

    -- S'il y a des obstacles � construire, les pions GEN (ou PIA) vont le construire
    if giveEngineerTask and self.params.engineerTask and self.params.engineerTask ~= NIL then
        integration.issueMission ( self, self.params.engineerTask,  #self.companyTask:getObstaclesPlan( myself.taskParams ), eEtatEchelon_None, nil, false, findBestsFunction, disengageTask )
    end

    -- Le premier echelon recoit les missions principales ("mainTasks")
    if giveMainTask then
        local bestUnits = integration.issueMission ( self, self.params.mainTasks, self.nbrFront, eEtatEchelon_First, nil, true, findBestsFunction, disengageTask )
        if not self.params.continueIfNoMainTask and #bestUnits == 0 then
            reportOnceFunction( eRC_NoPEInAutomat )
            myself.feedback = true
            return
        end
        integration.manageFeedback( bestUnits )
        
        self.bestUnits = copyTable( bestUnits )
    end

    -- Le second echelon recoit les missions de "supportTask"
    if giveSupportTask and self.params.supportTasks and self.params.supportTasks ~= NIL then
        local bestUnits = integration.issueMission ( self, self.params.supportTasks, #self.entitiesWithoutMission, eEtatEchelon_Second, nil, false, findBestsFunction, disengageTask )
        for i = 1, #bestUnits do
            self.initialSupportingUnits[i] = bestUnits[i].entity
        end
    end

    -- Les pions PEI
    if givePEITask and self.params.peiTasks and self.params.peiTasks ~= NIL then
        integration.issueMission ( self, self.params.peiTasks, self.nbrFront, eEtatEchelon_Scout, nil, false, findBestsFunction, disengageTask )
    end

    -- Ceux qui n'ont toujours pas de mission recoivent la mission par d�faut
    if giveDefaultTask then
        integration.issueMission ( self, self.params.defaultTask, #self.entitiesWithoutMission, eEtatEchelon_Reserve, nil, false, findBestsFunction, disengageTask )
    end
    
    -- Save initial positions
    myself.leadData.initialPositions = utilities.createTableWithSimIndexes()
    local subordinatesWithoutHQ = integration.getEntitiesFromAutomatCommunication( meKnowledge, "none", false )
    for i=1, #subordinatesWithoutHQ do
        myself.leadData.initialPositions[ subordinatesWithoutHQ[i].source ] = CreateKnowledge( integration.ontology.types.point, integration.getTeammatePosition( subordinatesWithoutHQ[i] ) )
    end
end

--- Generic activate for Lead skills
-- @param self The leading skill
-- @param findBestsFunction The "find bests" method used to find the best units in integration.issueMission (for example : findBests)
-- @param manageReinforcement Whether or not the skill should manage reinforcement if one of the subordinate can't perform work on an object (because of dotation or physical restriction).
-- @param removeSupportingUnits Whether or not the initial supporting units will be removed from the list of second echelon units given to the Support Manager
-- @author NMI
-- @release 2013-07-05
integration.leadActivate = function( self, findBestsFunction, manageReinforcement, removeSupportingUnits )
    local integration = integration
    local myself = myself

    if myself.newTask then
      self:create()
    end

    if #self.operationnalEntities == 0 then
        reportOnceFunction( eRC_MissionImpossibleUnitesSubordonneesNonOperationnelles )
        self.Feedback( self.feedbacks.done )
        return
    end
    
    if self.listenFrontElementInitialized then -- if a subordinate destroyed before and tasks issued a new time
        for _, elem in pairs( self.bestUnits or emptyTable ) do
            integration.ListenFrontElement( elem.entity )
        end
        self.listenFrontElementInitialized = false
    end

    if self.params.manageAddedAndDeletedUnits ~= false then
        manageAddedAndDeletedUnits( self, findBestsFunction )
    end

    local Activate = Activate
    
    -- Dynamicity managing
    integration.manageDynamicTask( self, findBestsFunction )
    
    -- Communication between the company and the subordinates
    if self.companyTask.communicateWithSubordinates then
        self.companyTask:communicateWithSubordinates()
    end
    local echelons = integration.getPionsInEchelons( self.parameters.commandingEntities )
    local pionsPE = echelons[1]
    local pionsSE = echelons[2]
    local pionsEE = echelons[3]

    -- Gestion du CR quand les pions PE arrivent sur les limas de type LC ou LD
    Activate( self.skill.links.synchronizeRC, 1, { entities = pionsPE } )

    integration.manageEndMission( self )

    -- Gestion du soutien
    if self.params.taskForSupporting and self.params.taskForSupporting ~= NIL then
        if not self.supportingUnits then
            self.supportingUnits = removeSupportingUnits
                                    and utilities.removeListFromList( self.initialSupportingUnits, utilities.copyTable( pionsSE ) )
                                    or pionsSE
        end
        Activate( self.skill.links.supportManager, 1, { companyTask = self.companyTask, parameters = self.parameters, PE = pionsPE, SE = self.supportingUnits, taskForSupporting = self.params.taskForSupporting })
    end

    -- Reinforcement management
    if manageReinforcement and self.params.taskForReinforcement ~= NIL then
        Activate( self.skill.links.manageReinforcement, 1, { reinforcingUnits = pionsSE, reinforcementMission = self.params.taskForReinforcement } )
    end

    if self.params.relieveManager then
        -- Gestion de la relève
        Activate( self.skill.links.relieveManager, 1, { pions = pionsPE, releve = pionsSE})
    end

    if not self.params.noCoordination then
        local fuseau = meKnowledge:getAreaOfResponsibility()

        -- Gestion de l'elongation   
        -- tous les pions sont projete sur un des deux fuseaux de l'automate
        -- self.progressionInAOR: contient les valeurs de progression sur l'axe de progression
        -- plus la valeur est grande, plus le pion est en avant dans le fuseau
        -- Effet de bord indesirable : en cas d'aller retour dans le fuseau la coordination est compl鴥ment inutilisable 
        local proj
        for _, entity in pairs( self.parameters.commandingEntities ) do
            proj = integration.advanceAlongAOR( entity.source ) 
            self.progressionInAOR[ entity ] = proj
        end

        -- Gestion de l'elongation intra echelon
        local largeurFuseau = fuseau:getWidth()
        Activate( self.skill.links.coordinationManager , 1, { enititesFromEchelon = pionsPE, progressionInAOR = self.progressionInAOR, distance = largeurFuseau/2 } )
        Activate( self.skill.links.coordinationManager , 1, { enititesFromEchelon = pionsEE, progressionInAOR = self.progressionInAOR, distance = largeurFuseau/2 } )

        -- Management of support coordination.
        -- The 'meKnowledge.subordinatesToSlowDown' table contains subordinates that need to wait their support.
        for subordinate in pairs( meKnowledge.subordinatesToSlowDown or emptyTable ) do 
            local sub = CreateKnowledge( integration.ontology.types.agent, subordinate )
            Activate( self.skill.links.issueSlowDownOrder , 1, { subordinate = sub } )
        end
    end
    
    if not self.params.continueIfNoMainTask and not next(integration.getPionsInEchelons( self.parameters.commandingEntities )[1]) then
        reportOnceFunction( eRC_NoPEInAutomat )
        myself.feedback = true -- terminating the mission if there is no operational unit in the first echelon
    end
end

--- Specific activate method for LeadDelay skill
-- @param self: The leading skill
-- @author NMI
-- @release 2013-07-05
integration.leadDelayActivate = function( self, disengageTask )  
    local integration = integration
    local myself = myself
    local meKnowledge = meKnowledge
    local Activate = Activate

    if #self.operationnalEntities == 0 then
        reportOnceFunction( eRC_MissionImpossibleUnitesSubordonneesNonOperationnelles )
        self.Feedback( self.feedbacks.done )
        return
    end

    manageAddedAndDeletedUnits( self, findBests, disengageTask, true )
    
    -- Mis à jour des echelons
    integration.setPionsEchelons( myself.leadData.pionsLima1, eEtatEchelon_First )
    integration.setPionsEchelons( myself.leadData.pionsLima2, eEtatEchelon_Second )
      
    -- Changement d'echelons lorsque l'ordre de conduite Decrocher a ete donne et que tous le deuxieme echelon est en place, pret a appuyer
    if self.decrocher and not next( meKnowledge.pionsToAwaitSource ) then
        if next(myself.leadData.pionsLima2) ~= nil then
            self.pionsLimaTmp = copyTable( myself.leadData.pionsLima1 )
            myself.leadData.pionsLima1 = copyTable( myself.leadData.pionsLima2 )
            myself.leadData.pionsLima2 = copyTable( self.pionsLimaTmp )
        end
        self.sendSupport = true
        self.decrocher = false
        myself.screenUnitDisengage = nil
        return 
    end

    -- L'automate donne l'ordre de conduite Decrocher à tout le premier echelon si au moins 1 pion du premier échelon est en danger
    -- Decrocher comporte l'embarquement du premier echelon et le débarquement à l'arrivée
    if self.params.performBlockingActions then
        self.orderUnitsToDisengage = meKnowledge:delayingSubordinateIsNotSafe( myself.leadData.pionsLima1 )
    else
        self.orderUnitsToDisengage = myself.screenUnitDisengage and #myself.screenUnitDisengage > 0
    end
    meKnowledge.arrivedUnits = meKnowledge.arrivedUnits or {}
    
    local pionsLima1Entities = {}
    for _, entity in pairs( myself.leadData.pionsLima1 ) do
        pionsLima1Entities[ #pionsLima1Entities + 1 ] = entity
    end
    
    if ( not self.decrocher and self.orderUnitsToDisengage ) 
        and meKnowledge.arrivedUnits
        and #meKnowledge.arrivedUnits >= tableSize( myself.leadData.pionsLima1 ) then
        local fragOrder = integration.createFragOrder("Disengage")
        local fragOrderKn = CreateKnowledge( integration.ontology.types.fragOrder, fragOrder )

        meKnowledge.pionsToAwait = copyTable( myself.leadData.pionsLima1 )

        -- Keep only the operational units
        for index, entity in pairs( meKnowledge.pionsToAwait ) do
            if not exists( self.operationnalEntities, entity ) then
                meKnowledge.pionsToAwait[ index ] = nil
            end
        end
        for k,v in pairs( meKnowledge.arrivedUnits ) do 
            meKnowledge.arrivedUnits[ k ] = nil
        end

        meKnowledge.pionsToAwaitSource = {}
        for k,v in pairs( meKnowledge.pionsToAwait ) do
            meKnowledge.pionsToAwaitSource[k.source] = v.source
        end
        
        Activate( self.skill.links.manageFragOrder, 1, { fragOrders = { fragOrderKn } , entities = pionsLima1Entities } )
        self.decrocher = true
        self.screenPosition = false
        myself.screenUnitDisengage = nil
    end
    local echelons = integration.getPionsInEchelons( self.parameters.commandingEntities )

    -- Gestion du CR quand les pions PE arrivent sur les limas de type LC ou LD
    Activate( self.skill.links.synchronizeRC, 1, { entities = echelons[1] } )

    -- Gestion de l'appui: les pions d'appui, appuient toujours le premier échelon  
    local tableObjectives = {}
    if not next( tableObjectives ) then
        tableObjectives = echelons[1]
    else
        for _, entity in pairs( myself.leadData.pionsLima1 ) do
            tableObjectives[ #tableObjectives + 1 ] = entity
        end
    end

    if self.sendSupport then 
        local tasksForSupporting = explode( ";", self.params.taskForSupporting )
        for _, supportingEntity in pairs( echelons[4] ) do
            for _, taskForSupporting in pairs( tasksForSupporting ) do
                if integration.RetrievePionTask( supportingEntity, taskForSupporting ) then
                    meKnowledge:sendTaskToPion( supportingEntity, taskForSupporting , { objectives = tableObjectives, retrogradeContext = true } , eEtatEchelon_Reserve )  
                end
            end
        end
        self.sendSupport = false
    end

    -- Coordination intra echelon
    -- tous les pions sont projeté sur un des deux fuseaux de l'automate
    -- self.progressionInAOR: contient les valeurs de progression sur l'axe de progression
    -- plus la valeur est grande, plus le pion est en avant dans le fuseau
    local proj
    for _, entity in pairs( self.parameters.commandingEntities ) do
        proj = integration.advanceAlongAOR( entity.source ) 
        self.progressionInAOR[ entity ] = proj
    end

    local fuseau = meKnowledge:getAreaOfResponsibility()
    local largeurFuseau = fuseau:getWidth()

    Activate( self.skill.links.coordinationManager , 1, { enititesFromEchelon = pionsLima1Entities, progressionInAOR = self.progressionInAOR, distance = largeurFuseau/2 } )

    integration.manageEndMission( self )

    if not next(integration.getPionsInEchelons( self.parameters.commandingEntities )[1]) then
        reportOnceFunction( eRC_NoPEInAutomat )
    end
end

--- Specific activate method for LeadDrone skill
-- @param self: The leading skill
-- @author NMI
-- @release 2013-07-05
integration.leadDroneActivate = function( self, findBestsFunction )
    local integration = integration
    
    if myself.newTask then
      self:create()
    end

    if #self.operationnalEntities == 0 then
        reportOnceFunction( eRC_MissionImpossibleUnitesSubordonneesNonOperationnelles )
        self.Feedback( self.feedbacks.done )
        return
    end
    local echelons = integration.getPionsInEchelons( self.parameters.commandingEntities )
    local pionsPE = echelons[1]
    local pionsSE = echelons[2]
    local pionsEE = echelons[3]
    
    if meKnowledge.availableDrone then
        local drones = { meKnowledge.availableDrone }
        -- Le premier echelon recoit les missions principales ("mainTasks")
        local bestUnits = integration.issueMission ( self, self.params.droneTasks, 1, eEtatEchelon_First, drones, false, findBests, false)
        for _, elem in pairs ( drones ) do
              integration.ListenFrontElement( elem )
        end
        meKnowledge.availableDrone = nil
    end

    -- Dynamicity managing
    integration.manageDynamicTask( self, findBestsFunction, disengageTask )
    
    integration.manageEndMission( self )
end

--- Generic destroy for Lead skills
-- @param self: The leading skill
-- @param setEchelonNone: Whether or not the echelon state of the commanding entities should be set at "none"
-- @author NMI
-- @release 2013-07-05
integration.leadDestroy = function ( self, setEchelonNone )
    local integration = integration
    if self.companyTask and self.companyTask.destroy then
        self.companyTask:destroy( self.params, self.parameters )
    end

    if setEchelonNone then
        for i = 1, #self.parameters.commandingEntities do
           integration.setEchelonState( self.parameters.commandingEntities[i].source, eEtatEchelon_None )
        end
    end
    self.parameters.pcObjective = nil
    
    if self.params.forbidFlyingAgentsFromLanding == true and self.entitiesToAllowToLand then
        for i = 1, #self.entitiesToAllowToLand do
            integration.SendMessage( "ReactivateBehaviour", self.entitiesToAllowToLand[i], { behaviour = "landWhenStationary" }, { type = "dynamic" } )
        end
    end
end
