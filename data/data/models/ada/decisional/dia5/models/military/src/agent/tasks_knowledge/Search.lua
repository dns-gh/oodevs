local result = 
{
    getPionEfficiency = function( self )
        return ePionEfficiencyRecon
    end,

    fillParameters = function( self, companyTask, params )
        local searchables = companyTask:getSearchables( params )
        local index = next( searchables )
        if index then
            return  { objective = searchables[ index ] }
        else
            error( "No objective for task "..tostring(self.name).." in company task "..tostring(companyTask.name) )
        end
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["agent.tasks_knowledge.Search"] = t

return result
