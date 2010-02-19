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

integration.startMoveToIt = function( reachable )
  -- Leave tactical object
  if kBase.me.body.actionOccupy then
    DEC_StopAction( kBase.me.body.actionOccupy )
    BreakForDebug( "unoccuppy")
  end
  if not reachable.moveAction then
    default_engine.methods.occupyPosition( kBase.me.body, nil ) -- UnOccupyPosition
    it = DEC_CreerItineraireBM( reachable.sim_pos.x, reachable.sim_pos.y, reachable.sim_pos.z, eTypeItiMouvement )
    reachable.moveAction = DEC_StartDeplacement( it )
    actionCallbacks[ reachable.moveAction ] = function( arg ) reachable.etat = arg end
  end
  return false
end

integration.updateMoveToIt = function( reachable )
  if reachable.etat == eEtatActionDeplacement_Termine then
    default_engine.methods.occupyPosition( kBase.me.body, reachable ) --OccupyPosition
    reachable.moveAction = DEC_StopAction( reachable.moveAction )
    reachable.moveAction = nil
    reachable.etat = nil
    return true
  elseif reachable.etat == eEtatActionDeplacement_Pause then
    DEC_ReprendAction( reachable.moveAction )
  elseif reachable.etat == eEtatActionDeplacement_ManqueCarburant then
    DEC_PauseAction( reachable.moveAction )
  elseif reachable.etat == eEtatActionDeplacement_NonAutorise then
    --TODO
  elseif reachable.etat == eEtatActionDeplacement_DejaEnDeplacement then
    --TODO
  end
end

integration.stopMoveToIt = function( reachable )
  DEC_StopAction( reachable.moveAction )
  reachable.moveAction = nil
  reachable.etat = nil
  return false
end