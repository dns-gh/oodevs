local result = 
{
    fillParameters = function( self, companyTask, params, entity )
        local paramsPositions = {}
        local positions, fuseau = companyTask:getScoutObjectives( params, entity )
        for _, objective in pairs( positions ) do
            paramsPositions[ #paramsPositions + 1 ] = objective:getMyPosition()
        end
        return { objectives = paramsPositions, fuseau = fuseau }
    end,

    getPionEfficiency = function( self )
        return ePionEfficiencyRecon
    end,
}

local t = initTaskKnowledge( result )
taskKnowledge[ "agent.tasks.knowledges.Reconnoiter" ] = t

return result