local result = 
{
    getPionEfficiency = function( self )
        return ePionEfficiencyCombat
    end,

    fillParameters = function( self, companyTask, params, entity )
        local objectives = companyTask:getDelayObjectives( params, entity )
        if next( objectives ) then
           return { positions = objectives }
        end
        return nil --use by keepBest in Lead skill
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["platoon.combat.tasks.knowledges.Delay"] = t

return result
