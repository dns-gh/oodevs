-- --Safeguardable Implementation

-- ROE states
local  eRoeStateNone = 0
local  eRoeStateFreeFire = 1
local  eRoeStateRestrictedFire = 2
local  eRoeStateFireByOrder = 3


integration.computeAggressiveness = function( target )

  -- if the unit doesn't have ROE permits to fire, we consider that the ennemi is not aggressive and must not being shot
  local stateROE = DEC_Agent_GetEtatROE()  
  if stateROE == eRoeStateNone 
     or stateROE == eRoeStateFireByOrder then
  return 0
  end
  
  -- else it depends of the agressiviness of ennemi
  local agrAgent = DEC_ConnaissanceAgent_Dangerosite(target.source)
  return (agrAgent-1)*100
end


