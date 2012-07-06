-- --------------------------------------------------------------------------------
-- Delays for magic actions in MINUTES
-- --------------------------------------------------------------------------------
local decontaminationDelays = 2 -- min

-- --------------------------------------------------------------------------------
-- Decontamination area
-- --------------------------------------------------------------------------------
method "decontaminateIt" ( masalife.brain.integration.startStopAction( 
{ 
    started = function( self )
        if waitInMin( self, decontaminationDelays ) then -- $$$ temp
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
        return integration.getAreaPositionSecu( self )
    end )

method "getPositions" (
    function( self )
        return integration.getAreaPositionsSecu( self )
    end )

return {}