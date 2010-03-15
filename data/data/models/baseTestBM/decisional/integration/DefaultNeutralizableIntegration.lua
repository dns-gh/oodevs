-- --Neutralizable Implementation

eTirDirectNormal = 0
eTirDirectLibre = 1


local eActionTirDirect_Impossible = 0
local eActionTirDirect_EnemyDestroyed = 1
local eActionTirDirect_NoCapacity = 2
local eActionTirDirect_NoAmmo = 3
local eActionTirDirect_Running = 4
local eActionTirDirect_Finished = 5
local eActionTirDirect_None = 6

-- ROE states
local  eRoeStateNone = 0
local  eRoeStateFreeFire = 1
local  eRoeStateRestrictedFire = 2
local  eRoeStateFireByOrder = 3


integration.neutralizationPriority  = function( target )
  return integration.reachPriority( target )
end

integration.firePermitted = function(target)
  local stateROE = DEC_Agent_GetEtatROE()
  return stateROE == eRoeStateFreeFire
end

integration.getOperationalCapacity = function(target)
  local etatOps = DEC_ConnaissanceAgent_EtatOps( target.source )*100
  return etatOps
end

integration.startNeutralizingIt = function( target )
  if not target.actionTir then
    target.actionTir = DEC_StartTirDirect( target.source, 100, eTirDirectNormal, 0 )
    actionCallbacks[ target.actionTir ] = function( arg ) target.eTir = arg end
  end
end

integration.stopNeutralizingIt = function( target )
  target.actionTir = DEC_StopAction( target.actionTir )
  target.actionTir = nil
end

integration.getAttritionForNeutralization = function( self, target, position )
  -- gestion des munitions 
  if   eTir == eActionTirDirect_NoAmmo
    or eTir == eActionTirDirect_NoCapacity
    or eTir == eActionTirDirect_Impossible then
    return 0
  end
  -- gestion de perception sur la cible par lancer de rayon
  if not DEC_ConnaissanceAgent_PercoitUnite(target.source) then
    return 0
  end
  -- gestion de la portée
  local pH = 0.5
  local porteeMax    = DEC_Tir_PorteeMaxPourTirerSurUnite( target.source, pH )
  local porteeMin    = DEC_Tir_PorteeMinPourTirerSurUnite( target.source, pH )
  local distanceAEni = integration.magnitude( position, target )
  
  if( distanceAEni > porteeMax or distanceAEni < porteeMin )
    then return 0 
  end  
   
  return 100
  
end