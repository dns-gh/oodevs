--- Compare two elements with their efficiency
-- @param w1 First element
-- @param w2 Second element
-- @return True if the efficiency of w1 is better than w2's
-- @author MGD
-- @release 2010-12-03
local function comp( w1,w2 )
    return ( w1.efficiency > w2.efficiency )
end

-- set echelon for elements in bestUnits
-- @params bestUnits : list of element
-- @params echelon
-- @author LMT
-- @release 2011-01-13
integration.setEchelon = function( bestUnits, echelon )
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
    for taskName, taskWorkMap in pairs( myself.leadData.workMap ) do
        local platoonHasTask = myself.leadData.platoonHasTask[taskName]
        for obstacle, builders in pairs( taskWorkMap ) do
            if not next(builders) then
                builders = builders or {}
                for _, platoon in pairs( platoonHasTask[obstacle] ) do
                    builders[ #builders + 1 ] = platoon
                end
            end
            -- Trier builders
            for i=1, #builders-1 do
                local builderI = CreateKnowledge(integration.ontology.types.body, builders[i] )
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
integration.findBests = function( entities, tasks, companyTask , params, nbrFront, context, isMain, objectif )
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
        myself.leadData.nbrWithMainTask = #bestList
    end
    return fillParameters( bestList, companyTask, params, nbrFront, context, isMain, objectif )
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
-- @author LMT
-- @release 2011-01-13
integration.issueMission = function( self, tasks, nbrFront, echelon, entities, isMain, findBestsFunction, disengageWhenTacticallyDestroyed )
    local tasks = explode( ";", tasks )
    entities = entities or self.entitiesWithoutMission
    
    if disengageWhenTacticallyDestroyed then
        -- NMI whenever a mission is given, non operational entities should not receive the mission,
        -- and receive the Disengage mission instead (of course this doesn't apply if the mission
        -- given in the first place is Disengage, that's what the next "if" is for)
        if not exists( tasks, "worldwide.agent.tasks.Disengage" ) then
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
                integration.issueMission( self, "worldwide.agent.tasks.Disengage", 1, eEtatEchelon_None, nonOperationalEntities, false, findBestsFunction, disengageWhenTacticallyDestroyed )
            end
        end
    end
    
    local bestUnits = findBestsFunction( entities, tasks, self.companyTask, self.parameters, nbrFront, context, isMain ) --Save the nbrFront best couple unit/task
    self.entitiesWithoutMission = removeFromListForLead( bestUnits, self.entitiesWithoutMission )
    integration.setEchelon( bestUnits, echelon )
    local nBestUnits = #bestUnits
    for i = 1, nBestUnits do
        local elem = bestUnits[i]
        if not integration.isLogisticConvoy( elem.entity.source ) then
            meKnowledge:sendTaskToPion( elem.entity, elem.taskName, elem.params, echelon ) -- Send the task to the unit
        end
    end
    return bestUnits
end

integration.manageAddedAndDeletedUnits = function( self, findBestsFunction, disengageWhenTacticallyDestroyed )
    local redone = false
    self.listenFrontElementInitialized = false
    local oldEntities = self.parameters.commandingEntities
    local newEntities = integration.getEntitiesFromAutomatCommunication( meKnowledge, "none", self.params.withPC )
    local newOperationnalEntities = integration.getOperationnalEntitiesFromAutomat( meKnowledge, "none", self.params.withPC )

    -- Si un pion a été supprimé de l'automate, on redistribue les missions
    for i, entity in pairs( oldEntities ) do
        if ( not exists( newEntities, entity ) ) and ( not integration.isLogisticConvoy( entity.source ) ) then
            if meKnowledge.pionsToAwaitSource then
                meKnowledge.pionsToAwaitSource[ entity.source ] = nil
            end
            self:create()
            redone = true
            self.listenFrontElementInitialized = true
            integration.initializeListenFrontElement()
            break
        end
    end
    
    
    if disengageWhenTacticallyDestroyed then
        -- Si un pion devient tactiquement detruit
        for _, entity in pairs( self.operationnalEntities ) do
            if not exists( newOperationnalEntities, entity ) then
                meKnowledge.pionsToAwaitSource[ entity.source ] = nil
                if myself.leadData.pionsLima1 and myself.leadData.pionsLima1[ entity ] then
                    myself.leadData.pionsLima1[ entity ] = nil
                elseif myself.leadData.pionsLima2 and myself.leadData.pionsLima2[ entity ] then
                    myself.leadData.pionsLima2[ entity ] = nil
                end
                giveMission ( self, "worldwide.agent.tasks.Disengage", 1, eEtatEchelon_None, { entity })
            end
        end
    end

    -- Si un pion est de nouveau operationnel et/ou un pion a été ajouté à l'automate
   if #newOperationnalEntities > #self.operationnalEntities and not redone then
        for i, entity in pairs( newOperationnalEntities ) do
            if not exists( self.operationnalEntities, entity ) then
                -- Si le pion n'a pas de mission ou si c'est de la LOG on donne Se deployer; si c'est un pion Convoi, il ne faut pas le prendre en compte
                if not integration.isLogisticAutomat() and not integration.isLogisticConvoy( entity.source ) then 
                    local tasksForNewEntity = ""
                    if myself.taskParams.echelonNumber == 1 then
                          tasksForNewEntity = self.params.mainTasks..";"..self.params.supportTasks..";"..self.params.defaultTask
                          integration.issueMission ( self, tasksForNewEntity, 1, eEtatEchelon_First, { entity }, false, findBestsFunction, disengageWhenTacticallyDestroyed )
                    else
                          tasksForNewEntity = self.params.supportTasks..";"..self.params.defaultTask
                          integration.issueMission ( self, tasksForNewEntity, 1, eEtatEchelon_Second, { entity }, findBestsFunction, disengageWhenTacticallyDestroyed )
                    end
                end
            end
        end
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
    if myself.feedback then
        if self.params.disableWhenDone then
            self.Feedback( self.feedbacks.done )
            return
        else
           meKnowledge:RC( eRC_FinMission )
        end
        for i = 1, #self.parameters.commandingEntities do
           integration.setEchelonState( self.parameters.commandingEntities[i].source, eEtatEchelon_None )
        end
        myself.feedback = false
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


--- Generic create for Lead skills
-- @param self: The leading skill
-- @param functionsToExecute: A table of potential functions to execute if needed (with self as the only parameter)
-- @param findBestsFunction: The "find bests" method used to find the best units in integration.issueMission (for example : integration.findBests)
-- @param disengageWhenTacticallyDestroyed : Whether or not the units should be issued the Disengage mission when tactically destroyed
-- @param givePCTask : Whether or not the PC task should be issued
-- @param giveEngineerTask : Whether or not the engineer task should be issued
-- @param giveMainTask : Whether or not the main task should be issued
-- @param giveSupportTask : Whether or not the support task should be issued
-- @param givePEITask : Whether or not the PEI task should be issued
-- @param giveDefaultTask : Whether or not the default task should be issued
-- @author NMI
-- @release 2013-07-05
integration.leadCreate = function( self, functionsToExecute, findBestsFunction, disengageWhenTacticallyDestroyed,
                                    givePCTask, giveEngineerTask, giveMainTask, giveSupportTask, givePEITask, giveDefaultTask )
    integration.initializeListenFrontElement()
    myself.newTask = false
    self.parameters = myself.taskParams
    self.parameters.commandingEntities = integration.getEntitiesFromAutomatCommunication( meKnowledge, "none", self.params.withPC )
    self.operationnalEntities = integration.getOperationnalEntitiesFromAutomat( meKnowledge, "none", self.params.withPC )
    for _, pion in pairs( self.parameters.commandingEntities ) do
        integration.setNeedReinforcement( pion, nil, nil, false )
    end
    -- Init
    local Activate = Activate
    myself.leadData = {}
    myself.leadData.workMap = {}
    myself.leadData.taskError = false
    self.bestUnits = {}
    self.entitiesWithoutMission = copyTable( self.parameters.commandingEntities )
    myself.feedback = false
    self.companyTask = integration.RetrieveAutomateTask( meKnowledge, self.params.companyTask )
    if self.params.mainTasks then
        meKnowledge.nbPionsMain = integration.nbPlatoonsHaveTask( self.parameters.commandingEntities, self.params.mainTasks )
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
        pcall( func, self )
    end

    -- Initialisation des missions pion
    -- Le pion PC rejoint le meetingPoint
    if givePCTask and self.params.pcTasks and self.params.pcTasks ~= NIL then
        self.parameters.pcObjective = self.params.pcObjective
        integration.issueMission ( self, self.params.pcTasks, 1, eEtatEchelon_Reserve, { integration.query.getPCUnit() }, false, findBestsFunction, disengageWhenTacticallyDestroyed )
    end

    -- S'il y a des obstacles � construire, les pions GEN (ou PIA) vont le construire
    if giveEngineerTask and self.params.engineerTask and self.params.engineerTask ~= NIL then
        integration.issueMission ( self, self.params.engineerTask,  #self.companyTask:getObstaclesPlan( myself.taskParams ), eEtatEchelon_None, nil, false, findBestsFunction, disengageWhenTacticallyDestroyed )
    end

    -- Le premier echelon recoit les missions principales ("mainTasks")
    if giveMainTask then
        local bestUnits = integration.issueMission ( self, self.params.mainTasks, self.nbrFront, eEtatEchelon_First, nil, true, findBestsFunction, disengageWhenTacticallyDestroyed )
        if #bestUnits == 0 then
            Activate( self.skill.links.RC, 1, { RC = eRC_NoPEInAutomat } )
            return
        end
        integration.manageFeedback( bestUnits )
        
        self.bestUnits = copyTable( bestUnits )
    end

    -- Le second echelon recoit les missions de "supportTask"
    if giveSupportTask then
        integration.issueMission ( self, self.params.supportTasks, #self.entitiesWithoutMission, eEtatEchelon_Second, nil, false, findBestsFunction, disengageWhenTacticallyDestroyed )
    end

    -- Les pions PEI
    if givePEITask and self.params.peiTasks and self.params.peiTasks ~= NIL then
        integration.issueMission ( self, self.params.peiTasks, self.nbrFront, eEtatEchelon_Scout, nil, false, findBestsFunction, disengageWhenTacticallyDestroyed )
    end

    -- Ceux qui n'ont toujours pas de mission recoivent la mission par d�faut
    if giveDefaultTask then
        integration.issueMission ( self, self.params.defaultTask, #self.entitiesWithoutMission, eEtatEchelon_Reserve, nil, false, findBestsFunction, disengageWhenTacticallyDestroyed )
    end
end


--- Generic activate for Lead skills
-- @param self: The leading skill
-- @param listenFrontElement : Whether or not there should be a call to integration.ListenFrontElement at the beginning of the function
-- @param endMissionBeforeCoordination : Whether the managing of the end of the mission should be done before or after the managing of the coordination
-- @param manageRelieveBeforeCoordination : Whether the managing of the relieve units should be done before or after the managing of the coordination
-- @param manageSecondEchelonWhenNoCoordination : Whether or not the second echelon should be managed when there is no coordination management
-- @param manageRCnoPEInAutomatWhenNoCoordination : Whether or not the RC "No PE In Automat" should be managed when there is no coordination management
-- @param assignDefaultTaskToSE : Whether or not the default task should be assigned to the second echelon (along with the support tasks)
-- @param findBestsFunction: The "find bests" method used to find the best units in integration.issueMission (for example : integration.findBests)
-- @param disengageWhenTacticallyDestroyed : Whether or not the units should be issued the Disengage mission when tactically destroyed
-- @author NMI
-- @release 2013-07-05
integration.leadActivate = function( self, listenFrontElement, endMissionBeforeCoordination, manageRelieveBeforeCoordination,
                            manageSecondEchelonWhenNoCoordination, manageRCnoPEInAutomatWhenNoCoordination, assignDefaultTaskToSE,
                            findBestsFunction, disengageWhenTacticallyDestroyed )
                                   
    if myself.newTask then
      self:create()
    end
          
    if listenFrontElement and self.listenFrontElementInitialized then -- if a subordinate destroyed before and tasks issued a new time
        for _, elem in pairs( self.bestUnits or emptyTable ) do
            integration.ListenFrontElement( elem.entity )
        end
        self.listenFrontElementInitialized = false
    end

    if self.params.manageAddedAndDeletedUnits ~= false then
        integration.manageAddedAndDeletedUnits( self, findBestsFunction, disengageWhenTacticallyDestroyed)
    end

    local Activate = Activate
    local echelons = integration.getPionsInEchelons( self.parameters.commandingEntities )
    local pionsPE =  echelons[1]
    local pionsSE =  echelons[2]
    local pionsEE =  echelons[3]

    -- Gestion du CR quand les pions PE arrivent sur les limas de type LC ou LD
    Activate( self.skill.links.synchronizeRC, 1, { entities = pionsPE } )

    if endMissionBeforeCoordination then
        integration.manageEndMission( self )
    end

    -- Gestion du soutien
    Activate( self.skill.links.supportManager, 1, { companyTask = self.companyTask, parameters = self.parameters, PE = pionsPE, SE = pionsSE, taskForSupporting = self.params.taskForSupporting })

    if manageRelieveBeforeCoordination then
        -- Gestion de la relève
        Activate( self.skill.links.relieveManager, 1, { pions = pionsPE, releve = pionsSE})
    end

    if not self.params.noCoordination then
        local fuseau = meKnowledge:getFuseau()
        

        -- Gestion de l'elongation   
        -- tous les pions sont projet� sur un des deux fuseaux de l'automate
        -- self.progressionInAOR: contient les valeurs de progression sur l'axe de progression
        -- plus la valeur est grande, plus le pion est en avant dans le fuseau
        -- Effet de bord indesirable : en cas d'aller retour dans le fuseau la coordination est compl鴥ment inutilisable 
        local proj
        for _, entity in pairs( self.parameters.commandingEntities ) do
            proj = integration.advanceAlongAOR( entity.source ) 
            self.progressionInAOR[ entity ] = proj
        end
    
        local pionHasSupport, supportDistance
        local maxsupportDistance = 1200
        for _, pion in pairs( pionsSE ) do
              supportDistance = integration.getSupportDistanceToCoordination( pion.source, 0.5 )
              if supportDistance > maxsupportDistance then
                  maxsupportDistance = supportDistance
              end
        end
      
        local automatePosition = meKnowledge:getPosition()
        local inCity = integration.pointIsInCity(automatePosition)
    
        local largeurFuseau = fuseau:getWidth()

        if inCity then
          maxsupportDistance = maxsupportDistance/4
        else
          maxsupportDistance = maxsupportDistance/2
        end
       
        -- Gestion de l'�longation intra echelon   
        Activate( self.skill.links.coordinationManager , 1, { enititesFromEchelon = pionsPE, progressionInAOR = self.progressionInAOR, distance = largeurFuseau/2 } )
        Activate( self.skill.links.coordinationManager , 1, { enititesFromEchelon = pionsEE, progressionInAOR = self.progressionInAOR, distance = largeurFuseau/2 } )
        
        -- Gestion de l'�longation inter echelon
        Activate( self.skill.links.coordinationManager , 1, { enititesFromEchelon = fusionList( pionsPE, pionsSE ), progressionInAOR = self.progressionInAOR, distance = maxsupportDistance } )
    end

    if not manageRelieveBeforeCoordination then
        -- Gestion de la relève
        Activate( self.skill.links.relieveManager, 1, { pions = pionsPE, releve = pionsSE})
    end

    if not ( not manageSecondEchelonWhenNoCoordination and self.params.noCoordination ) then
        -- Gestion du SE (car les objectifs ne sont plus les m�mes)
        local tasksForSE = self.params.supportTasks
        if assignDefaultTaskToSE then
            tasksForSE = tasksForSE .. ";" .. self.params.defaultTask
        end
        Activate( self.skill.links.AssignSETask, 1, { SE = pionsSE, tasks = tasksForSE, parameters = self.parameters, companyTask = self.companyTask })
    end


    if not endMissionBeforeCoordination then
        integration.manageEndMission( self )
    end
    
    if not( not manageRCnoPEInAutomatWhenNoCoordination and self.params.noCoordination ) then
        if not next(integration.getPionsInEchelons( self.parameters.commandingEntities )[1]) then
            Activate( self.skill.links.RC, 1, { RC = eRC_NoPEInAutomat } )
        end
    end
end

--- Specific activate method for LeadDelay skill
-- @param self: The leading skill
-- @author NMI
-- @release 2013-07-05
integration.leadDelayActivate = function( self )  
      integration.manageAddedAndDeletedUnits( self, integration.findBests, true )
    
        -- Mis à jour des echelons
      integration.setPionsEchelons( myself.leadData.pionsLima1, eEtatEchelon_First )
      integration.setPionsEchelons( myself.leadData.pionsLima2, eEtatEchelon_Second )
      
      -- Changement d'échelons lorsque l'ordre de conduite Decrocher a été donné et que tous le deuxième échelon est en place, prêt à appuyer
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
      if ( not self.decrocher and meKnowledge:hasPionsInDanger( myself.leadData.pionsLima1 ) ) 
                and meKnowledge.arrivedUnits
                and #meKnowledge.arrivedUnits >= tableSize( myself.leadData.pionsLima1 ) then
              local fragOrder = integration.createFragOrder("Disengage")
              local fragOrderKn = CreateKnowledge( military.world.FragOrder, fragOrder )

              meKnowledge.pionsToAwait = copyTable( myself.leadData.pionsLima1 )
              
              -- Keep only the operational units
              
              for index, entity in pairs( meKnowledge.pionsToAwait ) do
                  if not exists( self.operationnalEntities, entity ) then
                      meKnowledge.pionsToAwait[ index ] = nil
                  end
              end
              
              for k,v in pairs( meKnowledge.arrivedUnits ) do meKnowledge.arrivedUnits[ k ] = nil end
              
              meKnowledge.pionsToAwaitSource = {}
              for i,k in pairs( meKnowledge.pionsToAwait ) do
                  meKnowledge.pionsToAwaitSource[i.source] = k.source
              end

              Activate( self.skill.links.manageFragOrder, 1, { fragOrders = { fragOrderKn } , entities = myself.leadData.pionsLima1 } )
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
              meKnowledge:sendTaskToPion( supportingEntity , taskForSupporting , { objectives = tableObjectives, retrogradeContext = true } , eEtatEchelon_Reserve )  
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

      local fuseau = meKnowledge:getFuseau()

      largeurFuseau = fuseau:getWidth()

      Activate( self.skill.links.coordinationManager , 1, { enititesFromEchelon = myself.leadData.pionsLima1, progressionInAOR = self.progressionInAOR, distance = largeurFuseau/2 } )

      integration.manageEndMission( self )

      -- Gestion du SE (car les objectifs ne sont plus les mêmes)
      local tasksForSE = self.params.supportTasks..";"..self.params.defaultTask
      Activate( self.skill.links.AssignSETask, 1, { SE = pionsSE, tasks = tasksForSE, parameters = self.parameters, companyTask = self.companyTask })
      if not next(integration.getPionsInEchelons( self.parameters.commandingEntities )[1]) then
        Activate( self.skill.links.RC, 1, { RC = eRC_NoPEInAutomat } )
      end
end

--- Specific activate method for LeadDrone skill
-- @param self: The leading skill
-- @author NMI
-- @release 2013-07-05
integration.leadDroneActivate = function( self )
    local Activate = Activate
    local echelons = integration.getPionsInEchelons( self.parameters.commandingEntities )
    local pionsPE =  echelons[1]
    local pionsSE =  echelons[2]
    local pionsEE =  echelons[3]
    
    if meKnowledge.availableDrone then
        local drones = { meKnowledge.availableDrone }
        -- Le premier echelon recoit les missions principales ("mainTasks")
        local bestUnits = integration.issueMission ( self, self.params.droneTasks, 1, eEtatEchelon_First, drones, false, integration.findBests, false)
        for _, elem in pairs ( drones ) do
              integration.ListenFrontElement( elem )
        end
        meKnowledge.availableDrone = nil
    end
    
    integration.manageEndMission( self )
end

--- Generic destroy for Lead skills
-- @param self: The leading skill
-- @param setEchelonNone: Whether or not the echelon state of the commanding entities should be set at "none"
-- @author NMI
-- @release 2013-07-05
integration.leadDestroy = function ( self, setEchelonNone )
    if setEchelonNone then
        for i = 1, #self.parameters.commandingEntities do
           integration.setEchelonState( self.parameters.commandingEntities[i].source, eEtatEchelon_None )
        end
    end
    self.parameters.pcObjective = nil
end