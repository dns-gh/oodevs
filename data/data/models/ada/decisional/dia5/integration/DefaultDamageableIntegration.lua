integration.startDamageUrbanBlock = function( target )
    target.actionDamage = DEC_DetruireBlocUrbain( target.source )
end

integration.stopDamageUrbanBlock = function( target )
    if target.actionDamage then
        target.actionDamage = DEC__StopAction( target.actionDamage )
        target.actionDamage = nil
    end
end

integration.getUrbanBlockState = function( urbanBlock ) 
    return DEC_EtatBlocUrbain( urbanBlock.source )
end