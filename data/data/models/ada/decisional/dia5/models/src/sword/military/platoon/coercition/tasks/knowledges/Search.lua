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
taskKnowledge["sword.military.platoon.coercition.tasks.knowledges.Search"] = t

return result