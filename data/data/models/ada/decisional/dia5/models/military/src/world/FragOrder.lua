return
{
    isROE = function( self )
        return integration.isROE( self )
    end,
    setAutomatFragOrder = function( self )
        return integration.setAutomatFragOrder( self )
    end,
    mustBePropagate = function( self )
        return integration.mustBePropagate( self )
    end,
    updateROE = function( self )
       integration.updateROE( self )
    end,
    isTask = function( self )
        return integration.isTask( self )
    end,
    startTask = function( self )
       integration.startFragOrderTask( self )
    end,
    getParameters = function( self )
        return integration.getFireParameters( self )
    end,
    getTargetsForIndirectFireOrder = function( self )
        local targets = {}
        local uggly = function()
            targets[ 1 ] = CreateKnowledge( integration.ontology.types.agentKnowledge, integration.getAgentKnowledgeParameter( self ) )
        end
        if not pcall( uggly ) then
            targets[ 1 ] = CreateKnowledge( integration.ontology.types.point, integration.getpointCibleParameter( self ) )
        end
        return targets
    end,
    getQuantityForIndirectFireOrder = function( self )
        return integration.getNbItParameter( self )
    end,
    getAmmunitionTypeForIndirectFireOrder = function( self )
        return integration.getMunitionsParameter( self )
    end
}