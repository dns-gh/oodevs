--Destroyable Implementation
integration.destructionPriority = function( target )
  return integration.reachPriority( target )
end

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

integration.canDestroyIt = function( target )
  local stateROE = DEC_Agent_GetEtatROE()
  if not(eTir == eActionTirDirect_NoAmmo)
    and not(eTir == eActionTirDirect_NoCapacity)
    and not(eTir == eActionTirDirect_Impossible)
    and (stateROE == eRoeStateFreeFire) then
    return true
  else
    return false
  end
end

integration.startDestroyingIt = function( target )
  if not target.actionTir then
    target.actionTir = DEC_StartTirDirect( target.source, 100, eTirDirectNormal, 0 )
    actionCallbacks[ target.actionTir ] = function( arg ) eTir = arg end
  end
end

integration.stopDestroyingIt = function( target )
  DEC_StopAction( target.actionTir )
  target.actionTir = nil
end

integration.destructionLevel = function( target )
  etatOps = ( DEC_ConnaissanceAgent_NiveauDeDestructionTactique( target.source ) ) * 100
  return etatOps
end

integration.getEstimateAttrition = function( target, myself )
  return DEC_ConnaissanceAgent_AttritionPotentielle( target.source )
end
