--Reachable Implementation

--TODO use map to convert string to int/ or other  move function approach
eTypeItiMouvement = 0
eTypeItiReconnaissance = 1
eTypeItiInfiltration = 2
eTypeItiAttaque = 3
eTypeItiRepli = 4
eTypeItiAppui = 5
eTypeItiDeminage = 6
eTypeItiLogistique = 7

eEtatActionDeplacement_EnCours = 0
eEtatActionDeplacement_Termine = 1
eEtatActionDeplacement_Pause = 2
eEtatActionDeplacement_ManqueCarburant = 3
eEtatActionDeplacement_NonAutorise = 4
eEtatActionDeplacement_DejaEnDeplacement = 5
eEtatActionDeplacement_ItineraireDoitEtreRejoint = 6


integration.reachPriority = function( point )
  --TODO
	return LinearInterpolation( 0, 1, 200, 5000, false, DEC_Geometrie_Distance( point.source, DEC_Agent_Position() ) )
end

integration.moveToIt = function( point )
  moveAction = moveAction or nil
  if not moveAction then
    it = DEC_CreerItineraire( point.source, eTypeItiMouvement )
    moveAction = DEC_StartDeplacement( it )
    actionCallbacks[ moveAction ] = function( arg ) etat = arg end
  elseif etat == eEtatActionDeplacement_Termine then
    moveAction = DEC_StopAction( moveAction )
    etat = nil
  elseif etat == eEtatActionDeplacement_Pause then
    DEC_ReprendAction( moveAction )
  elseif etat == eEtatActionDeplacement_ManqueCarburant then
    DEC_PauseAction( moveAction )
  elseif etat == eEtatActionDeplacement_NonAutorise then
    --TODO
  elseif etat == eEtatActionDeplacement_DejaEnDeplacement then
    --TODO
  end
  return false
end

integration.isNearby = function( point )
	return DEC_Geometrie_Distance( point.source, self.source) < 500
end

integration.isDistant = function( point )
	return DEC_Geometrie_Distance( point.source, self.source) > 500
end
	
integration.isFar = function( point )
	return DEC_Geometrie_Distance( point.source, self.source) > 2000
end

integration.getProximityLevel = function( point )
	return LinearInterpolation( 0, 1, 200, 5000, false, DEC_Geometrie_Distance( point.source, self.source) )
end

integration.getAccessibilityLevel = function( point )
	--TODO
	return LinearInterpolation( 0, 1, 200, 5000, false, DEC_Geometrie_Distance( point.source, self.source) )
end