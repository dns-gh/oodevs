local result = 
{
    fillParameters = function( self, companyTask, params, entity )
        local paramsPositions = {}
        local positions, fuseau = companyTask:getObjectives( params, entity )
        for _, objective in pairs( positions ) do
            paramsPositions[ #paramsPositions + 1 ] = objective:getMyPosition()
        end
        return { objectives = paramsPositions, fuseau = fuseau }
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["sword.military.platoon.coercition.tasks.knowledges.ReconnoiterRoute"] = t

return result
