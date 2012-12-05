local result = 
{
    getPionEfficiency = function( self )
        return ePionEfficiencyCombatSupport
    end,

    fillParameters = function( self, companyTask, params, entity, marcheRetrograde, objectif )
        if marcheRetrograde then
            return { objectives = companyTask:getEntitiesToSupportDynamic( params ), marcheRetrograde = true }
        end
        if objectif and objectif ~= NIL then
            return { objectives = { objectif }, marcheRetrograde = false }
        else
            return { objectives = companyTask:getEntitiesToSupportDynamic( params ), marcheRetrograde = false }
        end
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["sword.military.platoon.coercition.tasks.knowledges.CombatSupport"] = t

return result
