local result = 
{
    getPionEfficiency = function( self )
        return ePionEfficiencyRecon
    end,

    fillParameters = function( self, companyTask, params, entity )
        local objective = companyTask:getDirection( params )
        if objective then
            return  { objective = objective }
        else
            error( "No objective or positions for task "..tostring(self.name).." in company task "..tostring(companyTask.name) )
        end
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["platoon.tasks.knowledges.RenseignerSur"] = t

return result
