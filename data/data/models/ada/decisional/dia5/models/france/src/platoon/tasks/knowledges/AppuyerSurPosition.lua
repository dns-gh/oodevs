local result = 
{
    efficiencyForEffect = function( self, effect )
        if effect == eEffectSupport then
            return 100
        end
        return 0
    end,

    fillParameters = function( self, companyTask, params )
        if companyTask then
            local position = companyTask:getSupportingPosition( params )
            local unitsToSupport = companyTask:getEntitiesToSupport( params )
            local objective = companyTask:getDirection( params )
            return { position = position, unitsToSupport = unitsToSupport, objective = objective }
        else
            return {objectives = params}
        end
    end,

    getMainEffect = function( self )
        return eEffectSupport
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["platoon.tasks.knowledges.AppuyerSurPosition"] = t

return result
