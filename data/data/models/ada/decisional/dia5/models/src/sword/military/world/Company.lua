local emptyTable = emptyTable

masalife.brain.communication.setMessageTreatment( "report",
    function( content, sender )
        if F_Pion_GeteEtatEchelon( content.platoon.source ) == eEtatEchelon_First then
            if content.state then
                myself.reportMessages[content.lima] = (myself.reportMessages[content.lima] or 0) + 1
            else
                myself.reportMessages[content.lima] = myself.reportMessages[content.lima] - 1
            end
        end
    end )

masalife.brain.communication.setMessageTreatment( "ArrivedOnLima",
    function( content, sender )
        if content.arrived and meKnowledge.pionsToAwait then
            meKnowledge.pionsToAwait[ content.me ] = nil
        end
    end )

masalife.brain.communication.setMessageTreatment( "constructedObject",
    function( content, sender )
      meKnowledge.constructedObjects = meKnowledge.constructedObjects or {}
      meKnowledge.constructedObjects[#meKnowledge.constructedObjects + 1] = content.object
      meKnowledge.constructedObjects[#meKnowledge.constructedObjects].alreadyGiven = false
      myself.numberOfConstructedObject = myself.numberOfConstructedObject or 0
      myself.numberOfConstructedObject = myself.numberOfConstructedObject + 1 
    end )
    
masalife.brain.communication.setMessageTreatment( "initializeConstructedObject",
    function( content, sender )
      meKnowledge.constructedObjects = nil
      myself.numberOfConstructedObject = 0
    end )

masalife.brain.communication.setMessageTreatment( "obstacleShouldBeActivated",
    function( content, sender )
    meKnowledge:RC( eRC_ObstacleEnAttenteActivation, content.object.source )  
    end )
    
masalife.brain.communication.setMessageTreatment( "needDotationByReinforcement",
    function( content, sender )
       content.unit.source.needReinforcementForDotation = content.dotation
       content.unit.source.needReinforcementForNumberMissing = content.numberMissing
       content.unit.source.needReinforcementForObstacle = content.object
       content.unit.source.stopNeedReinforcement = false
    end )
masalife.brain.communication.setMessageTreatment( "needCapacityByReinforcement",
    function( content, sender )
       content.unit.source.needReinforcementForCapacity = content.object
       content.unit.source.needReinforcementForAction = content.action
       content.unit.source.stopNeedReinforcement = false
    end )
masalife.brain.communication.setMessageTreatment( "stopNeedReinforcement",
    function( content, sender )
       content.unit.source.needReinforcementForDotation = nil
       content.unit.source.needReinforcementForNumberMissing = nil
       content.unit.source.needReinforcementForObstacle = nil
       content.unit.source.needReinforcementForCapacity = nil
       content.unit.source.needReinforcementForAction = nil
       content.unit.source.stopNeedReinforcement = true
    end )
masalife.brain.communication.setMessageTreatment( "reinforceGivenByUser",
    function( content, sender )
       content.unit.source.reinforceGivenByUser = true
    end )
masalife.brain.communication.setMessageTreatment( "writeCriticalIntelligence",
    function( content, sender )
    meKnowledge:RC( content.RC, content.criticalIntelligence )  
    end )
masalife.brain.communication.setMessageTreatment( "noDisponibleDrone",
    function( content, sender )
    meKnowledge:RC( eRC_PasDeDroneDisponible )  
    end )
masalife.brain.communication.setMessageTreatment( "disponibleDrone",
    function( content, sender )
    meKnowledge.availableDrone = content 
    end )
masalife.brain.communication.setMessageTreatment( "TaskStopped",
    function( content, sender )
    myself.stopFrago = true
    end )
    
-- -------------------------------------------------------------------------------- 
-- Predicates
-- --------------------------------------------------------------------------------
predicate "isReinforced"
{
    dependencies = "none",
    method = function( self )
        return self:reinforcementLevel() > 0 -- A company could be reinforced any number of times
    end
}
predicate "isCommanding"
{
    dependencies = "none",
    method = function( self )
        return DEC_Automate_EstEmbraye()
    end
}
predicate "isFar"
{ 
    dependencies = "none",
    method = function( self )
        return self:proximityLevel() <= 50 and self:proximityLevel() > 0
    end
}
predicate "isHostile"
{
    dependencies = "none",
    method = function( self )
        return self:hostilityLevel() > 66
    end
}
predicate "isPerceived"
{
    dependencies = "none",
    method = function( self )
        return self:perceptionLevel() > 0
    end
}
predicate "isContaminated"
{
    dependencies = "none",
    method = function( self )
        local DEC_Automate_PionEstContamine = DEC_Automate_PionEstContamine
        local listCommanding = self.source:DEC_Automate_PionsAvecPC()
        for _, entity in pairs( listCommanding or emptyTable ) do
            if DEC_Automate_PionEstContamine( entity ) then
                return true
            end
        end
        return false
    end
}
predicate "isPoisoned"
{
    dependencies = "none",
    method = function( self )
        local DEC_Automate_PionEstEmpoisonne = DEC_Automate_PionEstEmpoisonne
        local listCommanding = self.source:DEC_Automate_PionsAvecPC()
        for _, entity in pairs( listCommanding or emptyTable ) do
            if DEC_Automate_PionEstEmpoisonne( entity ) then
                return true
            end
        end
        return false
    end
}
predicate "isOperational"
{
    dependencies = "none",
    method = function( self )
        local listCommanding = self.source:DEC_Automate_PionsAvecPC()
        local operationalThreshold = 0
        local listOperational = 0
        for _, entity in pairs( listCommanding or emptyTable ) do
            if entity:DEC_Agent_EtatOpsMajeur() ~= 0 then
                listOperational = listOperational +1
            end
        end
        return (listOperational > operationalThreshold)
    end
}

return
{
    -- -------------------------------------------------------------------------------- 
    -- High level decisional methodes
    -- --------------------------------------------------------------------------------
    isNearby = function( self )
        return self:proximityLevel() > 50
    end,
    isDistant = function( self )
        return self:proximityLevel() == 0
    end,
    isReached = function( self )
        return self:proximityLevel() == 100
    end,
    proximityLevel = function( self )
        return self:getProximity( meKnowledge )
    end,
    perceptionLevel = function( self )
        return 100 -- $$$ a friend units, in same knoxledge group is always perceived.
    end,
    accessibilityLevel = function( self )
        return 100 -- hypothesis: always accessible.
    end,
    hostilityLevel = function( self )
        return 0 -- this company represent a friend company
    end,
    isNeutral = function( self )
        return self:hostilityLevel() >= 33 and self:hostilityLevel() <= 66
    end,
    isFriend = function( self )
        return self:hostilityLevel() < 33
    end,
    reinforcementLevel = function( self )
        return 100 -- A company is always considerated as a task organized unit with reinforcements.
    end,
    reinforcementPriority = function( self )
        return ( math.max( self:proximityLevel(), 1 ) ) / 100
    end,
    reachPriority = function( self )
        return ( math.max( self:proximityLevel(), 1 ) ) / 100
    end,
    observationPriority = function( self )
        return math.max( self:proximityLevel(), 1 ) / 100
    end,
    supportPriority = function( self )
        return ( math.max( self:proximityLevel(), 1 ) ) / 100
    end,
    canReinforceIt = function( self )
        return integration.distance( self, meKnowledge ) < 800 -- self:isReached() is better
    end,
    reinforceIt =  masalife.brain.integration.startStopAction(
    { 
        start = integration.beginReinforcementCompany,
    } ),
    changeAutomate = function( self )
        integration.changeAutomate( self, true )
    end,
    isMoving = function( self )
        return integration.isCompanyMoving( self )
    end,
    canBeFollowed = function( self )
        return self:isOperational()
    end,

    -- -------------------------------------------------------------------------------- 
    -- Intagration and specific methods
    -- --------------------------------------------------------------------------------
    getPosition = function( self )
        return DEC_Automate_Position( self.source )
    end,
    getPositions = function( self )
      return { self:getPosition() }
    end,
    getMyPosition = function( self )
        return CreateKnowledge( sword.military.world.Point, self:getPosition() )
    end,
    isSupported = function( self )
        return next( myself.supportMessages ) == nil -- $$$ MIA TODO
    end,
    RC = function( self, ... )
        integration.genericRC( ... )
    end,
    sendTaskToPion = function( self, receiver, mission_type, mission_objectives, echelon )
        integration.SendMessage( "CreateMissionPion", receiver, { mission_type = mission_type,  mission_objectives = mission_objectives, echelon = echelon }, { type = "dynamic" } )
    end,
    sendFragOrder = function( self, receiver, fragOrder )
        integration.SendMessage( "FragOrder", receiver, { fragOrder = fragOrder }, { type = "dynamic" } )
        integration.cleanFragOrder( fragOrder )
    end,
    sendDataToNewUnitInAutomat = function( self, receiver, saintRegimeTravail, santePrioritesTact, maintPrioritesTact, maintPriorites, santePriorites, etatROEPopulation, etatROE  )
        integration.SendMessage("DataToNewUnitInAutomat", receiver, { saintRegimeTravail = saintRegimeTravail, santePrioritesTact = santePrioritesTact, maintPrioritesTact = maintPrioritesTact, maintPriorites = maintPriorites, santePriorites = santePriorites, etatROEPopulation = etatROEPopulation, etatROE = etatROE }, { type = "dynamic" } )
    end,
    slowDownSyncronisation = function( self, receiver )
        integration.SendMessage( "SlowDownSyncronisation", receiver, emptyTable, { type = "dynamic" } )
    end,
    continueSyncronisation = function( self, receiver )
        integration.SendMessage( "ContinueSyncronisation", receiver, emptyTable, { type = "dynamic" } )
    end,
    hasPionsInDanger = function( self, entities )
        for _, entity in pairs( entities ) do
            if meKnowledge:getForceRatioAgent( entity ) <= 0.55 then -- Le pion a un rapport de force neutre ou défavorable
                return true
            end
        end
        return false 
    end,
    getForceRatioAgent = function ( self, pion )
        return integration.getForceRatioAgent( pion )
    end,
    -- Create Knowledge Fuseau
    getFuseau = function( self )
        local fuseau = DEC_Fuseau()
        return CreateKnowledge( sword.military.world.Fuseau, fuseau )
    end,
    getProximity = function( self, reachable )
        return integration.normalizedInversedDistance( self, reachable )
    end,
    observeIt = function( self )
        return integration.observeIt( self  )
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
    moveToIt = function( self, pathType ) -- $$$ not used
        local pionToReach = CreateKnowledge( 
            sword.military.world.PlatoonAlly, DEC_Automate_PionPC() )
        return meKnowledge:moveToItIntegration( pionToReach, pathType )
    end,
    destroyMoveToIt = function( self )  -- $$$ not used
        return meKnowledge:destroyMoveToItIntegration( self )
    end,
    setROE = function(self, roeState )
      integration.setCompanyROE( roeState )
    end,
    shareKnowledge = masalife.brain.integration.startStopAction(
    {
        start = integration.shareKnowledgeAgentFromAutomat,
        started = waitInMin,
        stop = function( self )
            meKnowledge:RC( eRC_FinLiaison )
            return true
        end
    } ),
    shareInstantlyKnowledge = masalife.brain.integration.startStopAction(
    {
        started = function( self )
           integration.shareKnowledgeAgentFromAutomat( self, 0)
        end
    } ),
    UpdateNewUnitInAutomat = function( self, unit )
        local saintRegimeTravail = -1
        local santePrioritesTact = {}
        local maintPrioritesTact = {}	
        local maintPriorites = {} 	
        local santePriorites = {}	
        local etatROEPopulation = -1
        local etatROE = -1
        if DEC_Automate_isLogistic then
            if DEC_Automate_isLogistic() then
                if DEC_Maintenance_RegimeTravail then
                    saintRegimeTravail = DEC_Maintenance_RegimeTravail()
                end
                if DEC_Sante_PrioritesTactiques then
                    santePrioritesTact = DEC_Sante_PrioritesTactiques()
                end
                if DEC_Maintenance_PrioritesTactiques then
                    maintPrioritesTact = DEC_Maintenance_PrioritesTactiques()
                end
                if DEC_Maintenance_Priorites then
                    maintPriorites = DEC_Maintenance_Priorites()
                end
                if DEC_Sante_Priorites then
                    santePriorites = DEC_Sante_Priorites()
                end
            end
        end
        if DEC_Automate_ROEPopulation then
            etatROEPopulation = DEC_Automate_ROEPopulation()
        end
        if DEC_Automate_ROE then
            etatROE = DEC_Automate_ROE()
        end
        meKnowledge:sendDataToNewUnitInAutomat( unit, saintRegimeTravail, santePrioritesTact, maintPrioritesTact, maintPriorites, santePriorites, etatROEPopulation, etatROE )
    end,
    getName = function( self )
        return integration.getName( self )
    end
}
