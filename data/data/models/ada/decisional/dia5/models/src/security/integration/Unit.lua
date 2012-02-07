predicate "isMounted"
{
    dependencies = "none",
    method = function( self )
        return integration.isMounted( self )
    end
}
predicate "isDismounted"
{
    dependencies = "none",
    method = function( self )
        return integration.isDismounted( self )
    end
}

return 
{  
    extinguish = function( self, element )
        return element:extinguishIt()
    end,
    
    moveToItIntegration = function( self, entity, pathType )
        return integration.moveToItGeneric( entity, pathType )
    end,
    
    destroyMoveToItIntegration = function( self, position )
        return integration.stopMoveToIt( position )
    end,
    
	  RC = function( self, ... )
        integration.genericRC( ... )
    end,
    
    getPosition = function( self )
        -- $$$ MGD:TODO replace by shared_ptr< DEC_Knowledge_Agent_ABC > and proxy for allies
        if self == meKnowledge then
            return DEC_Agent_Position() -- Me
        else
            return DEC_Agent_PositionPtr( self.source ) -- a friend 
        end
    end,
    
    getProximity = function( self, position )
        return DEC_Geometrie_Distance( self:getPosition(), position:getPosition() )
    end,


    orientateSensor = function( self, entity )
        return integration.observeIt( entity )
    end,
    
    computeMovementCapability = function( self, position )
        return position:getTraficabilityLevelFor( self, integration.isMounted( self ) )
    end,
    
    canMount = function( self ) 
        return integration.canMount( self )
    end,
    canDismount = function( self )
        return integration.canDismount( self )
    end,
    mount = masalife.brain.integration.startStopAction( 
    {
        start = function( self ) return integration.mount( self ) end,
        stop  = function( self ) integration.stopMount( self ) end
    } ),
    dismount = masalife.brain.integration.startStopAction(
    {
        start = function( self ) return integration.dismount( self ) end,
        stop  = function( self ) integration.stopDismount( self ) end
    } ),
    getNearestReachingElementFor = function( self, objective )
        return integration.query.getNearestPointFor( objective )
    end,
    
    getTasks = function( self )
        return self.tasks
    end,
    
    tasks =
    {
        adder = function( self, task ) error( "unused method", 2 ) end,
        accepter = function( self, visitor )
            local function roleVisitor( role )
                if role.body == self then return role.tasks:accept( visitor ) end
            end
            for group in masalife.brain.knowledge.executeQuery( { knowledgeType = { directia.core.model.Group } } ) do
                return group.roles:accept( roleVisitor )
            end
        end,
        remover = function( self, task )
            error( "unused method", 2 )
        end
     },
}