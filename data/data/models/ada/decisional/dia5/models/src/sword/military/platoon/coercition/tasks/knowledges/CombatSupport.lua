local result = 
{
    getPionEfficiency = function( self )
        return ePionEfficiencyCombatSupport
    end,

    fillParameters = function( self, companyTask, params, entity, context, objectif )
        if context == eContexte_Retrograde then
            return { objectives = companyTask:getEntitiesToSupport( params ), context = context }
        end
        if objectif and objectif ~= NIL then
            return { objectives = { objectif }, context = 0 }
        else
            return { objectives = companyTask:getEntitiesToSupport( params ), context = 0 }
        end
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["sword.military.platoon.coercition.tasks.knowledges.CombatSupport"] = t

return result
