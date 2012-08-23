local result = 
{
    getPionEfficiency = function( self )
        return ePionEfficiencyRecon
    end,

    fillParameters = function( self, companyTask, params, entity )
        local positions, fuseau = companyTask:getObjectives( params, entity )
        local totalRecce = companyTask:getTotalRecce()
        if positions then
          return { objectives = positions,  searchObjective = totalRecce, fuseau = fuseau }
        end
        error( "No objective for task "..tostring(self.name).." in company task "..tostring(companyTask.name) )
    end,
}

local t = initTaskKnowledge( result )
taskKnowledge["platoon.combat.support.circu.tasks.knowledges.ReconnoiterCircu"] = t

return result
