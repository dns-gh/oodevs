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

integration.magnitude = function ( pos1, pos2 )
    return math.sqrt( ( pos1.sim_pos.x - pos2.sim_pos.x )^2 + (pos1.sim_pos.y - pos2.sim_pos.y )^2 + ( pos1.sim_pos.z - pos2.sim_pos.z )^2 )
end

integration.normalizedInversedDistance = function( pos1, pos2 )
  return LinearInterpolation( 0, 100, 10, 10000, false, integration.magnitude( pos1, pos2 ) )
end

integration.moveToIt = function( reachable)
  moveAction = moveAction or nil
  if not moveAction then
    it = DEC_CreerItineraireBM( reachable.sim_pos.x, reachable.sim_pos.y, reachable.sim_pos.z, eTypeItiMouvement )
    moveAction = DEC_StartDeplacement( it )
    actionCallbacks[ moveAction ] = function( arg ) etat = arg end
  elseif etat == eEtatActionDeplacement_Termine then
    moveAction = DEC_StopAction( moveAction )
    moveAction = nil
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