return
{
    setMustBeEvaluate = function( self, value )
        self.mustBeEvaluate_ = value
    end,
    mustBeEvaluate = function( self )
        return self.mustBeEvaluate_
    end,
    currentObjective = 
    {
        getter = function( self ) 
            if self.currentObjective_ == nil then 
                self.currentObjective_ = 1
            end
            return self.currentObjective_
        end,
        setter = function( self, value ) 
            self.currentObjective_ = value
        end
    },
}
