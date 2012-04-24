local result = 
{
    getPionEfficiency = function( self )
        return ePionEfficiencyEngineeringSupport
    end,

    fillParameters = function( self, companyTask, params, entity, marcheRetrograde, objectif )
        if objectif and objectif ~= NIL then
            return { objective = objectif }
        else
            return { objective = companyTask:getEntityToReinforce( params ) }
        end	
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["sword.military.platoon.tasks.knowledges.Reinforce"] = t

return result
