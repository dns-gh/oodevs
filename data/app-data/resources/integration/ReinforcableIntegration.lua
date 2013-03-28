-------------------------------------------------------------------------------
-- Reinforcable Implementation : 
-- Regroup function to manage reinfrocement
-- @author MGD
-- @created 2010-08-27
--
-- This file is part of a MASA library or program.
-- Refer to the included end-user license agreement for restrictions.
--
-- Copyright (c) 2010 Math�matiques Appliqu�es SA (MASA)
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
    meKnowledge:RC( eRC_ReussiteRenforcementPion, meKnowledge.source, platoonAlly.source )
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
        meKnowledge:RC( eRC_ReussiteRenforcement, meKnowledge.source, companyAlly.source )
        return true
    else
        meKnowledge:RC( eRC_EchecRenforcement, meKnowledge.source, companyAlly.source )
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
            unit:sendDotationNeededForReinforcement( integration.GetSuperiorKnowledge( unit ), obstacle, dotation, number , unit )
        else
            unit:sendCapacityNeededForReinforcement( integration.GetSuperiorKnowledge( unit ), obstacle, unit, action )
        end
    else
        unit:sendNoLongerNeedReinforcement( integration.GetSuperiorKnowledge( unit ), unit )    
    end
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