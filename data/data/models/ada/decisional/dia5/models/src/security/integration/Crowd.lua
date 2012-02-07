predicate "isMounted"
{
    dependencies = "none",
    method = function( self )
        return false
    end
}

predicate "isDismounted"
{
    dependencies = "none",
    method = function( self )
        return true
    end
}

return
{
    RC = function( self, ... )
        integration.genericRC( ... )
    end,

    computeMovementCapability = function( self, position )
        return true
    end,
      
    destroyMoveToItIntegration = function( self, position )
        return integration.stopMoveToItCrowd( position )
    end,
    
    getPosition = function( self )
      return integration.getCrowdPosition( self )
    end,
    
    getTasks = function( self )
        return self.tasks
    end,
    
    moveToItIntegration = function( self, entity, pathType )
        return integration.moveToItCrowd( entity, pathType )
    end,
    
    orientateSensor = function( self, entity )
        return false -- NOTHING for the moment in the simulation
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
    
    canDismount = function()
        return false
    end,
    
    canMount = function()
        return false
    end,
    
    dismount = function()
        return false
    end,
    
    mount = function()
        return false
    end,
    
    getNearestReachingElementFor = function( self, objective )
        return integration.query.getNearestPointFor( objective )
    end,
}
