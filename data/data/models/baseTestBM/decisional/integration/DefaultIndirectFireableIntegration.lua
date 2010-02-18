--IndirectFireable Implementation

integration.canApplyFireOnSection = function ( target, munition, interventionType )
  return true
end

integration.startApplyFireOnSection = function( target, munition, interventionType )
  target.actionIndirectFire = DEC_StartTirIndirectSurConnaissancePtr( munition, interventionType, target.source )
end

integration.stopApplyFireOnIt = function( target )
  DEC_StopAction( target.actionIndirectFire )
  target.actionIndirectFire = nil
end