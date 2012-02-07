return
{
    RC = function( self, ... )
        integration.genericRC( ... )
    end,
    
    getPosition = function( self )
        return DEC_Automate_Position( self.source )
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
