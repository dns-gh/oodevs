--Positionnable Implementation
integration.takeUpPosition = function( reachable )
  return DEC_Agent_NiveauInstallation() >= 4 -- 4 corresponding to E_PostureType::ePosturePoste
end

integration.takeUpPositionObject = function( object )
  if( DEC_ConnaissanceObjet_PeutEtreOccupe( object.source ) ) then
    kBase.me.body.actionOccupy = DEC__StartOccuperObjet( object.source )
    BreakForDebug( "occupy")
  end
  return true
end
