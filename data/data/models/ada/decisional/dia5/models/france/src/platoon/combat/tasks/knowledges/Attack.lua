local result = 
{
    getPionEfficiency = function( self )
        return ePionEfficiencyCombat
    end,

    fillParameters = function( self, companyTask, params, entity )
        local fuseau = companyTask:getReachable( params )
        if fuseau then
          local point = CreateKnowledge( world.Point, DEC_Geometrie_CalculerPointArriveePourFuseau(fuseau.source))
          return { objective = point, fuseau = fuseau }
        end
        return nil --use by keepBest in Lead skill
    end,
}

local t = initTaskKnowledge( result )
taskKnowledge["platoon.combat.tasks.knowledges.Attack"] = t

return result
