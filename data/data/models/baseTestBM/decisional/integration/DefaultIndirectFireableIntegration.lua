-------------------------------------------------------------------------------
-- IndirectFireable Implementation : 
-- Regroup function to manage artillery field
-- @author MGD
-- @created 2010-01-22
-- @modified MGD 2010-01-27
--
-- This file is part of a MASA library or program.
-- Refer to the included end-user license agreement for restrictions.
--
-- Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
-------------------------------------------------------------------------------


--- Indicate if the unit has the munition 
-- @param munition The id of a munition
-- @return bool
-- @author MGD
-- @created 2010-01-27
local isIndirectMunitionValid = function( munition )
  return ( not ( DEC_Tir_PorteeMaxTirIndirect( munition ) == -1 ) ) and ( not ( DEC_Tir_PorteeMinTirIndirect( munition ) == -1 ) )
end

--- Indicate if the unit can fire at the target distance with corresponding munition 
-- @param munition The id of a munition
-- @param target The targeted net.masagroup.sword.military.world.Section knowledge
-- @return bool
-- @author MGD
-- @created 2010-01-27
local isRangeValid = function( munition, target)
  local dist = integration.magnitude( masalife.brain.knowledge.me.body, target )
  if dist > DEC_Tir_PorteeMaxTirIndirect_SansTesterEtatMunitions( munition ) then
    return false
  end
  if dist < DEC_Tir_PorteeMinTirIndirect_SansTesterEtatMunitions( munition ) then
    return false
  end
  return true
end

--- Indicate if the unit as the capacity and the right to fire on the target
-- @param target The targeted net.masagroup.sword.military.world.Section knowledge
-- @param munition The id of a munition
-- @param interventionType The number of IT to fire
-- @author MGD
-- @created 2010-01-22
-- @modified MGD 2010-01-27
integration.canApplyFireOnSection = function( target, munition, interventionType, authorizeInopine )
  --if authorizeInopine  || masalife.brain.knowledge.me.body:isReadyToApplyFire() then
    if not isIndirectMunitionValid( munition ) then
      BreakForDebug( "Invalid munition" )
      return false
    elseif not isRangeValid( munition, target ) then
      BreakForDebug( "invalid range" )
      return false
		end
  return true
  --return false
end

--- IndirectFireState
-- @created 2010-01-27
-- @modified MGD 2010-01-27
local eIndirectFireState_Running = 0
local eIndirectFireState_NoCapacity = 1
local eIndirectFireState_Finished = 2
local eeIndirectFireState_Impossible = 3
local eIndirectFireState_NoAmmo = 4

--- Start an IndirectFire action on a target
-- @param target The targeted net.masagroup.sword.military.world.Section knowledge
-- @param munition The id of a munition
-- @param interventionType The number of IT to fire
-- @author MGD
-- @created 2010-01-15
-- @modified MGD 2010-01-27
integration.startApplyFireOnSection = function( target, munition, interventionType )
  target.actionIndirectFire = DEC_StartTirIndirectSurConnaissancePtr( munition, interventionType, target.source )
  actionCallbacks[ target.actionIndirectFire ] = function( arg ) target.eIndirectFireState = arg end
  return false
end

--- Update an IndirectFire action on a target
-- @param target The targeted net.masagroup.sword.military.world.Section knowledge
-- @author MGD
-- @created 2010-01-27
-- @modified MGD 2010-01-27
integration.updateApplyFireOnSection = function( target )
  if target.eIndirectFireState == eIndirectFireState_Finished then
    DEC_StopAction( target.actionIndirectFire )
    target.actionIndirectFire = nil
    target.eIndirectFireState = nil
    return true
  elseif target.eIndirectFireState == eIndirectFireState_NoCapacity then
    --@TODO MGD ADD CR
  elseif target.eIndirectFireState == eIndirectFireState_Impossible then
    --@TODO MGD ADD CR
  elseif target.eIndirectFireState == eIndirectFireState_NoAmmo then
    --@TODO MGD ADD CR
  end  
  return false
end

--- Stop an IndirectFire action on a target
-- @param target The targeted net.masagroup.sword.military.world.Section knowledge
-- @author MGD
-- @created 2010-01-15
-- @modified MGD 2010-01-27
integration.stopApplyFireOnIt = function( target )
  DEC_StopAction( target.actionIndirectFire )
  target.actionIndirectFire = nil
  return false
end