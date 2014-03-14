-------------------------------------------------------------------------------
-- Reinforcable Implementation : 
-- Regroup function to manage reinfrocement
-- @author MGD
-- @created 2010-08-27
--
-- This file is part of a MASA library or program.
-- Refer to the included end-user license agreement for restrictions.
--
-- Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
-------------------------------------------------------------------------------

--- ReinforcementLevel : In first approximation the level is on top when one reinforcer is present
-- @param platoonAlly The reinforced integration.ontology.types.agent knowledge
-- @return int Indicate the current reinforcement level
-- @author MGD
-- @created 2010-08-27
integration.reinforcementLevel = function( platoonAlly )
    if DEC_Agent_NombreRenforts( platoonAlly.source ) > 0 then
        return 100
    else
        return 0
    end
end

--- Begin reinforcement action
-- @param platoonAlly The reinforced integration.ontology.types.agent knowledge
-- @return bool Indicate that reinforcement action is in progress
-- @author MGD
-- @created 2010-08-27
integration.beginReinforcementPlatoon = function( platoonAlly )
    DEC_Renforce( platoonAlly.source )
    reportFunction(eRC_ReussiteRenforcementPion, meKnowledge.source, platoonAlly.source )
    return true
end

integration.beginReinforcementAgentKnowledge = function( agentKnowledge )
    local agent = DEC_ConnaissanceAgent_EnAgent( agentKnowledge.source )
    DEC_Renforce( agent )
    reportFunction(eRC_ReussiteRenforcementPion, meKnowledge.source, agent )
    return true
end

--- Cancel reinforcement action
-- @return bool Indicate that reinforcement action is in progress
-- @author MGD
-- @created 2010-08-27
integration.cancelReinforcement = function( platoonAlly )
    DEC_AnnuleRenforcement()
    return false
end

--- Definitively switch of leading company
-- @param companyAlly The reinforced integration.ontology.types.automat knowledge
-- @return bool Indicate that reinforcement action is in progress
-- @author MGD
-- @created 2010-08-27
integration.beginReinforcementCompany = function( companyAlly ) 
    if DEC_Pion_ChangeAutomate( companyAlly.source ) then
        reportFunction(eRC_ReussiteRenforcement, meKnowledge.source, companyAlly.source )
        return true
    else
        reportFunction(eRC_EchecRenforcement, meKnowledge.source, companyAlly.source )
        return false
    end
end

integration.setNeedReinforcement = function( unit, action, obstacle, needDotation )
    if obstacle then
        if needDotation then
            local result = 0
            if action == "build" then
                if masalife.brain.core.class.isOfType( obstacle, integration.ontology.types.genObject) then
                    result = DEC_GetAgentDotationManquantePourConstruireObjet(unit.source, obstacle.source)
                else
                    result = DEC_GetAgentDotationManquantePourConstruireObjetExistant(unit.source, obstacle.source)
                end
            elseif action == "mine" then
                result = DEC_GetAgentDotationManquantePourValoriserObjet(unit.source, obstacle.source)
            end
            local dotation = result.first
            local number = result.second
            if unit.sendCapacityNeededForReinforcement then
                unit:sendDotationNeededForReinforcement( integration.GetSuperiorKnowledge( unit ), obstacle, dotation, number , unit )
            end
        else
            if unit.sendCapacityNeededForReinforcement then 
                unit:sendCapacityNeededForReinforcement( integration.GetSuperiorKnowledge( unit ), obstacle, unit, action )
            end
        end
    else
        if unit.sendCapacityNeededForReinforcement then
            unit:sendNoLongerNeedReinforcement( integration.GetSuperiorKnowledge( unit ), unit )
        end    
    end
end

integration.setObjectNeedingReinforcementForCapacity = function( unit, object )
    unit.source.needReinforcementForCapacity = object
end

integration.setObjectNeedingReinforcementForDotation = function( unit, dotation )
    unit.source.needReinforcementForDotation = dotation
end

integration.setObjectNeedingReinforcementForNumberMissing = function( unit, numberMissing )
    unit.source.needReinforcementForNumberMissing = numberMissing
end

integration.setObjectNeedingReinforcementForObstacle = function( unit, object )
    unit.source.needReinforcementForObstacle = object
