masalife.brain.communication.setMessageTreatment( "CreateMissionPion", integration.communication.StartMissionPion )
masalife.brain.communication.setMessageTreatment( "CreateMissionPionVersPion", integration.communication.StartMissionPionVersPion )
masalife.brain.communication.setMessageTreatment( "ContinueSyncronisation", integration.communication.Continue )
masalife.brain.communication.setMessageTreatment( "SlowDownSyncronisation", integration.communication.SlowDown )

-- Store engineerObject to manage by EnginnerMobilitySupport task
masalife.brain.communication.setMessageTreatment( "needEngineerSupport",
    function( content, sender )
        myself.engineerObjectsOnPath[ 1 ] = content
    end )

masalife.brain.communication.setMessageTreatment( "FragOrder",
    function( content, sender )
        if not myself.fragOrders then
            myself.fragOrders = {}
        end
        myself.fragOrders[ content.fragOrder ] = content.fragOrder
    end )

-- The unit has got a route for escorting
masalife.brain.communication.setMessageTreatment( "getRouteForEscorting",
    function( content, sender )
        myself.getRouteForEscorting = content.value
    end )

--The unit can (or not) give a route
masalife.brain.communication.setMessageTreatment( "canGiveRoute",
    function( content, sender )
        myself.canGiveRoute = content.value
    end )

--The unit ask (or not) for a route
masalife.brain.communication.setMessageTreatment( "askForRoute",
    function( content, sender )
        myself.askForRoute = content.value
    end )

-- Speed of the unit
masalife.brain.communication.setMessageTreatment( "speedModulationHasToBeMax",
    function( content, sender )
        myself.speedModulationHasToBeMax = content.value
    end )

-- The obstacle is bypassed, the unit can move
masalife.brain.communication.setMessageTreatment( "obstacleByPassed",
    function( content, sender )
        myself.blocked = nil
    end )


--The unit is caught
masalife.brain.communication.setMessageTreatment( "capture",
    function( content, sender )
        meKnowledge:RC( eRC_Capture )
        myself.isCaught = true
    end )

--The uav is disponible
masalife.brain.communication.setMessageTreatment( "displayRC",
    function( content, sender )
        meKnowledge:RC( content.value )
    end )

masalife.brain.communication.setMessageTreatment( "EndOfMission",
    function( content, sender )
        if meKnowledge.dronesToEmbark then
          if content.cancel then
            meKnowledge.dronesToEmbark[ content.me ] = nil
          else
            meKnowledge.dronesToEmbark[ content.me ] = true
          end
        end
    end )

-- -------------------------------------------------------------------------------- 
-- Predicates
-- --------------------------------------------------------------------------------
predicate "isStressed"
{
    dependencies = {},
    method = function( self )
        return self:stressLevel() > 50
    end
}
predicate "isDepressed"
{
    dependencies = {},
    method = function( self )
        return self:moraleLevel() < 50
    end
}
predicate "isReinforced"
{
    dependencies = "none",
    method = function( self )
        return self:reinforcementLevel() == 100
    end
}
predicate "isMounted"
{
    dependencies = "none",
    method = function( self )
        return integration.isMounted( self )
    end
}
predicate "isDismounted"
{
    dependencies = "none",
    method = function( self )
        return integration.isDismounted( self )
    end
}
predicate "isLoaded"
{
    dependencies = "none",
    method = function( self )
        return self:loadLevel() == 100
    end
}
predicate "hasMission"
{
    dependencies = "none",
    method = function( self )
       return self.hasMissionValue
    end
}
-- Force ratio interfaces methods implementation
--
--   0     0.25   0.5    0.75    1
--   |------|------|------|------|   force ratio value
predicate "hasNeutralForceRatio"
{
    dependencies = {},
    method = function( self )
        local forceRatio = self:getForceRatio()
        return forceRatio >= 0.45 and forceRatio <= 0.55
    end
}
predicate "hasGoodForceRatio"
{
    dependencies = {},
    method = function( self )
        return self:getForceRatio() > 0.55 
    end
}
predicate "hasBadForceRatio"
{
    dependencies = {},
    method = function( self )
        return self:getForceRatio() < 0.45
    end
}
predicate "isSelfCommanding"
{
    dependencies = {},
    method = function( self )
        return not DEC_Agent_AutomateEstEmbraye()
    end
}
predicate "isDestroyed" 
{
    dependencies = {},
    method = function( self )
        return self:destructionLevel() == 100
    end 
}
predicate "isOperational"
{
    dependencies = {},
    method = function( self )
        return self:operationalLevel() ~= 0
    end 
}
predicate "isMoving"
{
    method = function( self )
        return integration.bodyIsMoving( self )
    end
}

