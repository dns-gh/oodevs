local result = 
{
    getPionEfficiency = function( self )
        return ePionEfficiencyRecon
    end,

    fillParameters = function( self, companyTask, params, entity )
        local positions = companyTask:getObjectivesPei( params, entity )
        local objective = companyTask:getDirection( params, entity, positions[next(positions)] )
        local indexPositions = next( positions )
        if indexPositions and objective then
            return  { positions = positions , objective = objective }
        else
            error( "No objective or positions for task "..tostring(self.name).." in company task "..tostring(companyTask.name) )
        end
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["agent.tasks.knowledges.Monitor"] = t

return result
