local result = 
{
    getPionEfficiency = function( self )
        return ePionEfficiencyCommon
    end,

    fillParameters = function( self, companyTask, params )
        local defaultObjectives = companyTask:getEntitiesToFollow( params )
        local secondaryObjectives = companyTask:getEntitiesToSupport( params )
        if next( defaultObjectives ) then
             return { objective = defaultObjectives[next( defaultObjectives )] }
        elseif next( secondaryObjectives ) then
             return { objective = secondaryObjectives[next( secondaryObjectives )] }
        end
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["sword.military.platoon.tasks.knowledges.Follow"] = t

return result
