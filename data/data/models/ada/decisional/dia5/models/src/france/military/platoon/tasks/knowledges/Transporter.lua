local result = 
{   
    fillParameters = function( self, companyTask, params, entity, marcheRetrograde, objectif, taskName )
        local units = companyTask:getTargets( params, entity, taskName )
        local loadingPoint = companyTask:getLoadingPoint( params )
        local destination = companyTask:getDestination( params )
        local meetingPoint = companyTask:getMeetingPoint( params )
        local withEquipment = companyTask:getEquipmentTransportation( params )
        return { units = units, loadingPoint = loadingPoint, destination = destination, meetingPoint = meetingPoint[1], withEquipment = withEquipment }
    end,
    
    canDoIt = function( self, entity, unit, loadable )
       onlyLoadable = not loadable
       local agentToTransport = nil 
       if masalife.brain.core.class.isOfType( unit, sword.military.world.Platoon) then
           agentToTransport = integration.getAgentFromAutomatKnowledge( unit )
       else -- PlatoonAlly
           agentToTransport = unit
       end
       return DEC_Agent_AgentPeutTransporterPion( entity.source, agentToTransport.source, onlyLoadable )
    end,
    
    getObstaclesParameter = function( self, companyTask, params )
        return companyTask:getObstaclesParameter( params )
    end,

    hasDotation = function( self, entity, obstacle )
        return true
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["france.military.platoon.tasks.knowledges.Transporter"] = t

return result
