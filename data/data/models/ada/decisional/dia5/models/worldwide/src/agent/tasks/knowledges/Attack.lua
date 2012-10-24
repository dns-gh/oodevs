local result = 
{
    getPionEfficiency = function( self )
        return ePionEfficiencyCombat
    end,

    fillParameters = function( self, companyTask, params, entity )
        local fuseau = companyTask:getReachable( params )
        if fuseau then
          local point = CreateKnowledge( world.Point, integration.computeArrivedPointForAOR(fuseau.source))
          return { objective = point, fuseau = fuseau }
        end
        return nil --use by keepBest in Lead skill
    end,
}

local t = initTaskKnowledge( result )
taskKnowledge["agent.tasks.knowledges.Attack"] = t

return result
