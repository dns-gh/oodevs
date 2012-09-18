local result = 
{
    getPionEfficiency = function( self )
        return ePionEfficiencyCommon
    end,

    fillParameters = function( self, companyTask, params )
        local defaultObjectives = companyTask:getEntitiesToFollow( params )
        local secondaryObjectives = companyTask:getEntitiesToSupport( params )
        if next( defaultObjectives ) then
             return { objectives = defaultObjectives }
        elseif next( secondaryObjectives ) then
             return { objectives = secondaryObjectives }
        end
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["agent.tasks.knowledges.Follow"] = t

return result
