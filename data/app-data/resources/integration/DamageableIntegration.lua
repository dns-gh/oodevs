integration.startDamageUrbanBlock = function( urbanBlock )
    urbanBlock.actionDamage = DEC_DetruireBlocUrbain( urbanBlock.source )
    return false
end

integration.stopDamageUrbanBlock = function( urbanBlock )
    if urbanBlock.actionDamage then
        urbanBlock.actionDamage = DEC__StopAction( urbanBlock.actionDamage )
        urbanBlock.actionDamage = nil
    end
    return true
end

integration.getUrbanBlockState = function( urbanBlock ) 
    return DEC_EtatBlocUrbain( urbanBlock.source )
end