end

integration.setObjectNeedingReinforcementForAction = function( unit, object )
    unit.source.needReinforcementForAction = object
end

integration.setObjectStopNeedingReinforcement = function( unit, boolean )
    unit.source.stopNeedReinforcement = boolean
end

integration.canReinforceWithDotation = function ( unitToSupport, support, object, action, dotation, quantite )
    return ( integration.dotationAgentPourConstruire( support, unitToSupport.source.needReinforcementForObstacle ) >= unitToSupport.source.needReinforcementForDotation )
end
integration.unitsAbleToReinforce = function ( unitToSupport, units, obstacle, action )
    list = {}
    for _, unit in pairs ( units ) do
        if not unit.source.reinforceGivenByUser then
            list[#list + 1] = unit.source
        end	
    end
    local localisationObject = obstacle:getLocalisation()
    local localisationisOK = false
    if localisationObject ~= nil then
        localisationisOK = true
    end
    if action == "build" and localisationisOK then
        return DEC_GetAgentsPouvantConstruireAvecLocalisation( list, obstacle:getType(), localisationObject )
    elseif action == "mine" and localisationisOK then
        return DEC_GetAgentsPouvantValoriserAvecLocalisation( list, obstacle:getType(), localisationObject )
    elseif action == "bypass" then
        return DEC_GetAgentsPouvantDevaloriser( list, obstacle:getType() )
    elseif action == "destroy" and localisationisOK then
        return DEC_GetAgentsPouvantDetruireAvecLocalisation( list, obstacle:getType(), localisationObject )
    elseif action == "demine" and localisationisOK then
        return DEC_GetAgentsPouvantDevaloriserAvecLocalisation( list, obstacle:getType(), localisationObject )
    else
        return {}
    end
end

integration.nombreDotationAgent =  function( unit, dotationCategory )
    return DEC_Agent_GetAgentDotation( unit.source, dotationCategory )
end

--- For a given set of 'kUnit', returns the table of units able to perform the given 'action'onto the given 'kObject'.
-- This method can only be called by an automat or an agent.
-- @param kUnits the set of directia agents to perform the work.
-- @param kObject the directia object to act on.
-- @param action the string name of the action to perform. Possible actions are:
-- "construct", "improve", "bypass", "deconstruct", "removeImprovement". If the provided 
-- action name is unknown, the method returns an empty table.
-- @return the table of simulation units able to perform the given action.
integration.unitsAbleToWorkOnObject = function ( kUnits, kObject, action )
    local localisationObject = kObject:getLocalisation()
    if not localisationObject then
        return emptyTable
    end 
    local simUnits = {}
    for i = 1, #kUnits do 
        simUnits[ i ] = kUnits[ i ].source
    end
    if action == "construct" then
        return DEC_GetAgentsPouvantConstruireAvecLocalisation( simUnits, kObject:getType(), localisationObject )
    elseif action == "improve" then
        return DEC_GetAgentsPouvantValoriserAvecLocalisation( simUnits, kObject:getType(), localisationObject )
    elseif action == "bypass" then
        return DEC_GetAgentsPouvantDevaloriser( simUnits, kObject:getType() )
    elseif action == "deconstruct" then
        return DEC_GetAgentsPouvantDetruireAvecLocalisation( simUnits, kObject:getType(), localisationObject )
    elseif action == "removeImprovement" then
        return DEC_GetAgentsPouvantDevaloriserAvecLocalisation( simUnits, kObject:getType(), localisationObject )
    else
        return emptyTable
    end
end

--- Returns 'true' if the provided directia agent (the 'kUnit' parameter) has dotation left to perform the imporvement
-- of the given directia object. It returns 'false' otherwise.
-- @param kUnits the directia agent to perform the work
-- @param kObject the directia object to act on.
-- @return boolean
integration.agentHasDotationToImprove = function( kUnit, kObject )
    local dotations = DEC_GetAgentDotationManquantePourValoriserObjet( kUnit.source, kObject.source ) -- returns the needed dotation to improve and the number left.
    return DEC_Agent_GetAgentDotation( kUnit.source, dotations.first ) >= 1 -- the agent has at least one dotation to improve
end