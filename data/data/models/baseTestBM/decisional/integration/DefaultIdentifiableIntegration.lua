--Default Observable Implementation

local eRadarType_Radar = 0
local eRadarType_Ecoute = 1
local eRadarType_EcouteRadar = 2

--- Get automat perception
-- @param knowledge on an automat
-- @author LMT
-- @release 2010-02-03
integration.getAutomatPerception = function( unit )
  perceptionLevel = DEC_ConnaissanceAgent_NiveauPerceptionMax( unit.source )
  if ( perceptionLevel == 1 ) then
    return 30
  elseif ( perceptionLevel == 2 ) then
    return 60
  elseif ( perceptionLevel == 3 ) then
    return 100
  else
    return 0   
  end
end

--- Observe a knowledge
-- @param position of a knowledge
-- @author LMT
-- @release 2010-02-04
integration.identifyIt = function( point )
  DEC_Perception_VisionVerrouilleeSurPointXY( point.sim_pos.x, point.sim_pos.y )
  DEC_Perception_ActiverRadarSurPointXY( eRadarType_Radar, point.sim_pos.x, point.sim_pos.y )
end
