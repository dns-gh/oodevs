--- Allows the agent to damage a urban block, by decreasing its structural state.
-- @see integration.stopDamageUrbanBlock methods
-- This method can only be used by a Crowd agent.
-- @param urbanBlock a DirectIA urban block knowledge
-- @return Boolean, returns 'false'.
integration.startDamageUrbanBlock = function( urbanBlock )
    urbanBlock.actionDamage = DEC_DetruireBlocUrbain( urbanBlock.source )
    DEC_Population_ChangeUrbanDestructionState( true )
    return false
end

--- Stop the action of damaging a urban block.
-- @see integration.startDamageUrbanBlock methods
-- This method can only be used by a Crowd agent.
-- @param urbanBlock a DirectIA urban block knowledge
-- @return Boolean, returns 'true'.
integration.stopDamageUrbanBlock = function( urbanBlock )
    if urbanBlock.actionDamage then
        urbanBlock.actionDamage = DEC__StopAction( urbanBlock.actionDamage )
        urbanBlock.actionDamage = nil
    end
    DEC_Population_ChangeUrbanDestructionState( false )
    return true
end

--- Returns the structural state for an urban block
-- @param urbanBlock Urban block knowledge
-- @return structural state between 0 and 1 (if value is 1, the urban block has no damaged) 
integration.getUrbanBlockState = function( urbanBlock ) 
    return DEC_EtatBlocUrbain( urbanBlock.source )
end