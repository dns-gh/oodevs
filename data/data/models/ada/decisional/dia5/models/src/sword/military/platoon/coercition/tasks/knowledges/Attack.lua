local result = 
{
    getPionEfficiency = function( self )
        return ePionEfficiencyCombat
    end,

    fillParameters = function( self, companyTask, params, entity )
        local fuseau = companyTask:getReachable( params )
        if fuseau then
          local point = CreateKnowledge( sword.military.world.Point, DEC_Geometrie_CalculerPointArriveePourFuseau(fuseau.source))
          return { objective = point, fuseau = fuseau }
        end
        return nil --use by keepBest in Lead skill
    end,
}

local t = initTaskKnowledge( result )
taskKnowledge["sword.military.platoon.coercition.tasks.knowledges.Attack"] = t

return result
