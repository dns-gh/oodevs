local result = 
{
    getPionEfficiency = function( self )
        return ePionEfficiencyCombatSupport
    end,

    fillParameters = function( self, companyTask, params, entity, retrogradeContext, objectif )
        if retrogradeContext then
            return { objectives = companyTask:getEntitiesToSupport( params ), retrogradeContext = true }
        end
        if objectif and objectif ~= NIL then
            return { objectives = { objectif }, retrogradeContext = false }
        else
            return { objectives = companyTask:getEntitiesToSupport( params ), retrogradeContext = false }
        end
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["france.military.platoon.tasks.knowledges.Appuyer"] = t

return result
