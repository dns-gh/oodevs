local result = 
{
    fillParameters = function( self, companyTask, params )
        local allie = companyTask:getAllie( params )
        local objective = companyTask:getObjective( params )
        local position = companyTask:getPosition( params )
        local munition = companyTask:getMunition( params )
        local interventionType = companyTask:getInterventionType( params )
        if next(objective) and next(position) and next(munition) and next(interventionType) then
            return { allie = allie, objective = objective[1], position = position[1], munition = munition[1], interventionType = interventionType[1] }
        end
        return nil --use by keepBest in Lead skill
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["platoon.combat.support.art.tasks.knowledges.CoordinateIllumination"] = t

return result
