-- --------------------------------------------------------------------------------
-- Delays for magic actions in MINUTES
-- --------------------------------------------------------------------------------
local decontaminationDelays = 2 -- min

-- --------------------------------------------------------------------------------
-- Movement
-- --------------------------------------------------------------------------------
method "reachIt" ( masalife.brain.integration.startStopAction( 
{ 
    start   = function( self )
                  return integration.startMoveToIt( self ) 
              end,
    started = function( self )
                  return integration.updateMoveToIt( self )
              end, 
    stop    = function( self )
                return integration.deselectMoveToIt( self )
              end,
} ) )

-- --------------------------------------------------------------------------------
-- Decontamination area
-- --------------------------------------------------------------------------------
method "decontaminateIt" ( masalife.brain.integration.startStopAction( 
{ 
    started = function( self )
        if waitInMin( self, decontaminationDelays ) then -- $$$ temp
            meKnowledge:sendReport( eRC_DecontaminationDone )
            return integration.decontaminateArea( self )
        end
        meKnowledge:sendReport( eRC_StartingToDecontaminate )
        return false
    end
} ) )

-- --------------------------------------------------------------------------------
-- Information
-- --------------------------------------------------------------------------------
method "getPosition" ( 
    function( self )
        return integration.getAreaPosition( self )
    end )

method "getPositions" ( 
    function( self )
        return integration.getAreaPositions( self )
    end )

return {}