--Destroyable Implementation
integration.destructionPriority = function( target )
  return integration.reachPriority( target )
end

eTirDirectNormal = 0
eTirDirectLibre = 1

local actionTir = actionTir or nil
local eTir = eTir or nil

local eActionTirDirect_Impossible = 0
local eActionTirDirect_EnemyDestroyed = 1
local eActionTirDirect_NoCapacity = 2
local eActionTirDirect_NoAmmo = 3
local eActionTirDirect_Running = 4
local eActionTirDirect_Finished = 5
local eActionTirDirect_None = 6

integration.canDestroyIt = function( target )
  if eTir == eActionTirDirect_NoAmmo
    or eTir == eActionTirDirect_NoCapacity
    or eTir == eActionTirDirect_Impossible then
    BreakForDebug( "Cannot Destroy It!" )
    return false
  else
    return true
  end
end

integration.destroyIt = function( target )
  if not actionTir then
    actionTir = DEC_StartTirDirect( target.source, 100, eTirDirectNormal, 0 )
    actionCallbacks[ actionTir ] = function( arg ) eTir = arg end
  end
end

integration.stopDestroyingIt = function( target )
  actionTir = DEC_StopAction( actionTir )
end

integration.destructionLevel = function( target )
  etatOps = ( DEC_ConnaissanceAgent_NiveauDeDestructionTactique( target.source ) ) * 100
  return etatOps
end

integration.getEstimateAttrition = function( target, myself )
  BreakForDebug( "DEC_ConnaissanceAgent_Dangerosite "..tostring( DEC_ConnaissanceAgent_Dangerosite( target ) ) )
  return DEC_ConnaissanceAgent_Dangerosite( target )
end