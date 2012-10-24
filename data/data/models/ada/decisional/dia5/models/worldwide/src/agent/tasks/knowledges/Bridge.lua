local result = 
{
    fillParameters = function( self, companyTask, params, entity, retrogradeContext, objectif, taskName )
        if self.alreadyInit then
            companyTask:init()
            self.alreadyInit = true
        end
        local objective = companyTask:getSite( params )
        local system = companyTask:getBridgeSystem( params )
        return { crossingSite = objective, bridgeSystem = system }
    end,

    getObstaclesParameter = function( self, companyTask, params )
        return companyTask:getObjectives( params )
    end,

    hasDotation = function( self, entity, obstacle )
        return integration.hasDotationToBuildObject( entity.source, tostring( obstacle:getType() ) )
    end,

    canDoIt = function( self, entity, obstacle )
        local localisation = obstacle:getLocalisation()
        if localisation ~= nil then
            return integration.canBuildObjectWhenLoadedWithLocation( 
                   entity.source, tostring( obstacle:getType() ), localisation )
        else
            return false
        end
    end
}

local t = initTaskKnowledge( result )
taskKnowledge[ "agent.tasks.knowledges.Bridge" ] = t

return result