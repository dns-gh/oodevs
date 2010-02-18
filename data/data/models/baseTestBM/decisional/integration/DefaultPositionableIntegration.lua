--Positionnable Implementation
integration.takeUpPosition = function( reachable )
  return DEC_Agent_NiveauInstallation() >= 4 -- 4 corresponding to E_PostureType::ePosturePoste
end

integration.takeUpPositionObject = function( object )
  if( DEC_ConnaissanceObjet_PeutEtreOccupe( object.source ) ) then
    kBase.me.body.actionOccupy = DEC_StartOccuperObjet( object.source )
  end
  return true
end