predicate "isInCrowd"
{
    dependencies = "none",
    method = function( self )
       return integration.isInCrowd()
    end
}

predicate "fireForbiddenOnCrowd"
{
    dependencies = {},
    method = function( self )
        return integration.fireForbiddenOnCrowd()
    end,
}
predicate "WLRAllowedOnCrowd"
{
    dependencies = {},
    method = function( self )
        return integration.wlrAllowedOnCrowd()
    end,
}
predicate "fireAllowedOnCrowd"
{
    dependencies = {},
    method = function( self )
        return integration.fireAllowedOnCrowd()
    end,
}
predicate "isCommandingFor"
{
	-- $$$ MIA TODO a revoir, sert pas? Marche pas?
    dependencies = { self = { "communicate_" } },
    method = function( self )
        if self.communicate_ == nil then 
            CreateKnowledge( self.type, self.source, { communicate_ = false } )
        end
        return self.communicate_
    end
}

-- $$$ MIA TODO rapatrier les m�thodes ici.

return
{
    -- -------------------------------------------------------------------------------- 
    -- High level overrided methods
    -- --------------------------------------------------------------------------------
    -- $$$ MIA TODO rpatrier les m�thodes ici.
    approachPriority = integration.approachPriority,
    commandEfficiency = integration.unit_commandEfficiency,
    communicate = integration.unit_communicate,

    destructionLevel = function( self )
        return self:getDestructionState()
    end,
    destructionPriority = function( self )
        return 0 -- $$$ Pour le moment pas appel� car on ne d�truit pas d'amis dans SCIPIO
    end,
    canDestroyIt = function( self )
        return false -- $$$ Pour le moment pas appel� car on ne d�truit pas d'amis dans SCIPIO
    end,
    destroyIt = function( self )
        return false -- $$$ Pour le moment pas appel� car on ne d�truit pas d'amis dans SCIPIO
    end,
    operationalLevel = function( self )
        return self:getOperationalCapacity()
    end,
    neutralizationPriority = function() 
        return 0 -- $$$ Pour le moment pas appel� car on ne d�truit pas d'amis dans SCIPIO
    end,
    canNeutralizeIt = function( self )
        return false -- $$$ Pour le moment pas appel� car on ne d�truit pas d'amis dans SCIPIO
    end,
    neutralizeIt = function( self )
        return false -- $$$ Pour le moment pas appel� car on ne neutralise pas d'amis dans SCIPIO
    end,
    supportPriority = function( self )
        return (self:proximityLevel() / 50 + 1 - meKnowledge:getForceRatioAgent( self ))/1.02 
    end,
    isSupported = function( self )
        -- $$$ MIA TODO A revoir. Marche pas.
        return false
    end,
    canApproachIt = function( self )
       return meKnowledge:isSupported()
    end,
    reinforcementLevel = function( self )
        return integration.reinforcementLevel( self )
    end,
    reinforcementPriority = function( self )
        return math.max( self:proximityLevel() + ( 100 - self:reinforcementLevel() ), 1 ) / 200
    end,
    canReinforceIt = function( self )
        return integration.distance( self, meKnowledge ) < 100 -- $$$ sould be self:isReached()
    end,
    canRelieveIt = function ( self )
        return integration.canRelieveIt( self )
    end,
    canMount = function( self ) 
        return integration.canMount( self )
    end,
    canDismount = function( self )
        return integration.canDismount( self )
    end,
    canLoadIt = function( self )
        return integration.canLoadFriend( self, false ) 
               and self:isReached()and not self:isLoaded()
    end,
    canUnloadIt = function( self )
        return self:isLoaded() -- in skill unload, load element if arrived on position
    end,
    loadPriority = function( self )
        return math.max( self:proximityLevel(), 1 ) / 100
    end,
    unloadPriority = function( self )
        return math.max( self:proximityLevel(), 1 ) / 100
    end,
    loadLevel = function( self )
        return self:getLoadlevel()
    end,

    -- -------------------------------------------------------------------------------- 
    -- Integrations and sp�cifics methods
    -- --------------------------------------------------------------------------------
    isPosted = function( self )
        return integration.isPosted( self )
    end,
    isContaminated = function( self )
        return integration.isContaminated( self )
    end,
    isPoisoned = function( self )
        return integration.isPoisoned( self )
    end,
    sendTaskToPion = function( self, receiver, mission_type, mission_objectives, echelon )
        integration.SendMessage( "CreateMissionPionVersPion", receiver, { mission_type = mission_type,  mission_objectives = mission_objectives, echelon = echelon }, { type = "dynamic" } )
    end,
    sendForceRatio = function( self, receiver, myForceRatio )
            --FIXME remove from xml too
    end,
    askForMobilitySupport = function( self, receiver, engineerObject )
       integration.SendMessage( "needEngineerSupport", receiver, { engineerObject = engineerObject, platoon = meKnowledge }, { type = "dynamic" } )
    end,
    sendReport = function( self, receiver, lima, state )
       integration.SendMessage( "report", receiver, { lima = lima, platoon = meKnowledge, state = state }, { type = "dynamic" } )
    end,
    sendGetRouteForEscorting = function( self, receiver, value )
          integration.SendMessage( "getRouteForEscorting", receiver, {value = value}, { type = "dynamic" } )
    end,
    sendCanGiveRouteForEscorting = function( self, receiver, value )
        integration.SendMessage( "canGiveRoute", receiver, {value = value}, { type = "dynamic" } )
    end,
    sendNeedRoute = function( self, receiver, value )
        integration.SendMessage( "askForRoute", receiver, {value = value}, { type = "dynamic" } )
    end,
    sendSpeedModulationHasToBeMax = function( self, receiver, value )
        integration.SendMessage( "speedModulationHasToBeMax", receiver, {value = value}, { type = "dynamic" } )
    end,
    sendObstacleByPassed = function( self, receiver, value )
        integration.SendMessage( "obstacleByPassed", receiver, emptyTable, { type = "dynamic" } )
    end,
    sendObstacleToActivate = function( self, receiver, object )
        integration.SendMessage( "obstacleShouldBeActivated", receiver, {object = object}, { type = "dynamic" } )
    end,
    sendDotationNeededForReinforcement = function( self, receiver, object, dotation, numberMissing, unit )
        integration.SendMessage( "needDotationByReinforcement", receiver, {object = object, dotation = dotation, numberMissing = numberMissing, unit = unit}, { type = "dynamic" } )
    end,
    sendCapacityNeededForReinforcement = function( self, receiver, object, unit, action )
        integration.SendMessage( "needCapacityByReinforcement", receiver, {object = object, unit = unit, action = action}, { type = "dynamic" } )
    end,
    sendStopNeededReinforcement = function( self, receiver, unit )
        integration.SendMessage( "stopNeedReinforcement", receiver, { unit = unit }, { type = "dynamic" } )
    end,
    sendNoLongerNeedReinforcement = function( self, receiver, unit )
        integration.SendMessage( "needCapacityByReinforcement", receiver, {object = nil, unit = unit, action = nil}, { type = "dynamic" } )
        integration.SendMessage( "needDotationByReinforcement", receiver, {object = nil, dotation = 0, unit = unit }, { type = "dynamic" } )
    end,
    sendRC = function( self, receiver, value )
        integration.SendMessage( "displayRC", receiver, {value = value}, { type = "dynamic" } )
    end,
    sendNeedRC = function( self, receiver, value )
        integration.SendMessage( "needCR", receiver, {value = value}, { type = "dynamic" } )
    end,
    getObstaclesOnAllyPath = function( self )
        return myself.engineerObjectsOnPath
    end,
    computeProtectionCapability = function( self, objective, position )
        return position:getCoverAndConcealmentLevelFor( self, objective )
    end,
    computePerceptionCapability = function( self, objective, position )
        return math.max( position:computePerceptionCapabilityFor( self, objective ), 1 )
    end,
    computePerceptionCapabilityFor = function( self, entity, position )
        return integration.getPerception( self, position )
    end,
    computeMovementCapability = function( self, objective, position )
        if not DEC_IsPointInUrbanBlockTrafficable( position:getPosition() )
           and not DEC_Agent_EstEmbarquable() then
            return 0 
        else
            return 1
        end
    end,
    computeReconnaissanceCapability = function( self, objective, position )
        return position:getProximity( objective )
    end,
    computeSupportCapability = function( self, objective, position )
        -- Hypoth�se simplificatrice:plus la position est proche de l'objectif, 
        -- plus on estime que l'agent (ME) sera efficace pour soutenir.
        return math.max( position:computeSupportCapabilityFor( self, objective ), 1 )
    end,
    computeLoadCapability = function( self, objective, position )
         return position:getProximity( objective ) -- hypoth�se simplificatrice 
    end,
    computeUnloadCapability = function( self, objective, position )
         return position:getProximity( objective ) -- hypoth�se simplificatrice 
    end,
    computeIdentificationCapability = function ( self, objective, position )
        return math.max( integration.normalizedInversedDistance( objective, position ), 1 )
    end,
    computeDestructionCapability = function( self, objective, position )
        local pos = position:getPosition()
        if not pos then
          return 1
        end
        local attritionPower = integration.getAttrition( self, objective, position, 0.7 )
        if not integration.isPointInUrbanBlockTrafficable( position, true ) then
            if self:canDismount() then -- INF
                return position:getProximity( objective ) * attritionPower -- hypoth�se simplificatrice
            else
                return 0.1 * attritionPower
            end
        else -- cas normal
            return attritionPower
        end
    end,
    computeNeutralisationCapability = function( self, objective, position )
        if not integration.isPointInUrbanBlockTrafficable( position, true ) then
            if self:canDismount() then -- INF
                return position:getProximity( objective ) -- hypoth�se simplificatrice
            else
                return 0.1
            end
        else -- cas normal
            return math.max( integration.getAttrition( self, objective, position, 0.7 ), 1 ) -- sufficient PH estimated for destruction.
        end
    end,
    computeBuildCapability = function( self, objective, position )
        return math.max( integration.normalizedInversedDistance( objective, position ), 1 )
    end,
    coordinateWithIt = function( self )
        -- nothing $$$ MIA TODO 
    end,
    supportIt = function( self )
        -- nothing $$$ MIA TODO
    end,
    moveToIt = function( self, pathType ) -- not used
        return meKnowledge:moveToItIntegration( self, pathType )
    end,
    moveToItIntegration = function( self, entity, pathType )
        return integration.moveToItGeneric( entity, pathType )
    end,
    observeIt = function( self )
        return meKnowledge:orientateSensor( self )
    end,
    orientateSensor = function( self, entity )
        return integration.observeIt( entity )
    end,
    getPosition = function( self )
        if self == meKnowledge then
            return DEC_Agent_Position() -- Me
        else
            return DEC_Agent_PositionPtr( self.source ) -- a friend 
        end
    end,
    getMyPosition = function( self )
        return CreateProxyKnowledge(
            sword.military.world.ReachingArea, self, { distanceMin = 0, distanceMax = 10 } )
    end,
    getPositions = function( self )
        return DEC_Geometrie_CalculerTrafficablePointPourPoint( self:getPosition() )
    end,
    computeEstimateAttritionOnMe = function( self, threat )
        return ( DEC_ConnaissanceAgent_Dangerosite( threat.source ) - 1 ) * 100
    end,
    getProximity = function( self, reachable )
        return integration.normalizedInversedDistance( self, reachable )
    end,
    getDistanceTo = function( self, entity )
        return DEC_Geometrie_Distance( self:getPosition(), entity:getPosition() )
    end,
    computeRelation = function( self )
        return integration.computeRelationAgent( self )
    end,
    getPerception = function( self )
        return 100 -- a friend in my knowldege group is always perceived
    end,
    computePhysicalEfficiency = function( self, capacity )
        if capacity == ePionEfficiencyCommon then 
            return 1
        else
            return DEC_Automate_GetEfficacite( self.source, capacity )
        end
    end,
    computePhysicalEfficiencyForEffect = function( self, capacity )
        local coeffZurb = ( DEC_Automate_GetEfficacite( self.source, ePionEfficiencyUrbanArea )/100 )
        local etatOp = self.source:DEC_Agent_EtatOpsMajeur()
        local efficiency = self:computePhysicalEfficiency( capacity )
        return efficiency*coeffZurb*etatOp
    end,
    reinforceIt =  masalife.brain.integration.startStopAction(
    { 
        start = integration.beginReinforcementPlatoon,
        started = function() return true end,
        stop  = integration.cancelReinforcement
    } ),
    hasDotation = function( self, dotation )
        return integration.hasDotation( dotation )
    end,
    hasLauncherFor = function( self, dotation )
        return integration.hasLauncherFor( dotation )
    end,
    canUseDotation = function( self, dotation )
        return integration.canUseDotation( dotation )
    end,
    becomeStealthy = function( self, beStealth )
        integration.setStealth( beStealth ) -- $$$ MIA magic effect, independant from physical datas
    end,
    switchRadioOff = masalife.brain.integration.startStopAction(
    {
        start = integration.switchOffRadio
    } ),
    switchRadioPartialOff = masalife.brain.integration.startStopAction(
    {
        start = integration.switchEmitOnlyOffRadio
    } ),
    switchRadioOn = masalife.brain.integration.startStopAction(
    {
        start = integration.switchOnRadio
    } ),
    switchRadarOn = masalife.brain.integration.startStopAction(
    {
        start = integration.switchOnRadar
    } ),
    switchRadarOff = masalife.brain.integration.startStopAction(
    {
        start = integration.switchOffRadar
    } ),
    equipNBCOutfit = masalife.brain.integration.startStopAction( 
    {
        start = integration.equipNBCOutfit
    } ),
    equipProtectionNBC = function ( self )
        integration.equipNBCOutfit()
    end,
    unequipProtectionNBC = masalife.brain.integration.startStopAction( 
    {
        start = integration.unequipNBCOutfit
    } ),
    alertNBC = masalife.brain.integration.startStopAction(
    {
        start = integration.goOnNBCAlert,
    } ),
    moraleLevel = function( self ) 
        return 100  -- $$$ MIA good moral
    end,
    stressLevel = function( self )
        return 0  -- $$$ MIA no stress
    end,
    adoptSafetyAttitude = masalife.brain.integration.startStopAction(
    {
        start = function( self ) meKnowledge:RC( eRC_AmbianceSurete ) end,
        started = integration.switchOnSafetyMode,
        stop  = integration.switchOffSafetyMode
    } ),
    adoptCoverAttitude = masalife.brain.integration.startStopAction(
    {
        start = function( self ) meKnowledge:RC( eRC_CouvertureActive ) end,
        started = integration.switchOnCoverMode,
        stop  = integration.switchOffCoverMode
    } ),
    safetyAttitude = 
    {
        getter = function( self ) 
            if self.safetyAttitude_ == nil then 
                self.safetyAttitude_ = false 
            end
            return self.safetyAttitude_
        end,
        setter = function( self, value ) self.safetyAttitude_ = value end
    },
    getRole = function( self )
        return self.source:GetType()
    end,
    relieveIt = function ( self )
        integration.relieveIt( self )
    end,
    RC = function ( self, ... ) 
        integration.pionRC( ... )
    end,
    isPC = function( self )
        local pionPC = DEC_Automate_PionPC()
        if self.source == pionPC then
            return true
        end
        return false
    end,
        
    getTasks = function( self )
        return self.tasks
    end,
    tasks =
    {
        adder = function( self, task ) error( "unused method", 2 ) end,
        accepter = function( self, visitor )
            local function roleVisitor( role )
                if role.body == self then return role.tasks:accept( visitor ) end
            end
            for group in masalife.brain.knowledge.executeQuery( { knowledgeType = { directia.core.model.Group } } ) do
                return group.roles:accept( roleVisitor )
            end
        end,
        remover = function( self, task )
            error( "unused method", 2 )
        end
     },
    destroyMoveToIt = function( self )
        return meKnowledge:destroyMoveToItIntegration( self )
    end,
    destroyMoveToItIntegration = function( self, position )
        return integration.stopMoveToIt( position )
    end,
    mount = masalife.brain.integration.startStopAction( 
    {
        started = function( self ) return integration.mountStarted( self ) end,
        stop  = function( self ) integration.stopMount( self ) end
    } ),
    dismount = masalife.brain.integration.startStopAction(
    {
        started = function( self ) return integration.dismountStarted( self ) end,
        stop  = function( self ) integration.stopDismount( self ) end
    } ),
    getNearestReachingElementFor = function( self, objective )
        return integration.query.getNearestPointFor( objective )
    end,
    specificActionOnLCA = function( self )
        --NOTHING override later for NBC and ALAT
    end,
    getForceRatio = function( self )
        -- Interpolation lin�aire avec DEC_RapportDeForceLocal [1;5] --> [0;1]
        -- 0 signifie tr�s d�favorable, 1 tr�s favorable.
        return integration.getForceRatio()
    end,
    
    getForceRatioAgent = function( self, pion )
        -- Interpolation lin�aire avec DEC_RapportDeForceLocal [1;5] --> [0;1]
        -- 0 signifie tr�s d�favorable, 1 tr�s favorable pour le pion en param�tre.
        return integration.getForceRatioAgent( pion )
    end,
    
    setHasMission = function( self, value )
      self.hasMissionValue = value
    end,
    setFront = function( self )
        -- $$$ TODO
    end,
    setSupport = function( self )
        -- $$$ TODO
    end,
    setROE = function(self, roeState )
      integration.setROE( roeState )
    end,
    canIlluminate = function( self )
        return integration.canIlluminate()
    end,
    getOperationalCapacity = function( self )
        return DEC_Agent_EtatOpsMajeur() * 100
    end,
    getDestructionState = function( self ) -- destruction physique, ind�pendant des capacit�s op�rationnelles.
        return ( 1 - DEC_Agent_EtatOps() ) * 100
    end,
    getEchelon = function( self )
        return F_Pion_GeteEtatEchelon( meKnowledge.source )
    end,
    loadIt = function( self )
        return integration.loadFriend( self ) -- refugee
    end,
    unloadIt = function( self )
        return integration.unloadFriend( self )
    end,
    getLoadlevel = function( self )
        return integration.isFriendTranported( self ) and 100 or 0
    end,
    interrogateIt = masalife.brain.integration.startStopAction( 
    { 
        start = integration.startInterrogatePion,
        started = integration.updateInterrogatePion,
        stop = integration.stopInterrogatePion
    } ),
    selfDestruct = function( self )
        integration.selfDestruct()
    end,
    deploy = masalife.brain.integration.startStopAction( 
    { 
        start = integration.startDeploy,
        started = integration.deployed,
        stop = integration.undeploy
    } ),
    isDeployed = function( self )
        return integration.isDeployed( self )
    end,
    activateDrone = masalife.brain.integration.startStopAction( 
    { 
        start = integration.startActivateDrone,
        started = integration.startedActivateDrone,
        stop = integration.stopActivateDrone
    } ),
    treatInjured = masalife.brain.integration.startStopAction(
    { 
        start = function( self )
            integration.activateInjuredTreatment( )
        end,
    } ),
    modifyHumanWound = masalife.brain.integration.startStopAction(	
    { 
        start = function( self, blessuresTraitees )
            integration.modifyHumanWoundPriority( blessuresTraitees )
        end,
    } ),
    desactivateInjuredTreatment = function (self)
        integration.desactivateInjuredTreatment()
    end,
    hasRadar = function( self, typeRadar )
        return integration.hasRadar( typeRadar )
    end,
    adoptFiltrationPosture = masalife.brain.integration.startStopAction( 
    { 
        start = function( self, blockingStrength , bodySearchStrength , position )
            integration.buildInstantlyCheckPointOn( position )
            myself.CRCaptureSomeone = {}
        end,
        started = function( self, blockingStrength , bodySearchStrength , position )
            integration.doFiltration( bodySearchStrength, blockingStrength, position )
            integration.capture( integration.getNearbyTerrorists(), eRC_TerroristCaptured ) 
        end,
        stop = function( self, blockingStrength , bodySearchStrength , position )
            integration.destroyInstantlyCheckpointOn( position )
        end
    } ),
    adoptAnimatingDecontaminatePosture = masalife.brain.integration.startStopAction( 
    { 
        start = function( self, position )
            integration.buildInstantlyDecontaminatePlotOn( position )
        end,
        started = function( self, position )
            integration.animateDecontaminatePlot( position )
        end,
        stop = function( self, position )
            integration.destroyInstantlyDecontaminatePlotOn( position )
        end
    } ),
    buildObjectOnPosition = masalife.brain.integration.startStopAction( 
    { 
        start = function( self, typeObject, position )
            integration.buildInstantlyObjectOn( typeObject, position )
        end,
        stop = function( self, typeObject, position )
            integration.destroyInstantlyObjectOn( typeObject, position )
        end
    } ),
    escortIt = masalife.brain.integration.startStopAction( 
    { 
        start = function( self, taskName )
            if integration.isMissionAvailable( self, taskName ) then
                meKnowledge:sendTaskToPion( self, taskName, 
                    { objective = meKnowledge }, eEtatEchelon_Second )
                meKnowledge:RC( eRC_EscorteUnite ) 
                return true
            else
                --$$$ MIA! TODO escorte impossible.
                return false
            end
        end
    } ),
    transportTroups = masalife.brain.integration.startStopAction(
    {
        start = integration.startLoadQueue,
        started = integration.updateLoadQueue,
        stop = integration.stopLoadQueue
    } ),
    dropTroups = masalife.brain.integration.startStopAction(
    {
        start = integration.startUnloadQueue,
        started = integration.updateUnloadQueue,
        stop = integration.stopUnloadQueue
    } ),    
    addToTransport = function( self, onlyLoadable )
        integration.addPlatoonInQueue( self, onlyLoadable )
    end,
    readyForBeingTransported = function( self )
        return integration.readyForLoad( self )
    end,
    isTransportFinished = function( self )
        return integration.isTransportFinished()
    end,
    canTransportIt = function( self, onlyLoadable )
        return integration.canTransportUnit( self, onlyLoadable )
    end,
    canTransportCrowd = function( self )
        return integration.canTransportCrowd( self )
    end,
    isTransportingCrowd = function( self )
        return integration.isTransportingCrowd( self )
    end,
    isSurrendered = function( self )
        return integration.isSurrendered( self )
    end,
    changeAutomate = function( self )
        integration.changeAutomate( self )
    end,
    isEngaged = function( self )
        return self.engaged
    end,
    setEngaged = function( self, value )
        self.engaged = value
    end,
    
    isCaught = function( self )
        return myself.isCaught
    end,
    
    sendConstructedObject = function( self, receiver, object )
        integration.SendMessage( "constructedObject", receiver, { object = object }, { type = "dynamic" } )
    end,
    
    sendConstructedObjectInitialized = function( self, receiver, object )
        integration.SendMessage( "initializeConstructedObject", receiver, {}, { type = "dynamic" } )
    end,
    
    sendEndOfMission= function( self, receiver, sender, bCancel )
        integration.SendMessage( "EndOfMission", receiver, { cancel = bCancel, me = sender.source }, { type = "dynamic" } )
    end,
    getAutomat = function ( self )
        return CreateKnowledge( sword.military.world.Company, DEC_GetAutomate(self.source) )
    end,
    dischargeItIn = function( self, camp )
        return integration.dischargeAgent( self, camp )
    end,
    
    shareKnowledge = masalife.brain.integration.startStopAction(
    {
        start = integration.shareKnowledgeAgent,
        started = waitInMin,
        stop = function( self )
                meKnowledge:RC( eRC_FinLiaison )
                return true
            end
    } ),
    
    sortInjured = masalife.brain.integration.startStopAction(
    {
        start = integration.activateInjuredSorting,
        stop = integration.desactivateInjuredSorting
    } ),
    activateFunctions = masalife.brain.integration.startStopAction(
    {
        start = integration.activateFunctions,
        stop = integration.desactivateFunctions
    } ),
    activateSensors = masalife.brain.integration.startStopAction(
    {
        start = integration.activateSensors,
        stop = integration.deactivateSensors
    } ),
    LendTugs = masalife.brain.integration.startStopAction(
    {
        start = function( self, pionRenforce, pionARenforcer, nbrRemorqueurs)
            integration.StartLendTugs( pionRenforce, pionARenforcer, nbrRemorqueurs )
        end,
    } ),
    GetTugs = masalife.brain.integration.startStopAction(
    {
        start = function( self, pionRenforce, nbrRemorqueurs )
            integration.StartGetTugs( pionRenforce, nbrRemorqueurs )
        end,
    } ),
    LendVSRAM = masalife.brain.integration.startStopAction(
    {
        start = function( self, pionRenforce, pionARenforcer, nbrAmbulances)
            integration.StartLendVSRAM( pionRenforce, pionARenforcer, nbrAmbulances )
        end,
    } ),
    GetVSRAM = masalife.brain.integration.startStopAction(
    {
        start = function( self, pionRenforce, nbrAmbulances )
            integration.StartGetVSRAM( pionRenforce, nbrAmbulances )
        end,
    } ),
}
