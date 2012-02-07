return
{  
     -- ILLUMINATING
    illuminationEfficiency = function( self, objective )   
        return self:proximityLevel() / 100
    end,
    
    -- POSITIONNABLE
    canTakePosition = function( self )
        return integration.isInIlluminateRange( self.proxy ) -- take position once agent is at good distance.
   end,
    
    predicate "isIlluminatingFor" 
    { 
        dependencies = "none",
        method = function( self, objective )
              return objective == self.proxy
        end 
    },
}

