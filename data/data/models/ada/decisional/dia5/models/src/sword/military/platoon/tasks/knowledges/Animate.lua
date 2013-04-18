local result = 
{
    fillParameters = function( self, companyTask, params )
        local entities = companyTask:getObstacles( params )
        if next( entities ) then
            return { entities = entities[1] }
        end
        return nil --use by keepBest in Lead skill
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["sword.military.platoon.tasks.knowledges.Animate"] = t

return result