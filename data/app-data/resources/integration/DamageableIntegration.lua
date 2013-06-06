integration.startDamageUrbanBlock = function( urbanBlock )
    urbanBlock.actionDamage = DEC_DetruireBlocUrbain( urbanBlock.source )
    DEC_Population_ChangeUrbanDestructionState( true )
    return false
end

integration.stopDamageUrbanBlock = function( urbanBlock )
    if urbanBlock.actionDamage then
        urbanBlock.actionDamage = DEC__StopAction( urbanBlock.actionDamage )
        urbanBlock.actionDamage = nil
    end
    DEC_Population_ChangeUrbanDestructionState( false )
    return true
end

integration.getUrbanBlockState = function( urbanBlock ) 
    return DEC_EtatBlocUrbain( urbanBlock.source )
end