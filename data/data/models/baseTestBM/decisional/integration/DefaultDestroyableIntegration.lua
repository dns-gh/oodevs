--Destroyable Implementation
integration.destructionPriority = function( target )
  return integration.reachPriority( target )
end

integration.canDestroyIt = function( target )
  --TODO
  return true
end

eTirDirectNormal = 0
eTirDirectLibre = 1


eActionTirDirect_Impossible = 0
eActionTirDirect_EnemyDestroyed = 1
eActionTirDirect_NoCapacity = 2
eActionTirDirect_NoAmmo = 3
eActionTirDirect_Running = 4
eActionTirDirect_Finished = 5
eActionTirDirect_None = 6

integration.destroyIt = function( target )
   eEtatFeu_Executer = 1
   eEtatFeu_aucun = 0
   actionTir = actionTir or nil
   eTir = eTir or nil
   if not actionTir then
     BreakForDebug("Debut tir")
     actionTir = DEC_StartTirDirect( target.source, 100, eTirDirectNormal, 0 )
     actionCallbacks[ actionTir ] = function( arg ) eTir = arg end
  -- elseif eTir == eActionTirDirect_Impossible then
     --TODO Report
   elseif eTir == eActionTirDirect_NoAmmo 
        or eTir == eActionTirDirect_NoCapacity
        or eTir == eActionTirDirect_EnemyDestroyed
        or eTir == eActionTirDirect_Finished then
       BreakForDebug("Il est mort!")
       actionTir = DEC_StopAction( actionTir )
       BreakForDebug("actionTir"..tostring(actionTir))
      --F_Pion_SeteEtatFeu( myself, eEtatFeu_aucun )
      --self.params.__returnValue = eActionEffectuee  TODO FEEDBACK
      --Halt()
   elseif eTir == eActionTirDirect_None
        or eTir == eActionTirDirect_Running then
         BreakForDebug("On fait rien")
      --F_Pion_SeteEtatFeu( myself, eEtatFeu_Executer )
   end  
end

integration.destructionLevel = function( target )
  return DEC_Agent_EtatOps
end

integration.isDestroyed = function( target )
  return DEC_Agent_EstMort
end

integration.dangerosityLevel = function( target )
  return 1.0
end

integration.isDangerous = function( target )
  return true
end